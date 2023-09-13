"""Python interface"""
import sys
import numpy as np
import math
import symnmfmodule

MAX_ITER = 300
EPSILON = 1e-4
ERROR_MSG = "An Error Has Occurred"


def general_error_and_exit():
    print(ERROR_MSG)
    sys.exit()


def try_float(st):
    try:
        return float(st)
    except:
        general_error_and_exit()


def init_decomposition_matrix(norm_matrix, k):
    ''' init_decomposition_matrix randomly initialize decomposition matrix with values from the interval [0, 2 * sqrt(m/k)], where m is the average of
        all entries of norm_matrix.'''
    np.random.seed(0)

    n = len(norm_matrix)
    min_val = 0
    max_val = 2 * (((np.average(norm_matrix)) / (k)) ** 0.5)

    initial_matrix = np.random.uniform(min_val, max_val, size=(n, k))
    return initial_matrix


def multiply_matrices(matrix1, matrix2):
    '''multiply_matrices calculates matrix multiplication.'''
    result = np.dot(matrix1, matrix2)
    return result


def subtract_matrices(matrix1, matrix2):
    '''subtract_matrices calculates matrix subtraction.'''
    result = matrix1 - matrix2
    return result


def divide_matrices(matrix1, matrix2):
    '''divide_matrices calculates matrix division.'''
    result = np.divide(matrix1, matrix2)
    return result


def transpose_matrix(matrix):
    '''transpose_matrix calculates the transposed matrix of a given matrix.'''
    result = np.transpose(matrix)
    return result


def frob_norm(matrix):
    '''frob_norm calaculates the frobenius norm of a given matrix.'''
    return np.linalg.norm(np.array(matrix), 'fro')


def update_decomposition_matrix(initial_decomp_matrix, norm_matrix):
    '''update_decomposition_matrix updates given matrix iteratively.'''
    decomp_matrix = initial_decomp_matrix.copy()
    new_decomp_matrix = initial_decomp_matrix.copy()
    beta = 0.5
    i = 0

    while (i < MAX_ITER):

        numerator = multiply_matrices(norm_matrix, decomp_matrix)
        denominator = multiply_matrices(multiply_matrices(
            decomp_matrix, transpose_matrix(decomp_matrix)), decomp_matrix)
        new_decomp_matrix = subtract_matrices(multiply_matrices(decomp_matrix, (1 - beta)),
                                              divide_matrices(numerator, denominator))

        if (frob_norm(subtract_matrices(new_decomp_matrix, decomp_matrix)) ** 2 < EPSILON):
            break

        decomp_matrix = new_decomp_matrix
        i += 1

    return decomp_matrix


def find_decomposition_matrix(norm_matrix, k):
    ''' find_decomposition_matrix findes the decomposition matrix H.'''
    initial_matrix = init_decomposition_matrix(norm_matrix, k)
    updated_matrix = update_decomposition_matrix(initial_matrix, norm_matrix)
    return updated_matrix


def check_inputs(k, goal, input_file):
    ''' check_inputs validates the required inputs in main function.'''
    if ((not k.isnumeric()) or (not goal in ["symnmf", "sym", "ddg", "norm"]) or (not input_file.endswith(".txt"))):
        general_error_and_exit()


def input_loader(filename):
    """load input file as list of strings
    and turns lines into list of points"""
    try:
        with open(filename, "r") as f:
            lines = f.readlines()
    except:
        general_error_and_exit()

    return np.array([[try_float(num) for num in line.split(",")] for line in lines])


def print_mat(mat):
    lines = [",".join(row) for row in mat]
    print("\n".join(lines))


def main(args=sys.argv):
    if (len(args) < 3):
        general_error_and_exit()
    k = args[1]
    goal = args[2]
    input_file = args[3]
    print("args loaded")
    check_inputs(k, goal, input_file)
    print("inputs checked")
    points = input_loader(input_file)
    print("text file loaded")

    result = [[]]
    if goal == "symnmf":
        # symnmf as described in sec 1 in the pdf
        print("goal symnmf")
        norm_mat = symnmfmodule.norm(list(points))
        print("norm calculated")
        initial_decomp = init_decomposition_matrix(points, k)
        print("initial calculated")
        result = symnmfmodule.symnmf(list(initial_decomp), norm_mat)
        print("symnmf calculated")

    elif goal == "sym":
        # sym as described in sec 1.1 in the pdf
        print("goal sym")
        result = symnmfmodule.sym(list(points))
        print("norm calculated")
    elif goal == "ddg":
        # ddg as described in sec 1.2 in the pdf
        print("goal ddg")
        result = symnmfmodule.ddg(list(points))
        print("norm calculated")
    elif goal == "norm":
        # norm as described in sec 1.3 in the pdf
        print("goal norm")
        result = symnmfmodule.norm(list(points))
        print("norm calculated")
    print_mat(result)


if __name__ == "__main__":
    main()
