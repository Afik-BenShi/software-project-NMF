'''setup file'''
from setuptools import Extension, setup

module = Extension(
    "symnmf",
    sources=[
        'symnmf.h', 
        'Py_utils.h', 
        './impls/Py_utils_impl.c', 
        'symnmfmodule.c'
    ]
)
setup(
    name='symnmf',
    version='1.0',
    description='''
    Sym NMF module with functions to calculate: 
    Similarity matrix,
    Diagonam Degree matrix,
    and Normalied Similarity matrix ''',
    ext_modules=[module]
)