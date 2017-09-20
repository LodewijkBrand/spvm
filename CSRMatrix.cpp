#include <iostream>
#include <omp.h>

#include "CSRMatrix.h"

using namespace std;

double* CSRMatrix::multiply(double* vector){
    double* result = new double[num_rows];
    
    for (int i = 0; i < num_rows; i++){
        for (int j = count_nonzero[i]; j < count_nonzero[i+1]; j++){
            result[i] = result[i] + nonzero_elements[j] * vector[column_index[j]];

            //Symmetry multiplications
            if (i != column_index[j]){
                result[column_index[j]] = result[column_index[j]] + nonzero_elements[j] * vector[column_index[j]];
            }
        }
    }

    return result;
}

double* CSRMatrix::parallel_multiply(double* vector){
    double* result = new double[num_rows]; 

    #pragma omp parallel
    {
        double* partial = new double[num_rows];

        int tid = omp_get_thread_num();
        int num = omp_get_num_threads();

        int start = (int) ((double) tid / (double) num * (double) num_rows);
        int end = (int) ((double) (tid + 1) / (double) num * (double) num_rows);

        for (int i = start; i < end; i++){
            for (int j = count_nonzero[i]; j < count_nonzero[i+1]; j++){
                partial[i] = partial[i] + nonzero_elements[j] * vector[column_index[j]];

                //Symmetry multiplications
                if (i != column_index[j]){
                    partial[column_index[j]] = partial[column_index[j]] + nonzero_elements[j] * vector[column_index[j]];
                }
            }
        }

        for (int i = 0; i < num_rows; i++)
            #pragma omp atomic
            result[i] += partial[i]; // Note: Only one thread at a time should access result[i] at a time
    }

    return result;
}
     

int CSRMatrix::getNumRows(){
    return num_rows;
}

int CSRMatrix::getNumCols(){
    return num_cols;
}

void CSRMatrix::print() {
    cout << "Non-zero Elements: [";
    for (int i = 0; i < num_nz; i++){
        cout << nonzero_elements[i] << " ";
    }
    cout << "]" << endl;
    
    cout << "Non-zero counter: [";
    for (int i = 0; i < num_rows + 1; i++){
        cout << count_nonzero[i] << " ";
    }
    cout << "]" << endl;

    cout << "Column indices:   [";
    for (int i = 0; i < num_nz; i++){
        cout << column_index[i] << " ";
    }
    cout << "]" << endl;
}
