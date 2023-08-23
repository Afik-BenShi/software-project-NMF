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
    int *shape = get_2d_shape(Py_points);
    if (shape == NULL)
    {
        return NULL;
    }

    /* convert to matrix */
    double **points = convert_to_double_array(Py_points);

    /* call sym */
    double **sym_mat = sym(points, shape[0], shape[1]);
    free_2d((void *)points, shape[0]);

    PyObject *Py_sym = convert_to_python_object(sym_mat, shape[0], shape[0]);
    free_2d((void *)sym_mat, shape[0]);
    free(shape);
    return Py_sym;
}

static PyObject *ddg_wrapper(PyObject *self, PyObject *args)
{
    PyObject *Py_sym;
    if (!PyArg_ParseTuple(args, "O", &Py_sym))
    {
        return NULL;
    }
    int line_num = PyObject_Length(Py_sym);
    if (line_num < 0)
    {
        return NULL;
    }

    /* convert to matricies */
    double **sym_mat = convert_to_double_array(Py_sym);

    /* call ddg */
    double **ddg_mat = ddg(sym_mat, line_num);
    free_2d((void *)sym_mat, line_num);

    /* convert to python list */
    PyObject *Py_ddg = convert_to_python_object(ddg_mat, line_num, line_num);
    free_2d((void *)ddg_mat, line_num);
    return Py_ddg;
}

static PyObject *norm_wrapper(PyObject *self, PyObject *args)
{
    PyObject *Py_sym, *Py_ddg;
    if (!PyArg_ParseTuple(args, "OO", &Py_sym, &Py_ddg))
    {
        return NULL;
    }
    int line_num = PyObject_Length(Py_ddg);
    if (line_num < 0)
    {
        return NULL;
    }

    /* convert to matricies */
    double **sym_mat = convert_to_double_array(Py_sym);
    double **ddg_mat = convert_to_double_array(Py_ddg);

    /* call norm */
    double **norm_mat = norm(sym_mat, ddg_mat, line_num);
    free_2d((void *)sym_mat, line_num);
    free_2d((void *)ddg_mat, line_num);

    /* convert to python list*/
    PyObject *Py_norm = convert_to_python_object(norm_mat, line_num, line_num);
    free_2d((void *)norm_mat, line_num);
    return Py_norm;
}

/* module's function table*/
static PyMethodDef SymNMF_FunctionsTable[] = {
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
    {NULL, NULL, 0, NULL}};

// modules definition
static struct PyModuleDef SymNMF_Module = {
    PyModuleDef_HEAD_INIT,
    "symnmf",                                                                                                                           // name of module exposed to Python
    "Sym NMF module with functions to calculate:\n\tSimilarity matrix,\n\tDiagonam Degree matrix,\n\tand Normalied Similarity matrix ", // module documentation
    -1,
    SymNMF_FunctionsTable};

PyMODINIT_FUNC PyInit_SymNmf(void)
{
    return PyModule_Create(&SymNMF_Module);
}