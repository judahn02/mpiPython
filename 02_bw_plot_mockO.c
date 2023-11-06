#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <complex.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include <png.h>

//typedef uint8_t bool; 

//to compile: gcc 02_bw_plot_mockO.c -o C_seq -lm -lpng
//torun: ./C_seq

void is_stable(double complex**, int, int, int);
void bool2D_png(double complex **test, int rangex, int rangey, png_bytepp *rows) ;

int main() {

    double  xmin = -2,
            xmax = 0.5,
            ymin = -1.5,
            ymax = 1.5,
            pixel_dencity = 3072;
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
            c[i][j] = row[i] + col[j] * I;  
        }
    }

    // row and col can be freed after this.

    is_stable(c, rangex, rangey, 20);


    FILE *fp = fopen("TestResO.png", "wb") ; 
    if (!fp) return 1 ;

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL) ;
    if (!png) {fclose(fp);  return 1;}

    png_infop info = png_create_info_struct(png) ;
    if (!info) {png_destroy_info_struct(png, NULL);  fclose(fp);  return 1;}

    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp) ;

    png_set_IHDR(
            png, 
            info, 
            rangey, 
            rangex, 
            8, 
            PNG_COLOR_TYPE_GRAY, 
            PNG_INTERLACE_NONE, 
            PNG_COMPRESSION_TYPE_DEFAULT, 
            PNG_FILTER_TYPE_DEFAULT
            );
    
    png_bytepp rows = (png_bytep*)malloc(sizeof(png_bytep) * rangex) ;
    for (int i = 0; i < rangex; i++)
    {
        rows[i] = (png_byte*)malloc(sizeof(png_byte) * rangey) ;
    }
    bool2D_png(c, rangex, rangey, &rows) ;
    png_write_info(png, info) ;
    

    png_write_image(png, rows);
    png_write_end(png, NULL) ;
    png_destroy_write_struct(&png, &info) ;
    fclose(fp) ;


    for (int i = 0; i < rangex; i++) {
        free(c[i]);
        free(rows[i]);
    }
    free(c);
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

void bool2D_png(double complex **test, int rangex, int rangey, png_bytepp *rows)
{
    uint8_t white = 255;
    uint8_t black = 0; 
    for(int i = 0; i < rangex; i++)
    {
        for (int j = 0; j < rangey; j++)
        {
            if ((isinf(cimag(test[i][j]))) || (isinf(creal(test[i][j])))) 
            {
                (*rows)[i][j] = (png_byte) white ; 
                continue;
            }
            else if (cabs(test[i][j]) <= 2) 
                (*rows)[i][j] = (png_byte) black ;
            else
                (*rows)[i][j] = (png_byte) white ;
            //(*rows)[i][j] = (png_byte) 255 ;
        }
    }
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