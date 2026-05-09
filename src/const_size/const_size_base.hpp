#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <string>

#ifndef ZEDZHENOS_CONST_SIZE_BASE_H
#define ZEDZHENOS_CONST_SIZE_BASE_H

namespace const_size {
    namespace py = pybind11;
    using namespace py::literals;

    static const py::int_ *PY_ZERO = new py::int_(0);
    static const py::str *PY_BIG = new py::str("big");
    static const py::str *PY_LITTLE = new py::str("little");

    py::int_ normal_index(py::int_ index, const py::int_ &max_index);

    template<std::size_t N>
    class bytesN {
        std::byte data[N];

        inline static const py::int_ *MAX_INDEX = new py::int_(N);

    public:
        explicit bytesN(const std::byte data[N]) {
            for (std::size_t i = 0; i < N; ++i) {
                this->data[i] = data[i];
            }
        }

        bytesN(const bytesN &data) : bytesN(data.data) {
        }

        explicit bytesN(const py::bytes &data) {
            if (py::len(data) == N) {
                std::string s = data;
                for (std::size_t i = 0; i < N; ++i) {
                    this->data[i] = reinterpret_cast<std::byte &>(s[i]);
                }
            } else {
                throw py::value_error();
            }
        }

        py::bytes bytes() const {
            char data_[N];
            for (std::size_t i = 0; i < N; ++i) {
                data_[i] = reinterpret_cast<const char &>(this->data[i]);
            }
            return py::bytes(data_, N);
        }

        const std::byte &operator[](std::size_t i) const {
            return data[i];
        }

        std::byte &operator[](std::size_t i) {
            return data[i];
        }

        py::bytes py_getitem(py::int_ i) const {
            char byte = static_cast<char>(data[normal_index(i, *MAX_INDEX)]);
            return {&byte, 1};
        }

        virtual std::string name() = 0;

        py::str repr() {
            return py::str(name() + "(") + py::repr(bytes()) + py::str(")");
        }
    };

    template<typename uint_type, typename bytes_type, typename real_type>
    class uintN {
        uint_type data;

        inline static const py::int_ *MIN_VALUE = new py::int_(0);
        inline static const py::int_ *MAX_VALUE = new py::int_(static_cast<uint_type>(-1));
        static constexpr std::size_t N = sizeof(uint_type);

        inline static const uint_type one = 1;

        template<typename T>
        class _BitsReference {
        protected:
            T value;
            const py::object o;
            inline static const py::int_ *MAX_INDEX = new py::int_(8 * N);

            _BitsReference(T value, const py::object &o) : value(value), o(o) {
            }

        public:
            bool operator[](std::size_t index) const {
                return (value >> index) & one;
            }

            bool get_bit(py::int_ i) const {
                return (*this)[normal_index(i, *MAX_INDEX)];
            }
        };

    public:
        class BitsReference;

        class BitReference {
            uint_type &value;
            const std::size_t bit;

        public:
            BitReference(uint_type &value, std::size_t bit) : value(value), bit(bit) {
            }

            BitReference(BitsReference &value, std::size_t bit) : value(value.value), bit(bit) {
            }

            BitReference(BitReference &&value) = delete;

            BitReference(const BitReference &value) : value(value.value), bit(value.bit) {
            }

            operator bool() const {
                return (value >> bit) & one;
            }


            bool operator=(bool value) {
                if (value) {
                    this->value |= one << this->bit;
                } else {
                    this->value &= ~(one << this->bit);
                }
                return value;
            }
        };

        class BitsReference : public _BitsReference<uint_type &> {
            friend BitReference;

        public:
            BitsReference(uint_type &value, const py::object &o) : _BitsReference<uint_type &>(value, o) {
            }

            BitsReference(const BitsReference &value) : _BitsReference<uint_type &>(value.value, value.o) {
            }

            BitReference operator[](std::size_t index) {
                return BitReference(*this, index);
            }

            bool set_bit(py::int_ i, bool value) {
                return (*this)[static_cast<std::size_t>(normal_index(i, *this->MAX_INDEX))] = value;
            }
        };

        class ConstBitsReference : public _BitsReference<const uint_type &> {
        public:
            ConstBitsReference(const uint_type &value, const py::object &o) : _BitsReference<const uint_type &>(
                value, o) {
            }

            ConstBitsReference(const ConstBitsReference &&value) : _BitsReference<const uint_type &>(
                value.value, value.o) {
            }

            ConstBitsReference(const ConstBitsReference &value) : _BitsReference<const uint_type &>(
                value.value, value.o) {
            }
        };

        using T_int = uint_type;

        uintN() : uintN(static_cast<uint_type>(0)) {
        }

        explicit uintN(const uint_type &data) : data(data) {
        }

        uintN(const real_type &data) : uintN(data.data) {
        }

        explicit uintN(const py::int_ &data) {
            if (*MIN_VALUE <= data && data < *MAX_VALUE) {
                this->data = data;
            } else {
                throw py::value_error();
            }
        }

        bytes_type to_bytes(const py::str &byteorder = *PY_BIG) const {
            if (PY_BIG->equal(byteorder)) {
                std::byte res[N];
                for (std::size_t i = 0; i < N; ++i) {
                    res[N - 1 - i] = static_cast<std::byte>((data >> (8 * i)) & 0xFF);
                }
                return bytes_type(res);
            }
            if (PY_LITTLE->equal(byteorder)) {
                std::byte res[N];
                for (std::size_t i = 0; i < N; ++i) {
                    res[i] = static_cast<std::byte>((data >> (8 * i)) & 0xFF);
                }
                return bytes_type(res);
            }
            throw py::value_error();
        }

        static real_type from_bytes(const bytes_type &data, const py::str &byteorder = *PY_BIG) {
            if (PY_BIG->equal(byteorder)) {
                uint_type res = 0;
                for (std::size_t i = 0; i < N; ++i) {
                    res <<= 8;
                    res |= std::to_integer<uint8_t>(data[i]);
                }
                return real_type(res);
            }
            if (PY_LITTLE->equal(byteorder)) {
                uint_type res = 0;
                for (std::size_t i = N - 1; i < N; --i) {
                    res <<= 8;
                    res |= std::to_integer<uint8_t>(data[i]);
                }
                return real_type(res);
            }
            throw py::value_error();
        }

        py::int_ index() const {
            return py::int_(data);
        }

        BitsReference bits() {
            auto b = BitsReference(data, py::cast(this));
            return b;
        }

        ConstBitsReference bits() const {
            return ConstBitsReference(data, py::cast(this));
        }

        virtual std::string name() = 0;

        py::str repr() {
            return py::str(name() + "(") + py::repr(index()) + py::str(")");
        }

        explicit operator uint_type() const {
            return data;
        }

        friend bool operator==(const real_type &value1, const real_type &value2) {
            return value1.data == value2.data;
        }

        real_type &operator|=(const real_type &value) {
            data |= value.data;
            return static_cast<real_type &>(*this);
        }

        friend real_type operator|(real_type value1, const real_type &value2) {
            return value1 |= value2;
        }
    };

    template<typename T>
    void register_bytesN(py::module_ m, const char *name) {
        py::class_<T, py::smart_holder>(m, name, py::is_final())
                .def(py::init<const T &>(), "data"_a, py::pos_only())
                .def(py::init<const py::bytes &>(), "data"_a, py::pos_only())
                .def("__bytes__", &T::bytes)
                .def("__getitem__", &T::py_getitem)
                .def("__add__", [](const T &data1, const py::bytes &data2) { return data1.bytes() + data2; },
                     py::is_operator(), "other"_a, py::pos_only())
                .def("__radd__", [](const T &data1, const py::bytes &data2) { return data2 + data1.bytes(); },
                     py::is_operator(), "other"_a, py::pos_only())
                .def("__repr__", &T::repr);
    }

    template<typename T>
    void register_uintN(py::module_ m, std::string name) {
        using BitsReference = typename T::BitsReference;
        py::class_<T, py::smart_holder> cls(m, name.c_str(), py::is_final());
        py::class_<BitsReference, py::smart_holder>(cls, "BitsReference", py::is_final())
                .def("__getitem__", &BitsReference::get_bit, "index"_a, py::pos_only())
                .def("__setitem__", &BitsReference::set_bit, "index"_a, "value"_a, py::pos_only());
        cls
                .def(py::init<>())
                .def(py::init<const typename T::T_int &>(), "data"_a, py::pos_only())
                .def(py::init<const py::int_ &>(), "data"_a, py::pos_only())
                .def(py::init<const T &>(), "data"_a, py::pos_only())
                .def("to_bytes", &T::to_bytes, "byteorder"_a = "big", py::pos_only())
                .def_static("from_bytes", &T::from_bytes, "data"_a, "byteorder"_a = "big", py::pos_only())
                .def("__index__", &T::index)
                .def("bits", static_cast<BitsReference(T::*)()>(&T::bits))
                .def("__repr__", &T::repr)
                .def(py::self |= py::self, "other"_a, py::pos_only())
                .def(py::self | py::self, "other"_a, py::pos_only())
                .def(py::self == py::self, "other"_a, py::pos_only());
    }
}
#endif
