#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mpi.h"


//to compile: mpicc.mpich mpi_reduce_base.c -o mrBase
//to run: time mpirun.mpich -np 4 ./mrBase
//to lazy: mpicc.mpich mpi_reduce_base.c -o mrBase ;time mpirun.mpich -np 4 ./mrBase


#define FILE0   "numbers0.txt"
#define FILE1   "numbers1.txt"
#define FILE2   "numbers2.txt"
#define FILE3   "numbers3.txt"

#define METASIZE    300000000


void readNumbers(FILE *file, int *dataArray, int size) ;

int main(int argc, char *args[])
{
    int worldSize, rank ;
    MPI_Init(&argc, &args) ;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize) ;
    if (worldSize != 4) MPI_Abort(MPI_COMM_WORLD, 1) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
    FILE *file ;
    int *numbers = (int *) malloc(sizeof(int) * METASIZE) ;
    int *max;

    switch (rank)
    {
    case 0:
        file = fopen(FILE0, "r") ;
        max = (int *) malloc (sizeof(int) * METASIZE) ;
        break;
    case 1:
        file = fopen(FILE1, "r") ;
        break;
    case 2:
        file = fopen(FILE2, "r") ;
        break;
    case 3:
        file = fopen(FILE3, "r") ;
        break;
    default:
        MPI_Abort(MPI_COMM_WORLD, 2) ;
        break;
    }

    readNumbers(file, numbers, METASIZE) ;
    MPI_Barrier(MPI_COMM_WORLD) ;
    //Typical aproach to collecting a summing
    if (rank == 0)
    {
        int *data1 = (int *) malloc(sizeof(int) * METASIZE) ;
        int *data2 = (int *) malloc(sizeof(int) * METASIZE) ;
        int *data3 = (int *) malloc(sizeof(int) * METASIZE) ;
        int maxTmp ;

        MPI_Recv(data1, METASIZE, MPI_INT, 1, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
        MPI_Recv(data2, METASIZE, MPI_INT, 2, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
        MPI_Recv(data3, METASIZE, MPI_INT, 3, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE) ;
        for (int i = 0; i < METASIZE; i++)
        {
            maxTmp = numbers[i] ;
            if (data1[i] > maxTmp)
                maxTmp = data1[i] ;
            if (data2[i] > maxTmp)
                maxTmp = data2[i] ;
            if (data3[i] > maxTmp)
                maxTmp = data3[i] ;
            max[i] = maxTmp ;
        }
        free(data1) ;
        free(data2) ;
        free(data3) ;
    }
    else
    {
        MPI_Send(&numbers[0], METASIZE, MPI_INT, 0, 10, MPI_COMM_WORLD) ;
    }
    //MPI_Reduce(&numbers[0], &max[0], METASIZE, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD) ;
    //printf("rank: %d, %d %d %d %d, last: %d\n", rank, numbers[0], numbers[1], numbers[2], numbers[3], numbers[METASIZE-1]) ;
    //if (rank == 0) printf("MAX: rank: %d, %d %d %d %d, last: %d\n", rank, max[0], max[1], max[2], max[3], max[METASIZE-1]) ;
    fclose(file) ;
    free(max) ;
    free(numbers) ;
    MPI_Finalize() ;
}

void readNumbers(FILE *file, int *dataArray, int size)
{
    for (int i = 0; i < size; i++)
    {
        fscanf(file, "%d, ", &dataArray[i]) ;
    }
}
