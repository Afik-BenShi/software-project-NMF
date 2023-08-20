#include "../symnmf.h"


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
