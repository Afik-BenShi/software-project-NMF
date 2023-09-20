#include "../Py_utils.h"

/* Function to convert double** to Python object */
PyObject *convert_to_python_object(double **arr, int rows, int cols)
{
    int i, j;
    PyObject *obj = PyList_New(rows);

    for (i = 0; i < rows; i++)
    {
        PyObject *row = PyList_New(cols);

        for (j = 0; j < cols; j++)
        {
            PyObject *item = PyFloat_FromDouble(arr[i][j]);
            if (!item)
            {
                /* Clean up and return NULL if conversion fails */
                Py_DECREF(row);
                Py_DECREF(obj);
                return NULL;
            }
            PyList_SetItem(row, j, item);
        }

        PyList_SetItem(obj, i, row);
    }

    return obj;
}

/* Function to convert Python object to double** */
double **convert_to_double_array(PyObject *obj)
{
    Py_ssize_t i, j;
    Py_ssize_t rows = PyList_Size(obj);
    double **arr = (double **)malloc(rows * sizeof(double *));

    for (i = 0; i < rows; i++)
    {
        PyObject *row = PyList_GetItem(obj, i);
        Py_ssize_t cols = PyList_Size(row);
        arr[i] = (double *)malloc(cols * sizeof(double));

        for (j = 0; j < cols; j++)
        {
            PyObject *item = PyList_GetItem(row, j);
            if (!PyFloat_Check(item))
            {
                /* Clean up and return NULL if item is not a float */
                for (int k = 0; k < i; k++)
                {
                    free(arr[k]);
                }
                free(arr);
                return NULL;
            }
            arr[i][j] = PyFloat_AsDouble(item);
        }
    }

    return arr;
}
