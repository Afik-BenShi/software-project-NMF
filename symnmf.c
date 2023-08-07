/*C interface*/
#include "symnmf.h" 
/* gives us file loading, array tools,
 stdlib.h, stdio.h, math.h */

/* Calculates the similarity matrix from the points
 * according to sec 1.1 in the pdf*/
double** sym(double **points, int line_num, int dim){
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



int main(int argc, char *argv)
{
    int i, j;
    char *goal = argv[1], *filename = argv[2];
    if (goal == "sym")
    {
    }
    else if (goal == "ddg")
    {
    }
    else if (goal == "norm")
    {
    }
    return 1;
}