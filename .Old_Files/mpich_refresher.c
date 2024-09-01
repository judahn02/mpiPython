#include "mpi.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int group, rank ;
    MPI_Init(&argc, &argv) ;
    MPI_Comm_size(MPI_COMM_WORLD, &group) ;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank) ;

    if (group != 4)
    {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    double *data ;
    if (rank == 0)
    {
        data = (double *)malloc(sizeof(double) * 4) ;
        data[0] = 10.0 ;
        data[1] = 20.0 ;
        data[2] = 30.0 ;
        data[3] = 40.0 ;
    }
    double *val = (double*)malloc(sizeof(double)) ;
    MPI_Scatter(data, 1, MPI_DOUBLE, val, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD) ;
    printf("rank: %d, val= %02f\n", rank, *val) ;


    free(data) ;
    MPI_Finalize() ;
    return 0;
}