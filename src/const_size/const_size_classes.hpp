#include <pybind11/pybind11.h>
#include <string>
#include "const_size_base.hpp"

#ifndef ZEDZHENOS_CONST_SIZE_CLASSES_H
#define ZEDZHENOS_CONST_SIZE_CLASSES_H

namespace const_size {
    class bytes8 final : public bytesN<1> {
    public:
        using bytesN::bytesN;

        std::string name() override;
    };

    class bytes16 final : public bytesN<2> {
    public:
        using bytesN::bytesN;

        std::string name() override;
    };

    class bytes32 final : public bytesN<4> {
    public:
        using bytesN::bytesN;

        std::string name() override;
    };

    class bytes64 final : public bytesN<8> {
    public:
        using bytesN::bytesN;

        std::string name() override;
    };


    class uint8 final : public uintN<uint8_t, bytes8, uint8> {
    public:
        using uintN::uintN;

        std::string name() override;
    };

    class uint16 final : public uintN<uint16_t, bytes16, uint16> {
    public:
        using uintN::uintN;

        std::string name() override;
    };

    class uint32 final : public uintN<uint32_t, bytes32, uint32> {
    public:
        using uintN::uintN;

        std::string name() override;
    };

    class uint64 final : public uintN<uint64_t, bytes64, uint64> {
    public:
        using uintN::uintN;

        std::string name() override;
    };
}

namespace std {
    template<>
    struct hash<const_size::uint8> {
        std::size_t operator()(const const_size::uint8 &s) const noexcept;
    };

    template<>
    struct hash<const_size::uint16> {
        std::size_t operator()(const const_size::uint16 &s) const noexcept;
    };

    template<>
    struct hash<const_size::uint32> {
        std::size_t operator()(const const_size::uint32 &s) const noexcept;
    };

    template<>
    struct hash<const_size::uint64> {
        std::size_t operator()(const const_size::uint64 &s) const noexcept;
    };
}
#endif
