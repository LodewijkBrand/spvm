#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "CSRMatrix.h"
#include "mmio.h"

using namespace std;

void readMatrixMarket(char* filename){
    FILE *f;

    if ((f = fopen(filename, "r")) == NULL){
        cerr << "Error reading in matrix market file: " << filename << endl;
        exit(1);
    }
}

int main(int argc, char** argv){
    CSRMatrix m = CSRMatrix::Builder().build();

    m.print();

    if (argc < 2){
        cerr << "Usage: " << argv[0] << " [matrix-market-filename]" << endl;
        exit(1);
    }



}
