#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <complex.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include <png.h>

//typedef uint8_t bool; 

void is_stable(double complex**, int, int, int);
bool** bool2D(double complex **test, int rangex, int rangey) ;

int main() {

    //!!!!! complex matrix
    double  xmin = -2,
            xmax = 0.5,
            ymin = -1.5,
            ymax = 1.5,
            pixel_dencity = 512;
    int     rangex = (int)((xmax - xmin) * pixel_dencity),
            rangey = (int)((ymax - ymin) * pixel_dencity) ;
    double  row[rangex],
            col[rangey] ;
    for (int i = 0; i < rangex; i++)
        row[i] = xmin + i * (xmax-xmin)/(rangex-1) ;
    for (int j = 0; j < rangey; j++)
        col[j] = ymin + j * (ymax-ymin)/(rangey-1) ;
    
    double complex **c = (double complex **) malloc(rangex * sizeof(complex double *)) ;

    for (int i = 0; i < rangex; i++)
    {
        c[i] = (double complex *) malloc(sizeof(complex double) * rangey) ;
    }

    for (int i = 0; i < rangex; i++) {
        for (int j = 0; j < rangey; j++) {
            c[i][j] = row[i] + col[j] * I;  // Example: set each element to a complex value
        }
    }
    //!!!!!

    is_stable(c, rangex, rangey, 20);
    bool **map = bool2D(c, rangex, rangey) ;
    printf("pass 5000\n") ;
    printf()

    FILE *fp = fopen("TestRes.png", "wb") ;
    if (!fp) return 1 ;

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL) ;
    if (!png) {fclose(fp);  return 1;}

    png_infop info = png_create_info_struct(png) ;
    if (!info) {png_destroy_info_struct(png, NULL);  fclose(fp);  return 1;}

    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp) ;

    png_set_compression_level(png, 9) ;

    // int width = rangey ;
    // int height = rangex ;
    png_set_IHDR(
            png, info, rangey, rangex, 1, 
            PNG_COLOR_TYPE_GRAY, 
            PNG_INTERLACE_NONE, 
            PNG_COMPRESSION_TYPE_DEFAULT, 
            PNG_FILTER_TYPE_DEFAULT);
    
    // png_bytep row_pointers[rangex] ;
    // png_bytepp rows = (png_bytep*)malloc(sizeof(png_bytep*)*rangex) ;

    // for (int i = 0; i < rangex; i++)
    //     rows[i] = (png_bytep) &map[i][0] ;
    
    png_bytepp rows = (png_bytepp)map;
    
    png_write_image(png, rows);
    png_write_end(png,info) ;
    // png_set_rows(png, info, row_pointers) ;
    // png_write_png(png, info, PNG_TRANSFORM_IDENTITY, NULL) ;

    png_destroy_write_struct(&png, &info) ;
    fclose(fp) ;


    for (int i = 0; i < rangex; i++) {
        free(c[i]);
        free(map[i]);
        // free(rows[i]);
    }
    free(c);
    free(map);
    free(rows);

    return 0;
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

bool** bool2D(double complex **test, int rangex, int rangey)
{
    bool **c = (bool **) malloc(rangex * sizeof(bool *)) ;
    for (int i = 0; i < rangex; i++)
    {
        c[i] = (bool *) malloc(sizeof(bool) * rangey) ;
    }

    for(int i = 0; i < rangex; i++)
    {
        for (int j = 0; j < rangey; j++)
        {
            if ((isinf(cimag(test[i][j]))) || (isinf(creal(test[i][j])))) {c[i][j] = 0 ; continue;}
            // test[i][j] = cpow(test[i][j], 2) + test[i][j] ;
            else if (cabs(test[i][j]) <= 2) 
                c[i][j] = 1 ;
            else
                c[i][j] = 0 ;
        }
    }
    return c;
}

void printGrid(bool **test, int rows, int cols) 
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", (int)test[i][j]);
        }
        printf("\n");
    }
}