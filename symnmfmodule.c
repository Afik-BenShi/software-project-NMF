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

    /* TODO:
     * implement sym call
     * convert to PyObject and return
     */
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
    double **points = convert_to_double_array(Py_points);
    /* call ddg */

    /* TODO:
     * implement ddg call
     * convert to PyObject and return
     */
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

    /* TODO:
     * implement norm call
     * convert to PyObject and return
     */
}

/* module's function table*/
static PyMethodDef SymNMF_FunctionsTable[] = {
    {"sym",        /* name exposed to Python */
     sym_wrapper,  /* C wrapper function*/
     METH_VARARGS, /* received variable args (but really just 1) */
     "Calculates the similarity matrix from the points 
     according to sec 1.1 in the pdf "/* documentation */
    },
    {"ddg",        /* name exposed to Python */
     ddg_wrapper,  /* C wrapper function*/
     METH_VARARGS, /* received variable args (but really just 1) */
     "Calculates the diagonal degree matrix 
     according to sec 1.2 in the PDF " /* documentation */
    },
    {"norm",       /* name exposed to Python */
     norm_wrapper, /* C wrapper function*/
     METH_VARARGS, /* received variable args (but really just 1) */
     "Calculate the normalized similarity matrix 
     according to sec 1.3 in the PDF " /* documentation */
    },
    {NULL, NULL, 0, NULL}};

// modules definition
static struct PyModuleDef SymNMF_Module = {
    PyModuleDef_HEAD_INIT,
    "symnmf", // name of module exposed to Python
    "Sym NMF module with functions to calculate: 
    Similarity matrix,
    Diagonam Degree matrix,
    and Normalied Similarity matrix ", // module documentation
        - 1,
    SymNMF_FunctionsTable};

PyMODINIT_FUNC PyInit_SymNmf(void)
{
    return PyModule_Create(&SymNMF_Module);
}