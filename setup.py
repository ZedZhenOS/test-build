from glob import glob
from os import environ

from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

setup(
    cmdclass={"build_ext": build_ext},
    ext_modules=[
        Pybind11Extension(
            "const_size._const_size",
            sorted(glob("src/const_size/*.cpp")),
            depends=sorted(glob("src/const_size/*.hpp")),
            optional=environ.get("CIBUILDWHEEL", '0') != '1',
            cxx_std=20
        ),
    ],
)
