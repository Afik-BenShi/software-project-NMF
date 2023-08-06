import sys


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
