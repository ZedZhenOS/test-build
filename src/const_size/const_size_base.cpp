#include <pybind11/pybind11.h>
#include "const_size_base.hpp"

#ifndef ZEDZHENOS_CONST_SIZE_BASE
#define ZEDZHENOS_CONST_SIZE_BASE

namespace const_size {
    namespace py = pybind11;

    py::int_ normal_index(py::int_ index, const py::int_ &max_index) {
        if (index < *PY_ZERO) {
            index = *max_index + index;
            if (index < *PY_ZERO) {
                throw py::index_error();
            }
        }
        if (index >= *max_index) {
            throw py::index_error();
        }
        return index;
    }
}
#endif
