# ifndef UTILS_H_
# define UTILS_H_
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>

#ifndef GENERAL_ERROR
# define GENERAL_ERROR ("An Error Has Occurred\n")
# endif
/* ============ Array Tools ============ */

/* prints a 2D matrix of doubles
*/
void matrix_printer(double **matrix, int rows, int cols);

/*  frees 2d arrays. Array needs to be cast into void** */
void free_2d(void **mat, int rows);

/* Checks that an allocated pointer actually has somewhere to point to */
void pointer_check(void *ptr, const char *error_msg);

/* Creates an array of points that are all 0.
 * len - length of the array
 * Checks new pointers  */
double **empty_points_arr(int len, int dim);

/* Copies the first k points from one array to a new one.
 * Checks new pointers */
double **point_array_copy(double **points, int k, int dim);

/* Sum over the columns of a 2D array. 
 * sum_columns({{1,5},{0,3}}) = {6, 3}*/
double* sum_columns(double **arr, int rows, int cols);

/* Squared Euclidian distance between p and q.
 * Formula d=sqrt(pow(p[0]-q[0], 2) + ... + pow(p[n]-q[n], 2)) */
double sq_distance(double *p, double *q, int dim);

double distance(double *p, double *q, int dim);

/* ============ File Read and String Manipulation ============*/

/* Reads the file and saves it to a string.
    * Checks new pointers */
char *read_file(char *filename);

/* Counts the dimention of the points
 */
int get_dimention(char *st);

/* Counts the number of lines in the string
 */
int count_lines(char *st);

/* Take the string and split it to lines array.
 * Warning: This replaces any '\n' with a '\0' in the original string,
 * as the data is not copied to a new location in memory.
 * Checks new pointers */
char **split_to_lines(char *st, int lines_count);
/* Takes a single line, splits it to numbers and returns a double array representing a point.
 * Warning: This splits the string while running and mends it back in the end.
 * Checks new pointers */
double *read_point(char *line, int dim);

/* Takes an array of lines, splits them to numbers
 * and returns an array of points from the lines.
 * Warning: This replaces any ',' with a '\0' in each of the lines.
 * as the data is not copied to a new location in memory.
 * Checks new pointers */
double **lines_to_points(char **lines, int line_num, int dim);
# endif
