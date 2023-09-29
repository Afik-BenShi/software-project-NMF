"""Python interface"""
import sys
import numpy as np
import symnmfmodule
np.random.seed(0)

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
    '''
    init_decomposition_matrix randomly initialize decomposition matrix
    with values from the interval [0, 2 * sqrt(m/k)], where m is the average of
    all entries of norm_matrix.
    '''

    n = len(norm_matrix)
    min_val = 0
    max_val = 2 * (((np.average(norm_matrix)) / (k)) ** 0.5)

    initial_matrix = np.random.uniform(min_val, max_val, size=(n, k))
    return initial_matrix.tolist()


def check_inputs(k, goal, input_file):
    ''' check_inputs validates the required inputs in main function.'''
    if (
        (not k.isnumeric()) or
        (goal not in ["symnmf", "sym", "ddg", "norm"]) or
        (not input_file.endswith(".txt"))
    ):
        general_error_and_exit()


def input_loader(filename):
    """load input file as list of strings
    and turns lines into list of points"""
    try:
        with open(filename, "r") as f:
            lines = f.readlines()
    except:
        general_error_and_exit()

    return [[try_float(num) for num in line.split(",")] for line in lines]


def print_mat(mat):
    def to_str(row): return (f"{cell:.4f}" for cell in row)
    lines = [",".join(to_str(row)) for row in mat]
    print("\n".join(lines))


def main(args=sys.argv):
    if (len(args) < 3):
        general_error_and_exit()
    check_inputs(*args[1:4])
    k = int(args[1])
    goal = args[2]
    input_file = args[3]
    points = input_loader(input_file)

    result = [[]]
    if goal == "symnmf":
        # symnmf as described in sec 1 in the pdf
        norm_mat = symnmfmodule.norm(points)
        initial_decomp = init_decomposition_matrix(norm_mat, k)
        result = symnmfmodule.symnmf(initial_decomp, norm_mat, EPSILON)

    elif goal == "sym":
        # sym as described in sec 1.1 in the pdf
        result = symnmfmodule.sym(points)
    elif goal == "ddg":
        # ddg as described in sec 1.2 in the pdf
        result = symnmfmodule.ddg(points)
    elif goal == "norm":
        # norm as described in sec 1.3 in the pdf
        result = symnmfmodule.norm(points)
    print_mat(result)


if __name__ == "__main__":
    main()
