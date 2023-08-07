"""Python interface"""
import sys

import numpy as np
from numpy.linalg import norm


def input_loader(filename):
    """load input file as list of strings
    and turns lines into list of points"""
    try:
        with open(filename, "r") as f:
            lines = f.readlines()
    except:
        print("An Error Has Occurred")
        sys.exit()

    return [np.array((float(num) for num in line.split(","))) for line in lines]


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
