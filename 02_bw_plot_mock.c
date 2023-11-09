#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <complex.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include <png.h>
#include "mpi.h"

double * onehalf(double xmin, double xmax, int density, int chunkT);
complex ** secondhalf(double * half, int chunkT, double ymin, double ymax, int density) ;
void is_stable(double complex **test, int rangex, int rangey, int itter) ;
unsigned char * bool2D_png(double complex **master, int chunk_partial, int chunkY) ;


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
    
    int chunkX = (int)((xmax - xmin) * density);
    int chunkY = (int)((ymax - ymin) * density) ;
    int chunk_partial = (int)(chunkX / group) ;
    if (chunkX % group != 0)
    {
        printf("error: chunkT 00 group \n ") ;
        exit(1) ;
    }

    double * store = NULL;
    if (rank == 0)
    {
        store = onehalf(xmin, xmax, density, chunkX) ;
    }

    double * storeP = (double *) malloc(sizeof(double) * chunk_partial) ;

    MPI_Scatter(&store[0], chunk_partial, MPI_DOUBLE, storeP, chunk_partial,MPI_DOUBLE,0, MPI_COMM_WORLD) ;
    //printf("rank: %d, firstVal= %02f, lastVal= %02f.\n",rank, storeP[0], storeP[chunk_partial-1]) ;
    int displacement = chunk_partial * rank;
    complex ** master = secondhalf(storeP, chunk_partial,ymin, ymax, density) ;
    is_stable(master, chunk_partial, chunkY, 20) ;
    //printf("rank: %d, master[1][1] = %.4f + %.4fi, master[2][2] = %.4f + %.4fi\n",rank, creal(master[2][1]), cimag(master[2][1]), creal(master[2][2]), cimag(master[2][2])) ;
    unsigned char * master2 = bool2D_png(master, chunk_partial, chunkY) ;

    unsigned char * master3 = NULL ;
    if (rank == 0) {
        master3 = (unsigned char *) malloc (sizeof(unsigned char) * chunkX * chunkY) ;
        if (master3 == NULL)
            MPI_Abort(MPI_COMM_WORLD, 3) ;
    }
    MPI_Gather(&master2[0], chunk_partial * chunkY, MPI_UNSIGNED_CHAR, &master3[0], chunk_partial * chunkY, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD) ;

    if (rank == 0)
    {
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
            chunkY, 
            chunkX, 
            8, 
            PNG_COLOR_TYPE_GRAY, 
            PNG_INTERLACE_NONE, 
            PNG_COMPRESSION_TYPE_DEFAULT, 
            PNG_FILTER_TYPE_DEFAULT
        );
        png_bytepp rows = (png_bytep*)malloc(sizeof(png_bytep) * chunkX) ;
        for (int i = 0; i < chunkX; i++)
        {
            rows[i] = (png_bytep) &master3[i * chunkY] ;
        }
        png_write_info(png, info) ;

        png_write_image(png, rows);
        png_write_end(png, NULL) ;
        png_destroy_write_struct(&png, &info) ;
        fclose(fp) ;
        free(rows) ;

    }

    free(master3) ;
    free(master2) ;
    for (int i = 0; i < chunk_partial; i++)
        free(master[i]);
    free(master);
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

complex ** secondhalf(double * half, int chunkT, double ymin, double ymax, int density)
{
    int tmp = (int)((ymax - ymin) * density) ;
    double tmp2 = (ymax - ymin) / (tmp - 1.0) ;
    double tmp3[tmp];
    for (int i = 0; i < tmp; i++)
    {
        tmp3[i] = ymin + i * tmp2 ;
    }

    complex **master = (complex**) malloc(sizeof(complex*) * chunkT) ;
    for (int i = 0; i < chunkT; i++)
    {
        master[i] = (complex*)malloc(sizeof(complex) * tmp) ;
    }
    for (int i = 0; i < chunkT; i++)
    {
        for (int j = 0; j < tmp; j++)
        {
            master[i][j] = half[i] + tmp3[j] * I ;
        }
    }
    return &master[0] ;
}

void is_stable(double complex **test, int rangex, int rangey, int itter)
{   
    for (int ignore = 0; ignore < itter; ignore++)
    {
        for(int i = 0; i < rangex; i++)
        {
            for (int j = 0; j < rangey; j++)
            {
                if ((isinf(cimag(test[i][j]))) || (isinf(creal(test[i][j])))) continue ;
                test[i][j] = cpow(test[i][j], 2) + test[i][j] ;
            }
        }
    }
}

unsigned char * bool2D_png(double complex **master, int chunk_partial, int chunkY)
{
    uint8_t white = 255 ;
    uint8_t black = 0 ;
    unsigned char * master2 = (unsigned char *) malloc (sizeof(unsigned char) * chunk_partial * chunkY) ;
    if (master2 == NULL)
        MPI_Abort(MPI_COMM_WORLD, 2) ;
    for (int i = 0; i < chunk_partial; i++)
    {
        for (int j = 0; j < chunkY; j++)
        {
            if ((isinf(cimag(master[i][j]))) || (isinf(creal(master[i][j]))))
            {
                master2[(chunk_partial*i)+j] = (unsigned char) white ;
                continue ;
            }
            else if (cabs(master[i][j]) <= 2)
                master2[(chunk_partial*i)+j] = (unsigned char) black ;
            else
                master2[(chunk_partial*i)+j] = (unsigned char) white ;
        }
    }
    return &master2[0] ;

}

//test