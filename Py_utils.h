#ifndef PY_UTILS
#define PY_UTILS
#define PY_SSIZE_T_CLEAN
#include <Python.h>

/* Function to convert double** to Python object */
PyObject *convert_to_python_object(double **arr, int rows, int cols);

/* Function to convert Python object to double** */
double **convert_to_double_array(PyObject *obj);

/* Function to return the shape of the list*/
int *get_2d_shape(PyObject *lst);

#endif
