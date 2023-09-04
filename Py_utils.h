#ifndef PY_UTILS
#define PY_UTILS
#define PY_SSIZE_T_CLEAN
#include <Python.h>

/* Function to convert double** to Python object */
PyObject *convert_to_python_object(double **arr, int rows, int cols);

/* Function to convert Python object to double** */
double **convert_to_double_array(PyObject *obj);

#define get_2d_shape(lst)              \
    int shape[2] = {0, 0};             \
    shape[0] = PyObject_Length((lst)); \
    shape[1] = shape[0] > 0 ? PyObject_Length(PyObject_GetItem((lst), 0)) : 0

#endif
