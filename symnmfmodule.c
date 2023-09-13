/*Python C API wrapper*/
#include "symnmf.h"
#include "Py_utils.h"

static PyObject *sym_wrapper(PyObject *self, PyObject *args)
{
    PyObject *Py_points;
    if (!PyArg_ParseTuple(args, "O", &Py_points))
    {
        return NULL;
    }
    get_2d_shape(Py_points);

    /* convert to matrix */
    double **points = convert_to_double_array(Py_points);

    /* call sym */
    double **sym_mat = sym(points, shape[0], shape[1]);
    free_2d((void *)points, shape[0]);

    PyObject *Py_sym = convert_to_python_object(sym_mat, shape[0], shape[0]);
    free_2d((void *)sym_mat, shape[0]);

    return Py_sym;
}

static PyObject *ddg_wrapper(PyObject *self, PyObject *args)
{
    PyObject *Py_points;
    if (!PyArg_ParseTuple(args, "O", &Py_points))
    {
        return NULL;
    }
    get_2d_shape(Py_points);
    if (shape[0] < 0 || shape[1] < 0)
    {
        return NULL;
    }
    /* convert to matrix */
    double **points = convert_to_double_array(Py_points);

    /* call sym */
    double **sym_mat = sym(points, shape[0], shape[1]);
    free_2d((void *)points, shape[0]);

    /* call ddg */
    double **ddg_mat = ddg(sym_mat, shape[0]);
    free_2d((void *)sym_mat, shape[0]);

    /* convert to python list */
    PyObject *Py_ddg = convert_to_python_object(ddg_mat, shape[0], shape[0]);
    free_2d((void *)ddg_mat, shape[0]);

    return Py_ddg;
}

static PyObject *norm_wrapper(PyObject *self, PyObject *args)
{
    PyObject *Py_points;
    if (!PyArg_ParseTuple(args, "O", &Py_points))
    {
        return NULL;
    }
    get_2d_shape(Py_points);
    if (shape[0] < 0 || shape[1] < 0)
    {
        return NULL;
    }
    /* convert to matrix */
    double **points = convert_to_double_array(Py_points);

    /* call sym */
    double **sym_mat = sym(points, shape[0], shape[1]);
    free_2d((void *)points, shape[0]);

    /* call ddg */
    double **ddg_mat = ddg(sym_mat, shape[0]);

    /* call norm */
    double **norm_mat = norm(sym_mat, ddg_mat, shape[0]);
    free_2d((void *)sym_mat, shape[0]);
    free_2d((void *)ddg_mat, shape[0]);

    /* convert to python list*/
    PyObject *Py_norm = convert_to_python_object(norm_mat, shape[0], shape[0]);
    free_2d((void *)norm_mat, shape[0]);

    return Py_norm;
}

static PyObject *symnmf_wrapper(PyObject *self, PyObject *args)
{
    PyObject *Py_init_decomp, *Py_norm;
    double epsilon = 0.0;
    if (!PyArg_ParseTuple(args, "OOd", &Py_init_decomp, &Py_norm, epsilon))
    {
        return NULL;
    }
    get_2d_shape(Py_init_decomp);
    if (shape[0] < 0 || shape[1] < 0)
    {
        return NULL;
    }

    /* convert to matricies */
    double **init_decomp_mat = convert_to_double_array(Py_init_decomp);
    double **norm_mat = convert_to_double_array(Py_norm);

    double **decomp_mat = symnmf(init_decomp_mat, norm_mat, shape[0], shape[1], epsilon);
    free_2d((void *)norm_mat, shape[0]);
    free_2d((void *)init_decomp_mat, shape[0]);

    /* convert to python list*/
    PyObject *Py_decomp = convert_to_python_object(decomp_mat, shape[0], shape[1]);
    free_2d((void *)decomp_mat, shape[0]);

    return Py_decomp;
}

/* module's function table*/
static PyMethodDef symnmfmodule_FunctionsTable[] = {
    {
        "sym",                                                                               /* name exposed to Python */
        sym_wrapper,                                                                         /* C wrapper function*/
        METH_VARARGS,                                                                        /* received variable args (but really just 1) */
        "Calculates the similarity matrix from the points\naccording to sec 1.1 in the pdf " /* documentation */
    },
    {
        "ddg",                                                                    /* name exposed to Python */
        ddg_wrapper,                                                              /* C wrapper function*/
        METH_VARARGS,                                                             /* received variable args (but really just 1) */
        "Calculates the diagonal degree matrix\naccording to sec 1.2 in the PDF " /* documentation */
    },
    {
        "norm",                                                                        /* name exposed to Python */
        norm_wrapper,                                                                  /* C wrapper function*/
        METH_VARARGS,                                                                  /* received variable args (but really just 1) */
        "Calculate the normalized similarity matrix\naccording to sec 1.3 in the PDF " /* documentation */
    },
    {
        "symnmf",                                                                        /* name exposed to Python */
        symnmf_wrapper,                                                                  /* C wrapper function*/
        METH_VARARGS,                                                                    /* received variable args (but really just 1) */
        "Calculate the optimized decomposition matrix\naccording to sec 1.4 in the PDF " /* documentation */
    },
    {NULL, NULL, 0, NULL}};

// modules definition
static struct PyModuleDef symnmfmodule_Module = {
    PyModuleDef_HEAD_INIT,
    "symnmfmodule",                                                                                                                     // name of module exposed to Python
    "Sym NMF module with functions to calculate:\n\tSimilarity matrix,\n\tDiagonam Degree matrix,\n\tand Normalied Similarity matrix ", // module documentation
    -1,
    symnmfmodule_FunctionsTable};

PyMODINIT_FUNC PyInit_symnmfmodule(void)
{
    return PyModule_Create(&symnmfmodule_Module);
}