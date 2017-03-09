
// Offered prototypes
int bubblesort_int(int * array, int n_points);
 
// inefficient, but very simple sort algorithm
int bubblesort_int(int * array, int n_points){

    int i, swapped, tmp;

    do{
        swapped = 0;
        for(i = 0; i < (n_points - 1); ++i){
            if(array[i] > array[i+1]){
                tmp        = array[i];
                array[i]   = array[i+1];
                array[i+1] = tmp;
                swapped    = 1;
            }
        }
    }while(swapped == 1);
    
    return 0;
}
