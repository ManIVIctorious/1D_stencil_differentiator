CC = gcc -g -Wall #-Werror
CFLAGS = 

EXE = stencil
DEP = stencil_generator.c InvertMatrix.c
OBJ = stencil_generator.o InvertMatrix.o
LIB = `pkg-config --cflags --libs gsl`

all: $(EXE)

$(EXE): $(DEP)
	$(CC) $(CFLAGS) $(DEP) $(LIB) -o $@

clean:
	rm -f $(OBJ) $(EXE)
