/*C interface*/
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
    normalized_sym = (double *)malloc(line_num * sizeof(double));
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

/**
 * prints a 2D matrix of doubles
*/
void matrix_printer(double **matrix, int rows, int cols){
    return;
}

int main(int argc, char *argv)
{
    int i, j, dim, line_num;
    char *goal = argv[1], *filename = argv[2];
    double **points, **sym_mat, **ddg_mat, **result;
    char *file_buffer, **lines;
    
    /* file reading and parsing to points*/
    file_buffer = read_file(filename); 
    dim = get_dimention(file_buffer);
    line_num = count_lines(file_buffer);
    lines = split_to_lines(file_buffer, line_num);
    points = lines_to_points(lines, line_num, dim);

    /* free up memory */
    free_2d(lines, line_num);
    /* don't need to free file_buffer, becase lines point to the same location */

    sym_mat = sym(points, line_num, dim);
    if (goal == "sym"){
        result = sym_mat;
    }
    if (goal == "ddg")
    {
        result = ddg(sym_mat, line_num);
        free_2d(sym_mat, line_num);
    }
    if (goal == "norm")
    {
        ddg_mat = ddg(sym_mat, line_num);
        result = norm(sym_mat, ddg_mat, line_num);
        free_2d(sym_mat, line_num);
        free_2d(ddg_mat, line_num);
    }
    /* print and free all memory */
    matrix_printer(result, line_num, line_num);
    free_2d(result, line_num);
    return 1;
}