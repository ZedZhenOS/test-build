__all__ = ('bytes8', 'bytes16', 'bytes32', 'bytes64', 'uint8', 'uint16', 'uint32', 'uint64')

try:
    from const_size._const_size import bytes8, bytes16, bytes32, bytes64, uint8, uint16, uint32, uint64
except ImportError:
    from const_size.const_size import bytes8, bytes16, bytes32, bytes64, uint8, uint16, uint32, uint64
