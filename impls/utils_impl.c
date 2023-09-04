#include "../utils.h"

/* ============ Array Tools ============ */

/* prints a 2D matrix of doubles
 */
void matrix_printer(double **matrix, int rows, int cols)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            if (j != 0)
            {
                printf(", ");
            }
            printf("%f", matrix[i][j]);
        }
        printf("\n");
    }
    return;
}

/*  frees 2d arrays. Array needs to be cast into void** */
void free_2d(void **mat, int rows)
{
    int i;
    for (i = 0; i < rows; i++)
    {
        free(mat[i]);
    }

    free(mat);
}

/* Checks that an allocated pointer actually has somewhere to point to */
void pointer_check(void *ptr, const char *error_msg)
{
    if (ptr == NULL)
    {
        printf("%s", error_msg);
        exit(1);
    }

    return;
}

/* Creates an array of points that are all 0.
 * len - length of the array
 * Checks new pointers  */
double **empty_points_arr(int len, int dim)
{
    int i, j;
    double **arr = (double **)malloc(len * sizeof(double *));
    pointer_check((void *)arr, GENERAL_ERROR);

    for (i = 0; i < len; i++)
    {
        arr[i] = (double *)malloc(dim * sizeof(double));
        pointer_check((void *)arr[i], GENERAL_ERROR);

        for (j = 0; j < dim; j++)
        {
            arr[i][j] = 0.0;
        }
    }

    return arr;
}

/* Copies the first n points from one array to a new one.
 * Checks new pointers */
double **point_array_copy(double **points, int n, int dim)
{
    int i, j;
    double **new_points = (double **)malloc(n * sizeof(double *));
    pointer_check((void *)new_points, GENERAL_ERROR);

    for (i = 0; i < n; i++)
    {
        new_points[i] = (double *)malloc(dim * sizeof(double));
        pointer_check((void *)new_points[i], GENERAL_ERROR);
        /* copy the values directly and not by pointing */
        for (j = 0; j < dim; j++)
        {
            new_points[i][j] = points[i][j];
        }
    }

    return new_points;
}

/* Sum over the columns of a 2D array.
 * sum_columns({{1,5},{0,3}}) = {6, 3}*/
double *sum_columns(double **arr, int rows, int cols)
{
    int i, j;
    double *sum_cols, curr_col;
    sum_cols = (double *)malloc(rows * sizeof(double));
    for (i = 0; i < rows; i++)
    {
        curr_col = 0;
        for (j = 0; j < cols; j++)
        {
            curr_col += arr[i][j];
        }
        sum_cols[i] = curr_col;
    }
    return sum_cols;
}

/* Squared Euclidian distance between p and q.
 * Formula d=sqrt(pow(p[0]-q[0], 2) + ... + pow(p[n]-q[n], 2)) */
double sq_distance(double *p, double *q, int dim)
{
    int i;
    double dist = 0.0;
    for (i = 0; i < dim; i++)
    {
        dist += pow((p[i] - q[i]), 2);
    }

    return dist;
}
double distance(double *p, double *q, int dim)
{
    return sqrt(sq_distance(p, q, dim));
}

/* calculate_average return the average value of the matrix
 * calculate_average({{1,5},{0,4}}) = 2.5 */
double calculate_average(double **matrix, int rows, int cols)
{
    double sum = 0.0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            sum += matrix[i][j];
        }
    }
    return sum / (rows * cols);
}

void multiply_matrices(double **matrix1, int rows1, int cols1, double **matrix2, int rows2, int cols2, double **result)
{
    for (int i = 0; i < rows1; i++)
    {
        for (int j = 0; j < cols2; j++)
        {
            result[i][j] = 0.0;
            for (int k = 0; k < cols1; k++)
            {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

void subtract_matrices(double **matrix1, int rows1, int cols1, double **matrix2, int rows2, int cols2, double **result)
{
    for (int i = 0; i < rows1; i++)
    {
        for (int j = 0; j < cols1; j++)
        {
            result[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }
}

void divide_matrices(double **matrix1, int rows1, int cols1, double **matrix2, int rows2, int cols2, double **result)
{
    for (int i = 0; i < rows1; i++)
    {
        for (int j = 0; j < cols1; j++)
        {
            result[i][j] = matrix1[i][j] / matrix2[i][j];
        }
    }
}

void transpose_matrix(double **matrix, int rows, int cols, double **result)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[j][i] = matrix[i][j];
        }
    }
}

double frob_norm_sq(double **matrix, int rows, int cols)
{
    double sum = 0.0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            sum += matrix[i][j] * matrix[i][j];
        }
    }
    return sum;
}

/* ============ File Read and String Manipulation ============*/

/* Reads the file and saves it to a string.
 * Checks new pointers */
char *read_file(char *filename)
{
    char *buffer;
    long size;
    FILE *file = NULL;
    file = fopen(filename, "r");
    pointer_check((void *)file, GENERAL_ERROR);

    fseek(file, 0L, SEEK_END); /* go to end of file*/
    size = ftell(file);        /* save the size*/
    fseek(file, 0L, SEEK_SET); /* go to start of file*/

    /* allocate string to hold the file data */
    buffer = (char *)malloc(size + 1);
    pointer_check((void *)buffer, GENERAL_ERROR);

    /* read and save to buffer*/
    fread(buffer, size, 1, file);
    buffer[size] = '\0';

    fclose(file);

    return buffer;
}

/* Counts the dimention of the points
 */
int get_dimention(char *st)
{
    int i;
    int dim_ = 0;
    for (i = 0; (st[i] != '\n') && (st[i] != '\0'); i++)
    {
        /* count when seeing a comma */
        dim_ += (st[i] == ',');
    }

    /* add one if the first char is a number
     * this distinguishes if line has a number or is empty */
    dim_ += (st[0] <= '9' && st[0] >= '0') || st[0] == '-';

    return dim_;
}

/* Counts the number of lines in the string
 */
int count_lines(char *st)
{
    int i;
    int line_cnt = 0;
    pointer_check((void *)st, "Error: null pointer passed to count_lines\n");

    for (i = 0; st[i] != '\0'; i++)
    {
        line_cnt += (st[i] == '\n');
    }

    return line_cnt;
}

/* Take the string and split it to lines array.
 * Warning: This replaces any '\n' with a '\0' in the original string,
 * as the data is not copied to a new location in memory.
 * Checks new pointers */
char **split_to_lines(char *st, int lines_count)
{
    char **lines_arr;
    int i, j;

    lines_arr = (char **)malloc(lines_count * sizeof(char *));
    pointer_check((void *)lines_arr, GENERAL_ERROR);

    lines_arr[0] = &st[0]; /* point to first line*/
    for (i = 0, j = 1; st[i] != '\0'; i++)
    {
        if (st[i] == '\n')
        {
            st[i] = '\0';              /* split the string*/
            lines_arr[j] = &st[i + 1]; /* point to start of next line*/
            j++;
        }
    }

    return lines_arr;
}

/* Takes a single line, splits it to numbers and returns a double array representing a point.
 * Warning: This splits the string while running and mends it back in the end.
 * Checks new pointers */
double *read_point(char *line, int dim)
{
    int i;
    double *point = (double *)malloc(dim * sizeof(double));
    pointer_check((void *)point, GENERAL_ERROR);

    for (i = 0; i < dim; i++)
    {
        int char_cnt;
        double number;
        char curr;
        /* count the number of chars until the next ',' or '\0' */
        for (char_cnt = 0;; char_cnt++)
        {
            curr = line[char_cnt];
            if (curr == ',' || curr == '\0')
            {
                line[char_cnt] = '\0';       /* mark end of number */
                number = strtod(line, NULL); /* convert */
                line[char_cnt] = curr;       /* revert changes */
                break;
            }
        }
        if (curr != '\0')
        {
            line = &line[char_cnt + 1];
        }
        point[i] = number;
    }

    return point;
}

/* Takes an array of lines, splits them to numbers
 * and returns an array of points from the lines.
 * Warning: This replaces any ',' with a '\0' in each of the lines.
 * as the data is not copied to a new location in memory.
 * Checks new pointers */
double **lines_to_points(char **lines, int line_num, int dim)
{
    int i;
    double **points = (double **)malloc(line_num * sizeof(double *));
    pointer_check((void *)points, GENERAL_ERROR);

    for (i = 0; i < line_num; i++)
    {
        char *line = lines[i];
        points[i] = read_point(line, dim);
    }

    return points;
}
