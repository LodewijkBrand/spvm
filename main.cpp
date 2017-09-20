#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "CSRMatrix.h"
#include "mmio.h"

double random_in_range(double min, double max){
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

CSRMatrix readMatrixMarket(char* filename){
    FILE *f;
    MM_typecode matcode;
    int ret_code;
    int rows, cols, nz;

    if ((f = fopen(filename, "r")) == NULL){
        std::cerr << "Error reading in matrix market file: " << filename << std::endl;
        exit(1);
    }

    if (mm_read_banner(f, &matcode) != 0){
        std::cerr << "Could not process matrix market banner." << std::endl;
        exit(1);
    }

    // For this application only support matrix market files that have the following properties:
    // matrix coordinate pattern symmetric
    if ( !(mm_is_matrix(matcode) && mm_is_coordinate(matcode) && mm_is_pattern(matcode) && mm_is_symmetric(matcode)) ){
        std::cerr << "Sorry, this application does not support Matrix Market type: " << mm_typecode_to_str(matcode) << std::endl;
        exit(1);
    }

    // Find out size of sparse matrix...
    if ((ret_code = mm_read_mtx_crd_size(f, &rows, &cols, &nz)) != 0){
        std::cerr << "Error reading in the size of the sparse matrix" << std::endl;
        exit(1);
    }

    // Note: MatrixMarket file format for our large matrices looks like this
    // row col   22  1
    // row col   111 1
    // row col   234 1
    // ...
    // In order to read efficiently into CSR we will swap row and col
    // This works because the Matrix Market is "symmetric"
    int* I = new int[nz]; // Hold row coordinates
    int* J = new int[nz]; // Hold col coordinates
    double* val = new double[nz]; // Hold nonzero values
    for (int i = 0; i < nz; i++){
        fscanf(f, "%d %d\n", &J[i], &I[i]); // Note how &J and &I are switched
        I[i]--; // adjust from 1-based to 0 based
        J[i]--;
        val[i] = random_in_range(1, 1);
    }

    // Initialize and fill CSRMatrix variables
    int* count_nonzero = new int[rows + 1];
    count_nonzero[0] = 0;
    int* column_index = new int[nz];
    int current_row = 0;
    for (int i = 0; i < nz; i++){
        column_index[i] = J[i];
        if (I[i] != current_row){
            current_row++;
            count_nonzero[current_row + 1] = count_nonzero[current_row];
        }

        if(I[i] == current_row){
            count_nonzero[current_row + 1]++;
        }
    }

    // Fill in extra entries in count_nonzero
    for (int i = current_row + 1; i < rows; i++)
        count_nonzero[i + 1] = count_nonzero[i];

    CSRMatrix m = CSRMatrix::Builder()
        .setNumNonzero(nz)
        .setNumRows(rows)
        .setNumCols(cols)
        .setNonzeroElements(val)
        .setCountNonzero(count_nonzero)
        .setColumnIndex(column_index)
        .build();

    return m;
}

int main(int argc, char** argv){
    if (argc < 2){
        std::cerr << "Usage: " << argv[0] << " [matrix-market-filename]" << std::endl;
        exit(1);
    }

    // Reading in data to CSRMatrix
    CSRMatrix m = readMatrixMarket(argv[1]);

    // Fill in dense vector
    double* vect = new double[m.getNumCols()];
    for (int i = 0; i < m.getNumCols(); i++){
        vect[i] = random_in_range(1, 1);
    }

    timeval time; 

    // Timing serial SMPV Execution
    gettimeofday(&time, NULL);
    double start_serial = time.tv_usec + 1000000.0 * time.tv_sec;
    double* result = m.multiply(vect);
    gettimeofday(&time, NULL);
    double end_serial = time.tv_usec + 1000000.0 * time.tv_sec;

    // Timing parallel SMPV Execution
    gettimeofday(&time, NULL);
    double start_parallel = time.tv_usec + 1000000.0 * time.tv_sec;
    double* result_parallel = m.parallel_multiply(vect);
    gettimeofday(&time, NULL);
    double end_parallel = time.tv_usec + 1000000.0 * time.tv_sec;

    // Check to see if Serial and Parallel SPMV agree. If not exit(1)
    for (int i = 0; i < m.getNumRows(); i++){
        if (result[i] != result_parallel[i]){
            std::cout << "Parallel and Serial SPMV are Different!" << std::endl;
            exit(1);
        }
    }

    // Report results
    std::cout << "Time for serial multiply  : " << (end_serial - start_serial) << " microseconds." << std::endl;
    std::cout << "Time for paralell multiply: " << (end_parallel - start_parallel) << " microseconds." << std::endl;
}
