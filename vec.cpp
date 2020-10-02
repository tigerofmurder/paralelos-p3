#include "mpi.h" 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define DIM 1024

int main(int argc, char *argv[])
{

    int i, j, n; 
    int nlocal;
    double *fb;
    double a[DIM*DIM], b[DIM], x[DIM];
    int npes, myrank; 
    MPI_Status status; 

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &npes);


    fb = (double*)malloc(n*sizeof(double));

    nlocal = n/npes; 
    
    
    MPI_Allgather(b, nlocal, MPI_DOUBLE, fb, nlocal, MPI_DOUBLE, MPI_COMM_WORLD); 

    for (i=0; i<nlocal; i++) { 
        x[i] = 0.0;
        for (j=0; j<n; j++)
            x[i] += a[i*n+j]*fb[j];
    }
    free(fb);

    MPI_Finalize();
}
