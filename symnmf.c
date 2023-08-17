/*C interface*/
#include<string.h>
#include "symnmf.h" 
/* gives us file loading, array tools,
 stdlib.h, stdio.h, math.h */

/* Calculates the similarity matrix from the points
 * according to sec 1.1 in the pdf*/
double **sym(double **points, int line_num, int dim){
    int i,j;
    double **A;
    A = (double **)malloc(line_num*sizeof(double *));
    pointer_check((void*) A, GENERAL_ERROR);

    for (i = 0; i < line_num; i++){
        A[i] = (double *)malloc(line_num*sizeof(double));
        pointer_check((void *)A[i], GENERAL_ERROR);
        
        for (j = 0; j < line_num; j++){
            A[i][j] = i!=j? 
                exp(-sq_distance(points[i], points[j], dim) / 2) 
                : 0;
        }
    }
    return A;
}

double **ddg(double **sym_mat, int line_num){
    int i, j;
    double **D, 
    *sum_cols_sym = sum_columns(sym_mat, line_num, line_num);

    D = (double **)malloc(line_num * sizeof(double *));
    pointer_check((void *)D, GENERAL_ERROR);

    for (i = 0; i < line_num; i++)
    {
        D[i] = (double *)malloc(line_num * sizeof(double));
        pointer_check((void *)D[i], GENERAL_ERROR);

        for (j = 0; j < line_num; j++)
        {
            D[i][j] = i!=j? 0 
                : sum_cols_sym[i];
        }
    }
    free(sum_cols_sym);
    return D;
}


double **norm(double **sym_mat, double **ddg_mat, int line_num){
    int i, j;
    double *inv_sqrt_ddg, **normalized_sym;
    inv_sqrt_ddg = (double *)malloc(line_num * sizeof(double));
    pointer_check((void *)inv_sqrt_ddg, GENERAL_ERROR);

    /* calculate ddg^-0.5 (only the diagonal => 1D)*/
    for (i = 0; i < line_num; i++)
    {
        inv_sqrt_ddg[i] = -1/sqrt(ddg_mat[i][i]);
    }

    /* multiply matricies */
    normalized_sym = (double **)malloc(line_num * sizeof(double *));
    pointer_check((void *)normalized_sym, GENERAL_ERROR);
    for (i = 0; i < line_num; i++)
    {
        normalized_sym[i] = (double *)malloc(line_num * sizeof(double));
        pointer_check((void *)normalized_sym[i], GENERAL_ERROR);
        for (j=0; j< line_num; j++){
            normalized_sym[i][j] = inv_sqrt_ddg[i]*sym_mat[i][j]*inv_sqrt_ddg[j];
        }
    }
    free(inv_sqrt_ddg);
    return normalized_sym;
}

/* prints a 2D matrix of doubles
*/
void matrix_printer(double **matrix, int rows, int cols){
    int i, j;
    for ( i = 0; i < rows; i++)
    {
        for ( j = 0; j < cols; j++)
        {
            if (j!=0){
                printf(", ");
            }
            printf("%f", matrix[i][j]);
        }
        printf("\n");
    }
    return;
}

int main(int argc, char **argv)
{
    int dim, line_num;
    char *goal, *filename;
    double **points, **sym_mat, **ddg_mat, **norm_mat;
    char *file_buffer, **lines;
    if (argc < 3){
        printf("%s", GENERAL_ERROR);
        exit(1);
    }
    goal = argv[1];
    filename = argv[2];

    /* file reading and parsing to points*/
    file_buffer = read_file(filename); 
    dim = get_dimention(file_buffer);
    line_num = count_lines(file_buffer);
    lines = split_to_lines(file_buffer, line_num);
    points = lines_to_points(lines, line_num, dim);

    sym_mat = sym(points, line_num, dim);
    free_2d((void *)points, line_num);
    if (strcmp(goal, "sym") == 0){
        norm_mat = sym_mat;
        matrix_printer(sym_mat, line_num, line_num);
        free_2d((void *)sym_mat, line_num);
    }
    else if (strcmp(goal, "ddg") == 0)
    {
        ddg_mat = ddg(sym_mat, line_num);
        matrix_printer(ddg_mat, line_num, line_num);
        free_2d((void *)sym_mat, line_num);
        free_2d((void *)ddg_mat, line_num);
    }
    else if (strcmp(goal, "norm") == 0)
    {
        ddg_mat = ddg(sym_mat, line_num);
        norm_mat = norm(sym_mat, ddg_mat, line_num);
        matrix_printer(norm_mat, line_num, line_num);
        free_2d((void *)sym_mat, line_num);
        free_2d((void *)ddg_mat, line_num);
        free_2d((void *)norm_mat, line_num);
    }
    /* print and free all memory */
    return 1;
}
