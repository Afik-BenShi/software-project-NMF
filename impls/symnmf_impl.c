#include "../symnmf.h"

double **sym(double **points, int line_num, int dim)
{
    int i, j;
    double **A;
    A = (double **)malloc(line_num * sizeof(double *));
    pointer_check((void *)A, GENERAL_ERROR);

    for (i = 0; i < line_num; i++)
    {
        A[i] = (double *)malloc(line_num * sizeof(double));
        pointer_check((void *)A[i], GENERAL_ERROR);

        for (j = 0; j < line_num; j++)
        {
            A[i][j] = i != j ? exp(-sq_distance(points[i], points[j], dim) / 2)
                             : 0;
        }
    }
    return A;
}

double **ddg(double **sym_mat, int line_num)
{
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
            D[i][j] = i != j ? 0
                             : sum_cols_sym[i];
        }
    }
    free(sum_cols_sym);
    return D;
}

double **norm(double **sym_mat, double **ddg_mat, int line_num)
{
    int i, j;
    double *inv_sqrt_ddg, **normalized_sym;
    inv_sqrt_ddg = (double *)malloc(line_num * sizeof(double));
    pointer_check((void *)inv_sqrt_ddg, GENERAL_ERROR);

    /* calculate ddg^-0.5 (only the diagonal => 1D)*/
    for (i = 0; i < line_num; i++)
    {
        inv_sqrt_ddg[i] = -1 / sqrt(ddg_mat[i][i]);
    }

    /* multiply matricies */
    normalized_sym = (double **)malloc(line_num * sizeof(double *));
    pointer_check((void *)normalized_sym, GENERAL_ERROR);
    for (i = 0; i < line_num; i++)
    {
        normalized_sym[i] = (double *)malloc(line_num * sizeof(double));
        pointer_check((void *)normalized_sym[i], GENERAL_ERROR);
        for (j = 0; j < line_num; j++)
        {
            normalized_sym[i][j] = inv_sqrt_ddg[i] * sym_mat[i][j] * inv_sqrt_ddg[j];
        }
    }
    free(inv_sqrt_ddg);
    return normalized_sym;
}

int MAX_ITER = 300;

/**
 * calculate the change between iterations of the decomposition matrix
 * and rerurns the squared Frovinious norm of the delta
 */
double normedDiff(double **decomp_matrix, double **new_decomp_matrix, int n, int k)
{
    double squared_norm;
    double **diff = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++)
    {
        diff[i] = (double *)malloc(k * sizeof(double));
    }
    subtract_matrices(new_decomp_matrix, n, k, decomp_matrix, n, k, diff);

    squared_norm = frob_norm_sq(diff, n, k);
    free_2d((void *)diff, n);
    return squared_norm;
}

void update_decomposition_matrix(double **initial_decomp_matrix, double **norm_matrix, int n, int k, double epsilon, double **result)
{
    int i, j;
    double beta = 0.5, squared_norm_change;
    double **decomp_matrix,
        **new_decomp_matrix,
        **numerator,
        **decomp_transpose,
        **denominator,
        **partial_denom;

    decomp_matrix = point_array_copy(initial_decomp_matrix, n, k);

    new_decomp_matrix = (double **)malloc(n * sizeof(double *));
    numerator = (double **)malloc(n * sizeof(double *));
    decomp_transpose = (double **)malloc(k * sizeof(double *));
    partial_denom = (double **)malloc(n * sizeof(double *));
    denominator = (double **)malloc(n * sizeof(double *));
    for (i = 0; i < n; i++)
    {
        new_decomp_matrix[i] = (double *)malloc(k * sizeof(double));
        numerator[i] = (double *)malloc(k * sizeof(double));
        partial_denom[i] = (double *)malloc(n * sizeof(double));
        denominator[i] = (double *)malloc(k * sizeof(double));
    }
    for (
        i = 0; i < k;
        decomp_transpose[i++] = (double *)malloc(n * sizeof(double)))
        ;
    for (i = 0; i < MAX_ITER; i++)
    {

        multiply_matrices(norm_matrix, n, n, decomp_matrix, n, k, numerator);
        transpose_matrix(decomp_matrix, n, k, decomp_transpose);
        multiply_matrices(decomp_matrix, n, k, decomp_transpose, k, n, partial_denom);
        multiply_matrices(partial_denom, n, n, decomp_matrix, n, k, denominator);

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < k; j++)
            {
                new_decomp_matrix[i][j] = decomp_matrix[i][j] * (1.0 - beta + (beta * numerator[i][j] / denominator[i][j]));
            }
        }

        squared_norm_change = normedDiff(decomp_matrix, new_decomp_matrix, n, k);
        if (squared_norm_change < epsilon)
        {
            break;
        }

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < k; j++)
            {
                decomp_matrix[i][j] = new_decomp_matrix[i][j];
            }
        }
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < k; j++)
        {
            result[i][j] = decomp_matrix[i][j];
        }
    }

    for (i = 0; i < n; i++)
    {
        free(decomp_matrix[i]);
        free(new_decomp_matrix[i]);
        free(numerator[i]);
        free(partial_denom[i]);
        free(denominator[i]);
    }
    free(numerator);
    free(partial_denom);
    free(denominator);
    free(decomp_matrix);
    free(new_decomp_matrix);
    free_2d((void *)decomp_transpose, k);
}

double **symnmf(double **initial_matrix, double **norm_matrix, int line_num, int k, double epsilon)
{
    double **result_matrix = (double **)malloc(line_num * sizeof(double *));
    for (int i = 0; i < line_num; i++)
    {
        result_matrix[i] = (double *)malloc(k * sizeof(double));
    }

    update_decomposition_matrix(initial_matrix, norm_matrix, line_num, k, epsilon, result_matrix);

    free_2d((void *)initial_matrix, line_num);

    return result_matrix;
}
