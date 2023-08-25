'''Python interface'''
import sys
import numpy as np
import math

MAX_ITER = 300
EPSILON = math.e - 4


def calculate_average(matrix, rows, cols):
    '''calculate_average calculates the average of a matrix.'''
    sum = 0.0
    for i in range(rows):
        for j in range(cols):
            sum = sum + matrix[i][j]

    return sum / (rows * cols)


def multiply_matrices(matrix1, matrix2):
    '''multiply_matrices calculates matrix multiplication.'''
    n1, m1 = len(matrix1), len(matrix1[0])  # dimensions of matrix1 (n x m)
    n2, m2 = len(matrix2), len(matrix2[0])  # dimensions of matrix2 (n x k)
    
    if m1 != n2:
        print("An Error Accured!")
        exit(1)
    
    result = [[0] * m2 for _ in range(n1)]
    
    for i in range(n1):
        for j in range(m2):
            for k in range(m1):
                result[i][j] += matrix1[i][k] * matrix2[k][j]
    
    return result


def subtract_matrices(matrix1, matrix2):
    '''subtract_matrices calculates matrix substraction.'''
    if len(matrix1) != len(matrix2) or len(matrix1[0]) != len(matrix2[0]):
        print("An Error Accured!")
        exit(1)

    result = [[0] * len(matrix1[0]) for _ in range(len(matrix1))]
    
    for i in range(len(matrix1)):
        for j in range(len(matrix1[0])):
            result[i][j] = matrix1[i][j] - matrix2[i][j]
    
    return result

def divide_matrices(matrix1, matrix2):
    '''divide_matrices calculates matrix division.'''
    if len(matrix1) != len(matrix2) or len(matrix1[0]) != len(matrix2[0]):
        print("An Error Accured!")
        exit(1)
    
    result = []
    for i in range(len(matrix1)):
        row = []
        for j in range(len(matrix1[0])):
            if matrix2[i][j] == 0:
                print("An Error Accured!")
                exit(1)
            row.append(matrix1[i][j] / matrix2[i][j])
        result.append(row)
    
    return result

def transpose_matrix(matrix):
    '''transpose_matrix calculates the transposed matrix of a given matrix.'''
    rows, cols = len(matrix), len(matrix[0])
    transposed = [[0] * rows for _ in range(cols)]
    
    for i in range(rows):
        for j in range(cols):
            transposed[j][i] = matrix[i][j]
    
    return transposed

def init_decomposition_matrix(norm_matrix,k):
    ''' init_decomposition_matrix randomly initialize decomposition matrix with values from the interval [0, 2 * sqrt(m/k)], where m is the average of
        all entries of norm_matrix.'''
    np.random.seed(0)

    n = len(norm_matrix)
    min_val = 0
    max_val = 2 * (((calculate_average(norm_matrix)) / (k)) ** 0.5)

    initial_matrix = np.random.uniform(min_val, max_val, size=(n, k))
    return initial_matrix

def frob_norm(matrix):
    ''' frob_norm calaculates the frobenius norm of a given matrix'''
    return


def update_decomposition_matrix(initial_decomp_matrix, norm_matrix):
    ''' update_decomposition_matrix updates given matrix iterativly.'''
    decomp_matrix = initial_decomp_matrix
    new_decomp_matrix = initial_decomp_matrix
    beta = 0.5
    i = 0
    
    while (i < MAX_ITER):
    
        for i in range(len(decomp_matrix)):
            for j in range(len(decomp_matrix[i])):
                numerator = multiply_matrices(norm_matrix, decomp_matrix)
                denominator = multiply_matrices(multiply_matrices(decomp_matrix, transpose_matrix(decomp_matrix)), decomp_matrix)
                new_decomp_matrix[i][j] = decomp_matrix[i][j] * (1 - beta + beta*(numerator[i][j] / denominator[i][j]))

        if (frob_norm(subtract_matrices(new_decomp_matrix, decomp_matrix)) ** 2 < EPSILON):
            break

        decomp_matrix = new_decomp_matrix
    
    return decomp_matrix
        

def main(args=sys.argv):
    k = args[1]
    goal = args[2]
    input_file = args[3]

    if goal == "symnmf":
        # symnmf as described in sec 1 in the pdf
        pass
    elif goal == "sym":
        # sym as described in sec 1.1 in the pdf
        pass
    elif goal == "ddg":
        # ddg as described in sec 1.2 in the pdf
        pass
    elif goal == "norm":
        # norm as described in sec 1.3 in the pdf
        pass


if __name__ == "__main__":
    main()
