#include <iostream>

#include "CSRMatrix.h"

using namespace std;

double* CSRMatrix::multiply(double* vector){
    double* result = new double[num_rows];
    
    for (int i = 0; i < num_rows; i++){
        for (int j = count_nonzero[i]; j < count_nonzero[i+1]; j++){
            result[i] = result[i] + nonzero_elements[j] * vector[column_index[j]];

            //Symmetry multiplications
            //if (i != column_index[j]){
            //    result[column_index[j]] = result[column_index[j]] + nonzero_elements[j] * vector[column_index[j]];
            //}
        }
    }

    return result;
}

double* CSRMatrix::parallel_multiply(double* vector){
    double* result = new double[num_rows];

    #pragma omp parallel for 
    for (int i = 0; i < num_rows; i++){
        for (int j = count_nonzero[i]; j < count_nonzero[i+1]; j++){
            result[i] = result[i] + nonzero_elements[j] * vector[column_index[j]];

            //Symmetry multiplications
            //if (i != column_index[j]){
            //    result[column_index[j]] = result[column_index[j]] + nonzero_elements[j] * vector[column_index[j]];
            //}
        }
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
    for (int i = 0; i < num_cols; i++){
        cout << column_index[i] << " ";
    }
    cout << "]" << endl;
}
