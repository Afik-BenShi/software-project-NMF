CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
# compiled binaries directory
OUT = .out

# all and out tasks do not make actual files
.PHONY: all out

# calls out before symnmf_app
all: out symnmf_app

# creates the output directory
out:
	mkdir -p $(OUT)

# creates the app
symnmf_app: $(OUT)/symnmf.o $(OUT)/symnmf_impl.o $(OUT)/utils_impl.o symnmf.h utils.h
	gcc -o symnmf $(OUT)/symnmf.o $(OUT)/symnmf_impl.o $(OUT)/utils_impl.o $(CFLAGS) -lm

$(OUT)/symnmf.o: symnmf.c symnmf.h utils.h
	gcc -c symnmf.c -o $(OUT)/symnmf.o $(CFLAGS)

$(OUT)/symnmf_impl.o: ./impls/symnmf_impl.c utils.h
	gcc -c ./impls/symnmf_impl.c -o $(OUT)/symnmf_impl.o $(CFLAGS)

$(OUT)/utils_impl.o: ./impls/utils_impl.c
	gcc -c ./impls/utils_impl.c -o $(OUT)/utils_impl.o $(CFLAGS)

