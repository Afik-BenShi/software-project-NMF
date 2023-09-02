/*C interface*/
#include "symnmf.h"
/* gives us file loading, array tools,
 stdlib.h, stdio.h, math.h, string.h */

int main(int argc, char **argv)
{
    int dim, line_num;
    char *goal, *filename;
    double **points, **sym_mat, **ddg_mat, **norm_mat;
    char *file_buffer, **lines;
    if (argc < 3)
    {
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
    if (strcmp(goal, "sym") == 0)
    {
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
    else if (strcmp(goal, "symnmf") == 0)
    {
        ddg_mat = ddg(sym_mat, line_num);
        norm_mat = norm(sym_mat, ddg_mat, line_num);
        initial_decomp_mat = init_decomposition_matrix(norm_mat, dim, );
        free_2d((void *)sym_mat, line_num);
        free_2d((void *)ddg_mat, line_num);
        free_2d((void *)norm_mat, line_num);
    }
    /* print and free all memory */
    return 1;
}
