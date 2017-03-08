
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_matrix.h>

int factorial(int n);
int InvertMatrix(gsl_matrix *Matrix, gsl_matrix *InvMatrix, int dimension);

int main(int argc, char **argv){

    int i, j;
    unsigned int derivative_order    = 1;
    unsigned int sample_point_number = 0;
    double * constraint_vector;
    double * result_vector;
    gsl_matrix *     coefficient_matrix;
    gsl_matrix * inv_coefficient_matrix;

// get derivative_order and sample_point_number
    derivative_order = atoi(argv[1]);
    sample_point_number = argc - 2;
    if(derivative_order >= sample_point_number){
        fprintf(stderr, "Please enter a derivative order that is less than the number of points in your stencil\n");
        fprintf(stderr, "derivative order:\t%u\n", derivative_order);
        fprintf(stderr, "sample point number:\t%u\n", sample_point_number);
        exit(1);
    }

// allocate memory for coefficient_matrix and its inverse counterpart
    coefficient_matrix     = gsl_matrix_alloc(sample_point_number, sample_point_number);
    inv_coefficient_matrix = gsl_matrix_alloc(sample_point_number, sample_point_number);
    if(coefficient_matrix == NULL || inv_coefficient_matrix == NULL){
        fprintf(stderr, "Error in memory allocation for coefficient_matrix\n");
        exit(1);
    }

// allocate memory for constraint and result vector
    constraint_vector = malloc(sample_point_number * sizeof(double));
    result_vector     = calloc(sample_point_number, sizeof(double));
    if(constraint_vector == NULL || result_vector == NULL){
        fprintf(stderr, "Error in memory allocation for constraint_vector\n");
        exit(1);
    }

// fill the constraint_vector with data points
    for(i = 0; i < sample_point_number; ++i){
        if(i == derivative_order){
            constraint_vector[i] = (double) factorial(i);
        }
        else{
            constraint_vector[i] = 0;
        }
    }

// fill the coefficient matrix with data points
    for(i = 0; i < sample_point_number; ++i){
        for(j = 0; j < sample_point_number; ++j){
            gsl_matrix_set(coefficient_matrix, i, j, pow(atof(argv[j+2]),i));
        }
    }

// invert coefficient matrix
    InvertMatrix(coefficient_matrix, inv_coefficient_matrix, sample_point_number);

// solve A x b = c
    for(i = 0; i < sample_point_number; ++i){
        for(j = 0;j < sample_point_number; ++j){
            result_vector[i] += gsl_matrix_get(inv_coefficient_matrix, i, j) * constraint_vector[j];
        }
    }

// print output
    if(result_vector[0] >= 0){
        for(i=0; i<sample_point_number; ++i){
            printf("% 14.6lf f(x%+2.0lfh) / (%.0lfh**%u)\n", result_vector[i]/result_vector[0], gsl_matrix_get(coefficient_matrix, 1, i), 1.0 / result_vector[0], derivative_order);
        }
    }else{
        for(i=0; i<sample_point_number; ++i){
            printf("% 14.6lf f(x%+2.0lfh) / (%.0lf*h**%u)\n", -result_vector[i]/result_vector[0], gsl_matrix_get(coefficient_matrix, 1, i), -1.0 / result_vector[0], derivative_order);
        }
    }

    return 0;
}

int factorial(int n){

    int i;
    int factorial = 1;

    for(i = 2; i <= n; ++i){
        factorial *= i;
    }

    return factorial;
}
