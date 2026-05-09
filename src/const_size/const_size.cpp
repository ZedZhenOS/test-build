#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include "const_size_classes.hpp"

#ifndef ZEDZHENOS_CONST_SIZE
#define ZEDZHENOS_CONST_SIZE

namespace const_size {
    namespace py = pybind11;

    PYBIND11_MODULE(_const_size, m) {
        register_bytesN<bytes8>(m, "bytes8");
        register_bytesN<bytes16>(m, "bytes16");
        register_bytesN<bytes32>(m, "bytes32");
        register_bytesN<bytes64>(m, "bytes64");

        register_uintN<uint8>(m, "uint8");
        register_uintN<uint16>(m, "uint16");
        register_uintN<uint32>(m, "uint32");
        register_uintN<uint64>(m, "uint64");

        std::vector<std::string> all = {
            "bytes8", "bytes16", "bytes32", "bytes64",
            "uint8", "uint16", "uint32", "uint64"
        };
        py::tuple py_all = py::cast(all);
        m.attr("__all__") = py_all;
    }
}
#endif
