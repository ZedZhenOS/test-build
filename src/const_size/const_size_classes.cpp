#include <pybind11/pybind11.h>
#include <string>
#include "const_size_classes.hpp"

#ifndef ZEDZHENOS_CONST_SIZE_CLASSES
#define ZEDZHENOS_CONST_SIZE_CLASSES

namespace const_size {
    std::string bytes8::name() { return "const_size.bytes8"; }

    std::string bytes16::name() { return "const_size.bytes16"; }

    std::string bytes32::name() { return "const_size.bytes32"; }

    std::string bytes64::name() { return "const_size.bytes64"; }


    std::string uint8::name() { return "const_size.uint8"; }

    std::string uint16::name() { return "const_size.uint16"; }

    std::string uint32::name() { return "const_size.uint32"; }

    std::string uint64::name() { return "const_size.uint64"; }
}

namespace std {
    std::size_t hash<const_size::uint8>::operator()(const const_size::uint8 &s) const noexcept {
        return static_cast<uint8_t>(s);
    }

    std::size_t hash<const_size::uint16>::operator()(const const_size::uint16 &s) const noexcept {
        return static_cast<uint16_t>(s);
    }

    std::size_t hash<const_size::uint32>::operator()(const const_size::uint32 &s) const noexcept {
        return static_cast<uint32_t>(s);
    }

    std::size_t hash<const_size::uint64>::operator()(const const_size::uint64 &s) const noexcept {
        return static_cast<uint64_t>(s);
    }
}
#endif
