
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_matrix.h>
#include <stdarg.h>

int factorial(int n);
int InvertMatrix(gsl_matrix *Matrix, gsl_matrix *InvMatrix, int dimension);
int FiniteDifferenceCoefficients(unsigned int deriv_order, unsigned int sample_point_number, double * sample_point_location, double * result_vector);

int main(int argc, char **argv){

    int i;
    unsigned int derivative_order    = atof(argv[1]);
    unsigned int sample_point_number = argc-2;
    double * sample_point_location;
    double * result_vector;

    sample_point_location = calloc(sample_point_number, sizeof(double));
    result_vector         = calloc(sample_point_number, sizeof(double));
    if(sample_point_location == NULL || result_vector == NULL){
        fprintf(stderr, "Error in memory allocation for sample point location\n");
        exit(1);
    }

    for(i = 0; i < sample_point_number; ++i){
        sample_point_location[i] = atof(argv[i+2]);
    }

    FiniteDifferenceCoefficients(derivative_order, sample_point_number, sample_point_location, result_vector);

// print output
//    for(i = 0; i < sample_point_number; ++i){
//        fprintf(stdout, "% 14.6lf f(x%+2.0lfh) / (%.0lfh**%u)\n", result_vector[i]/result_vector[0], sample_point_location[i], 1.0 / result_vector[0], derivative_order);
//    }

// print raw output
    for(i = 0; i < sample_point_number; ++i){
        fprintf(stdout, "% 14.6lf f(x%+2.0lfh) / (h**%u)\n", result_vector[i], sample_point_location[i], derivative_order);
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

int FiniteDifferenceCoefficients(unsigned int deriv_order, unsigned int sample_point_number, double * sample_point_location, double * result_vector){

    int i, j;
    double     * constraint_vector;
    gsl_matrix *     coefficient_matrix;
    gsl_matrix * inv_coefficient_matrix;

// check if enough points for desired derivative are given
    if(deriv_order >= sample_point_number){
        fprintf(stderr, "Please enter a derivative order that is less than the number of points in your stencil\n");
        fprintf(stderr, "derivative order:\t%u\n", deriv_order);
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

// allocate memory for constraint and set the entries to zero
    constraint_vector = calloc(sample_point_number, sizeof(double));
    if(constraint_vector == NULL){
        fprintf(stderr, "Error in memory allocation for constraint_vector\n");
        exit(1);
    }

// fill the constraint_vector with data points
//  all derivatives of f(x) have to cancel out, except of the desired one
//  f^(n)(x) / n! = 1  -=>  f^(n)(x) = n!
    constraint_vector[deriv_order] = (double) factorial(deriv_order);

// fill the coefficient matrix with data points
//  first  line c1^0 c2^0 c3^0 c4^0 ... cn^0
//  second line c1^1 c2^1 c3^1 c4^1 ... cn^1
//      ...     ...  ...  ...  ...  ... ... 
//  n^th   line c1^n c2^n c3^n c4^n ... cn^n
    for(i = 0; i < sample_point_number; ++i){
        for(j = 0; j < sample_point_number; ++j){
            gsl_matrix_set(coefficient_matrix, i, j, pow(sample_point_location[j],i));
        }
    }

// solve coefficient_matrix * b = constraint_vector
//  inversion and matrix multiplication
    InvertMatrix(coefficient_matrix, inv_coefficient_matrix, sample_point_number);
    for(i = 0; i < sample_point_number; ++i){
        for(j = 0;j < sample_point_number; ++j){
            result_vector[i] += gsl_matrix_get(inv_coefficient_matrix, i, j) * constraint_vector[j];
        }
    }



    return 0;
}
