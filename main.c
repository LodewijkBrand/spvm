/* 
*   Matrix Market I/O example program
*
*   Read a real (non-complex) sparse matrix from a Matrix Market (v. 2.0) file.
*   and copies it to stdout.  This porgram does nothing useful, but
*   illustrates common usage of the Matrix Matrix I/O routines.
*   (See http://math.nist.gov/MatrixMarket for details.)
*
*   Usage:  a.out [filename] > output
*
*       
*   NOTES:
*
*   1) Matrix Market files are always 1-based, i.e. the index of the first
*      element of a matrix is (1,1), not (0,0) as in C.  ADJUST THESE
*      OFFSETS ACCORDINGLY offsets accordingly when reading and writing 
*      to files.
*
*   2) ANSI C requires one to use the "l" format modifier when reading
*      double precision floating point numbers in scanf() and
*      its variants.  For example, use "%lf", "%lg", or "%le"
*      when reading doubles, otherwise errors will occur.
*/

#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"
#include "cilk/cilk.h"

double random_in_range(double min, double max)
{
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

void read_in_CSR(int* I, int* J, int M, int N, int nz, int* CSR_count, int* CSR_col)
{
    int row, col, p;
    int count_nz;

    cilk_for (row=0; row<M; row++)
    {
        fprintf(stdout, "row: %d\n", row);
        count_nz = 0;
        cilk_for (col=0; col<N; col++)
        {
            //fprintf(stdout, "col: %d\n", col);
            cilk_for (p=0; p<nz; p++)
            {
                if (I[p]==row && J[p]==col)
                {
                    CSR_col[count_nz + CSR_count[row]] = col;
                    count_nz++;
                }
            }

            CSR_count[row+1] = CSR_count[row] + count_nz;            
        }
    }
}

int main(int argc, char *argv[])
{
    int ret_code;
    MM_typecode matcode;
    FILE *f;
    int M, N, nz;   
    int i, j, *I, *J;
    double *val, *vector, *result;
    int *CSR_count, *CSR_col;

    if (argc < 2)
	{
		fprintf(stderr, "Usage: %s [martix-market-filename]\n", argv[0]);
		exit(1);
	}
    else    
    { 
        if ((f = fopen(argv[1], "r")) == NULL) 
            exit(1);
    }

    if (mm_read_banner(f, &matcode) != 0)
    {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }


    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */

    if ( !(mm_is_matrix(matcode) && mm_is_coordinate(matcode) && mm_is_pattern(matcode) && mm_is_symmetric(matcode)) )
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    /* find out size of sparse matrix .... */

    if ((ret_code = mm_read_mtx_crd_size(f, &M, &N, &nz)) !=0)
        exit(1);


    /* reserve memory for matrices and vector*/

    I = (int *) malloc(nz * sizeof(int));
    J = (int *) malloc(nz * sizeof(int));
    val = (double *) malloc(nz * sizeof(double));
    vector = (double *) malloc(M * sizeof(double));

    for (i=0; i<nz; i++)
    {
        fscanf(f, "%d %d\n", &I[i], &J[i]);
        I[i]--;  /* adjust from 1-based to 0-based */
        J[i]--;
        val[i] = random_in_range(1, 1);
    }

    for (i=0; i<N; i++)
    {
        vector[i] = random_in_range(3, 3);
    }

    CSR_count = (int *) malloc(M * sizeof(int) + 1);
    CSR_count[0] = 0;
    CSR_col = (int *) malloc(nz * sizeof(int));

    fprintf(stdout, "Reading in matrix...\n");
    read_in_CSR(I, J, M, N, nz, CSR_count, CSR_col);

    result = (double *) malloc(N * sizeof(double));

    for (i=0; i<N; i++)
    {
        result[i] = 0.0;
    }

    fprintf(stdout, "Doing matrix multiplication...\n");
    //CSR Matrix (triangular, symmetric) * Dense Vector
    for (i=0; i<N; i++)
    {
        for(j=CSR_count[i]; j<CSR_count[i+1]; j++)
        {
            result[i] = result[i] + val[j] * vector[CSR_col[j]];

            if (i != CSR_col[j]) //Symmetry multiplications
            {
                result[CSR_col[j]] = result[CSR_col[j]] + val[j] * vector[CSR_col[j]];
            }
        }
    }

    if (f !=stdin) fclose(f);

    /************************/
    /* now write out matrix */
    /************************/

    /*mm_write_banner(stdout, matcode);
    mm_write_mtx_crd_size(stdout, M, N, nz);
    for (i=0; i<nz; i++)
        fprintf(stdout, "%d %d\n", I[i], J[i]);

    for (i=0; i<M+1; i++)
        fprintf(stdout, "%d ", CSR_count[i]);

    fprintf(stdout, "\n");

    for (i=0; i<nz; i++)
        fprintf(stdout, "%d ", CSR_col[i]);*/

    fprintf(stdout, "\nRESULT:\n");

    for (i=0; i<N; i++)
        fprintf(stdout, "%20.19g\n", result[i]);

	return 0;
}

