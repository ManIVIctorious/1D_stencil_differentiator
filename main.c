
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

// Dependencies
int bubblesort_int(int * array, int n_points);
int FiniteDifferenceCoefficients(unsigned int derivative, unsigned int point_number, int* point_location, double* result_vector);


int main(int argc, char **argv){

    int i, j;
    unsigned int derivative_order    = atoi(argv[1]);
    unsigned int sample_point_number = argc-2;
    int * sample_point_location;
    double * result_vector;

// allocate memory for sample point and result vector
    sample_point_location = calloc(sample_point_number, sizeof(double));
    result_vector         = calloc(sample_point_number, sizeof(double));
    if(sample_point_location == NULL || result_vector == NULL){
        fprintf(stderr, "Error in memory allocation for sample point location\n");
        exit(1);
    }

// fill sample point location vector
    for(i = 0; i < sample_point_number; ++i){
        sample_point_location[i] = atof(argv[i+2]);
    }

// sort sample point location vector and remove duplicate entries
    bubblesort_int(sample_point_location, sample_point_number);
    for(i = 0; i < (sample_point_number - 1); ++i){
        if(sample_point_location[i] == sample_point_location[i+1]){
            for(j = i+1; j < sample_point_number; ++j){
                sample_point_location[j] = sample_point_location[j+1];
            }
            sample_point_number = sample_point_number - 1;
        }
    }

// calculate finite difference coefficients
    FiniteDifferenceCoefficients(derivative_order, sample_point_number, sample_point_location, result_vector);


// output: print finite difference coefficients
    for(i = 0; i < sample_point_number; ++i){
        fprintf(stdout, "#% 14.6lf f(x%+0dh) / (h**%u)\n", result_vector[i], sample_point_location[i], derivative_order);
    }


// generate sinus start function
    int n_points = 1000;
    double * in_function  = calloc(n_points, sizeof(double));
    double * out_function = calloc(n_points, sizeof(double));

    double pi = 3.141592653589793238462643383279;
    double dx=2*pi/n_points;
    for(i = 0; i < n_points; ++i){
        in_function[i] = sin(dx*i);
    }

// calculation with full stencil and all points of in_function
//  leads to errors in edge cases (first and last points of out_function)
    for(i = 0; i < n_points; ++i){
        for(j = 0; j < sample_point_number; ++j){
            if(i + sample_point_location[j] >= 0 && i + sample_point_location[j] < n_points){
                out_function[i] += result_vector[j] * in_function[i+sample_point_location[j]];
            }
        }
        for(j = 0; j < derivative_order; ++j){
            out_function[i] = out_function[i] / dx;
        }
    }





// output only the points where a full stencil could be applied
    for(i = sample_point_location[0]; i < n_points; ++i){
        if(i + sample_point_location[0] < 0 || i + sample_point_location[sample_point_number-1] >= (n_points-1)){
            continue;
        }
        printf("%lf\t%lf\t%lf\n", i*dx, in_function[i], out_function[i]);
    }



    return 0;
}
