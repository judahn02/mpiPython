#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//to compile: gcc generate4.c -o generateR
//to run: ./generateR
//to lazy: gcc generate4.c -o generateR; ./generateR

#define METASIZE    300000000

void main(int argc, char *argv[])
{
    FILE *file ;
    char stringcache[100] ;
    char fileName[100] ;
    srand(time(NULL)) ;
    //This program is to make 4 text files of a million random numbers each.
    for (int i = 0; i < 4; i++)
    {
        strcpy(fileName, "numbers") ;
        sprintf(stringcache, "%d", i) ;
        strcat(fileName, stringcache) ;
        strcat(fileName, ".txt") ;
        file = fopen(fileName, "w") ;
        // fprintf(file, "This is a test") ;
        for (int j = 0; j < METASIZE; j++)
        {
            sprintf(stringcache, "%d, ", rand()) ;
            fprintf(file, stringcache) ;
        }
        fclose(file) ;
    }
}