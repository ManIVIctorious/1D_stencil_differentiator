CC = gcc
CFLAGS = -g -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -Werror -Wno-sign-compare -mtune=native

EXE = bin/stencil
OBJ = main.o InvertMatrix.o BubbleSort.o Factorial.o FiniteDifferenceCoefficients.o
LIB = `pkg-config --cflags --libs gsl`

all: $(EXE)
# define rule to build object files out of C-source files
%.o : %.c
	$(CC) $(CFLAGS) $(LIB) -c $<

# link all objects to create the executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(LIB) $(OBJ) -o $@

clean:
	rm -f $(OBJ) $(EXE)
