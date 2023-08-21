/*C header file*/
# ifndef SYMNMF_H_
# define SYMNMF_H_
# include "utils.h"

/* Calculates the similarity matrix from the points
 * according to sec 1.1 in the pdf*/
double **sym(double **points, int line_num, int dim);

/* Calculate the diagonal degree matrix
* according to sec 1.2 in the PDF */
double **ddg(double **sym_mat, int line_num);

/* Calculate the normalized similarity matrix
* according to sec 1.3 in the PDF*/
double **norm(double **sym_mat, double **ddg_mat, int line_num);
# endif
