#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <complex.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include <png.h>

double * onehalf(double xmin, double xmax, int density, int chunkT);

int main(int argc, char *argv[])
{
    int group, rank;
    MPI_Init(&argc, &argv) ;
    MPI_Comm_size(MPI_COMM_WORLD, &group) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;

    int density = 3072;
            
    double  xmin = -2,
            xmax = 0.5,
            ymin = -1.5,
            ymax = 1.5;

    if (density % group != 0)
    {
        printf("error: density 00 group \n ") ;
        exit(1) ;
    }
    
    int chunkT = (int)((0.5 - (-2)) * density);
    int chunk_partial = (int)(chunkT / group) ;
    if (chunkT % group != 0)
    {
        printf("error: chunkT 00 group \n ") ;
        exit(1) ;
    }

    double * store = NULL;
    if (rank == 0)
    {
        store = onehalf(xmin, xmax, density, chunkT) ;
    }

    double * storeP = (double *) malloc(sizeof(double) * chunk_partial) ;

    MPI_Scatter(&store[0], chunk_partial, MPI_DOUBLE, storeP, chunk_partial,MPI_DOUBLE,0, MPI_COMM_WORLD) ;
    printf("rank: %d, firstVal= %02f, lastVal= %02f.\n",rank, storeP[0], storeP[chunk_partial-1]) ;


    free(store) ;
    free(storeP) ;
    MPI_Finalize() ;
    return 0 ;
}

double * onehalf(double xmin, double xmax, int density, int chunkT)
{
    double tmp = (xmax - xmin) / (double)(chunkT-1) ;
    double *tmp2 = (double*) malloc(sizeof(double) * chunkT) ;
    for (int i = 0; i < chunkT; i++)
    {
        tmp2[i] = xmin + i * tmp ;
    }
    return &tmp2[0] ;
}