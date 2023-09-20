'''setup file'''
from setuptools import Extension, setup

module = Extension(
    "symnmfmodule",
    sources=[
        './impls/utils_impl.c',
        './impls/symnmf_impl.c',
        './impls/Py_utils_impl.c',
        'symnmfmodule.c',
    ]
)
setup(
    name='symnmfmodule',
    version='1.0',
    description='''
    Sym NMF module with functions to calculate: 
    Similarity matrix,
    Diagonam Degree matrix,
    and Normalied Similarity matrix ''',
    ext_modules=[module]
)
