#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    int   numtasks, taskid ;
    MPI_Init(&argc, &argv) ;
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks) ;
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid) ;

    int buf[2] ;
    buf[0] = taskid ;
    buf[1] = taskid * 10 ;

    printf("[%d %d]\n", buf[0], buf[1]) ;
    int *test = (int*) malloc(sizeof(int) * numtasks * 2) ;
    int total = numtasks ;

    MPI_Gather(buf, 2, MPI_INT, test, 2, MPI_INT, 0, MPI_COMM_WORLD) ;

    if (taskid == 0)
    {
        for (int i = 0; i < total * 2; i++)
            printf("%d ", test[i]);
        printf("\n") ;
    }

    free(test) ;
}