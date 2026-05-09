__all__ = ('bytes8', 'bytes16', 'bytes32', 'bytes64', 'uint8', 'uint16', 'uint32', 'uint64')

from typing_extensions import final

from const_size.const_size._base import bytesN, uintN, uintNMeta


@final
class bytes8(bytesN, length=1):
    pass


@final
class bytes16(bytesN, length=2):
    pass


@final
class bytes32(bytesN, length=4):
    pass


@final
class bytes64(bytesN, length=8):
    pass


class _uint8Meta(uintNMeta):
    def __new__(metacls, name, bases, attrs, **kw):
        kw['length'] = 1
        return super().__new__(metacls, name, bases, attrs, **kw)

    @property
    def _type(cls) -> type[bytes8]:
        return bytes8


@final
class uint8(uintN[bytes8], metaclass=_uint8Meta):
    pass


class _uint16Meta(uintNMeta):
    def __new__(metacls, name, bases, attrs, **kw):
        kw['length'] = 2
        return super().__new__(metacls, name, bases, attrs, **kw)

    @property
    def _type(cls) -> type[bytes16]:
        return bytes16


@final
class uint16(uintN[bytes16], metaclass=_uint16Meta):
    pass


class _uint32Meta(uintNMeta):
    def __new__(metacls, name, bases, attrs, **kw):
        kw['length'] = 4
        return super().__new__(metacls, name, bases, attrs, **kw)

    @property
    def _type(cls) -> type[bytes32]:
        return bytes32


@final
class uint32(uintN[bytes32], metaclass=_uint32Meta):
    pass


class _uint64Meta(uintNMeta):
    def __new__(metacls, name, bases, attrs, **kw):
        kw['length'] = 8
        return super().__new__(metacls, name, bases, attrs, **kw)

    @property
    def _type(cls) -> type[bytes64]:
        return bytes64


@final
class uint64(uintN[bytes64], metaclass=_uint64Meta):
    pass
