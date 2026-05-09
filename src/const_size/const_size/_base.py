from abc import abstractmethod, ABC

from typing_extensions import final, Self, Literal, Any

try:
    from const_size._const_size import bytes8, bytes16, bytes32, bytes64, uint8, uint16, uint32, uint64

    bytes_classes = (bytes8, bytes16, bytes32, bytes64)
    uint_classes = (uint8, uint16, uint32, uint64)
    HAS_CPP = True
except ImportError:
    HAS_CPP = False


@final
class BitsReference:
    def __init__(self, value: uintN[Any], bit_length: int) -> None:
        self._value = value
        self._bit_length = bit_length

    def _shift(self, x: int) -> int:
        if x >= 0:
            if x >= self._bit_length:
                raise IndexError
            return 1 << x
        else:
            return self._shift(-x - 1)

    def __getitem__(self, item: int) -> bool:
        return bool(self._value._value & self._shift(item))

    def __setitem__(self, item: int, value: bool) -> None:
        if value:
            self._value._value |= self._shift(item)
        else:
            self._value._value &= ~self._shift(item)


def __init_subclass__(cls, **kwargs):
    raise TypeError


class bytesMeta(type):
    __length: int | None

    def __new__(metacls, name, bases, attrs, length: int | None = None, **kw):
        cls = super().__new__(metacls, name, bases, attrs, **kw)
        cls.__length = length
        return cls

    @property
    def _length(cls) -> int:
        if cls.__length is None:
            raise TypeError
        return cls.__length

    if HAS_CPP:
        def __instancecheck__(cls, instance):
            return super().__instancecheck__(instance) or isinstance(instance, bytes_classes)

        def __subclasscheck__(cls, subclass):
            return super().__subclasscheck__(subclass) or issubclass(subclass, bytes_classes)


class bytesN(bytes, metaclass=bytesMeta):
    def __new__(cls, value: bytes) -> Self:
        if isinstance(value, cls):
            return super().__new__(cls, value)
        if isinstance(value, bytes):  # pyright: ignore[reportUnnecessaryIsInstance]
            if len(value) == cls._length:
                return super().__new__(cls, value)
            raise ValueError
        raise TypeError

    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        cls.__init_subclass__ = __init_subclass__


class uintNMeta(type, ABC):
    __length: int | None

    def __new__(metacls, name, bases, attrs, length: int | None = None, **kw):
        attrs['_type'] = property(lambda self: type(self)._type)
        attrs['_length'] = property(lambda self: type(self)._length)
        cls = super().__new__(metacls, name, bases, attrs, **kw)
        cls.__length = length
        return cls

    @property
    def _length(cls) -> int:
        if cls.__length is None:
            raise TypeError
        return cls.__length

    @property
    @abstractmethod
    def _type(cls) -> type[bytesN]:
        raise TypeError

    if HAS_CPP:
        def __instancecheck__(cls, instance):
            return super().__instancecheck__(instance) or isinstance(instance, uint_classes)

        def __subclasscheck__(cls, subclass):
            return super().__subclasscheck__(subclass) or issubclass(subclass, uint_classes)


class uintN[T: bytesN](metaclass=uintNMeta):
    _value: int

    @property
    def _length(self) -> int:
        return type(self)._length

    @property
    def _type(self) -> type[T]:
        return type(self)._type

    def __new__(cls, value: int) -> Self:
        if isinstance(value, cls):
            return super().__new__(cls)
        if isinstance(value, int):
            if 0 <= value < 2 ** (8 * cls._length):
                return super().__new__(cls)
            raise ValueError
        raise TypeError

    def __init__(self, value: int) -> None:
        self._value = value

    def __init_subclass__(cls, **kwargs):
        super().__init_subclass__(**kwargs)
        cls.__init_subclass__ = __init_subclass__

    def to_bytes(self, byteorder: Literal["little", "big"] = "big") -> T:
        return self._type(self._value.to_bytes(self._length, byteorder))

    @classmethod
    def from_bytes(cls, value: T, byteorder: Literal["little", "big"] = "big") -> Self:
        if isinstance(value, cls._type):
            return cls(int.from_bytes(value, byteorder))
        raise TypeError

    @property
    def bits(self) -> BitsReference:
        return BitsReference(self, self._length * 8)
