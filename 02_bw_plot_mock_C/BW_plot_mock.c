#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <complex.h>
#include <math.h> 
    // Don't forget to add -lm
#include <float.h>
#include <stdbool.h>
#include <png.h>
    // Don't forget to add -lpng
#include <mpi.h>

#include "metaData.h"

//to compile: mpicc.mpich  BW_plot_mock.c -o test -lm -lpng
//to run: mpirun.mpich -np 8 ./test

int main(int argc, char *argv[])
{

    Judahs_Init(argc, argv) ;

    XEdgeParS = (int)(XEdgeSize / comSize) ;
    
    if (rank == 0)
    {
        //Establishes the set of X that will be distributed among the nodes.
        fractalXset() ;
    }

    //Establishes the partial set of X that each process will work on.
    XsetPar = (double *) malloc(sizeof(double) * XEdgeParS) ;

    //Distribute the Xset among all notes.
    MPI_Scatter(&Xset[0], XEdgeParS, MPI_DOUBLE, &XsetPar[0], XEdgeParS,MPI_DOUBLE,0, MPI_COMM_WORLD) ;

    genXYmatrix() ;

    testFractal(20) ;

    XYComToBool() ;

    unsigned char * XYMatrixFull = NULL ;
    if (rank == 0) {
        XYMatrixFull = (unsigned char *) malloc (sizeof(unsigned char) * XEdgeSize * YEdgeSize) ;
        if (XYMatrixFull == NULL)
            MPI_Abort(MPI_COMM_WORLD, 3) ;
    }

    MPI_Gather(&XYBool[0], XEdgeParS * YEdgeSize, MPI_UNSIGNED_CHAR, &XYMatrixFull[0], XEdgeParS * YEdgeSize, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD) ;

    if (rank == 0)
    {
        // uint8_t white = 255 ;
        // FILE *test = fopen("test.txt", "wb") ;
        // long testI = 0 ;
        // for (int x = 0; x < XEdgeSize; x++)
        // {
        //     for (int y = 0; y < YEdgeSize; y++ )
        //     {
        //         if(XYMatrixFull[x * YEdgeSize + y] == (unsigned char) white)
        //             fprintf(test, "%d ", 0) ;
        //         else
        //         {
        //             fprintf(test, "%d ", 1) ;
        //             testI++ ;
        //         }
        //     }
        //     fprintf(test, "\n") ;
        // }
        // fclose(test) ;

        FILE *fp = fopen("TestRes.png", "wb") ; 
        if (!fp) {MPI_Abort(MPI_COMM_WORLD, 4); return 1 ;}

        png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL) ;
        if (!png) {fclose(fp);  MPI_Abort(MPI_COMM_WORLD, 4); return 1;}

        png_infop info = png_create_info_struct(png) ;
        if (!info) {png_destroy_info_struct(png, NULL);  MPI_Abort(MPI_COMM_WORLD, 4); fclose(fp);  return 1;}

        if (setjmp(png_jmpbuf(png))) abort();

        png_init_io(png, fp);
        png_set_IHDR(
            png, 
            info, 
            YEdgeSize, 
            XEdgeSize, 
            8, 
            PNG_COLOR_TYPE_GRAY, 
            PNG_INTERLACE_NONE, 
            PNG_COMPRESSION_TYPE_DEFAULT, 
            PNG_FILTER_TYPE_DEFAULT
        );
        png_bytepp rows = (png_bytep*)malloc(sizeof(png_bytep) * XEdgeSize) ;
        for (int i = 0; i < XEdgeSize; i++)
        {
            rows[i] = (png_bytep) &(XYMatrixFull[i * YEdgeSize]) ;
        }
        png_write_info(png, info) ;

        png_write_image(png, rows);
        png_write_end(png, NULL) ;
        png_destroy_write_struct(&png, &info) ;
        fclose(fp) ;
        free(rows) ;

    }

    free(XYMatrixFull) ;
    masterFree() ;
    MPI_Finalize() ;
    return 0 ;
}

