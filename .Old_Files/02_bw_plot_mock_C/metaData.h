#ifndef __METADATA_H_
#define __METADATA_H_
// This above prevents the compiler from including the header twice to the code.

// #define DENSITY 3072

//@brief generates the x set to be distributed among the nodes. 
void fractalXset();

//@brief Per process, generates a onedimentioal matricy to iterate over XYmatrix[XEdgeParS:YEdgeSize].
void genXYmatrix() ;

//@brief based of the algorithm XY[x:y] = XY[x:y]^2 + XY[x:y] what will either go to 0 or inf after itteration x
void testFractal(int itteration) ;

//@brief transforms the XYmatrix values into bool in the XYbool
void XYComToBool() ;

//@brief Hides the necesary typical initialization, establishes runtime dependant constants, and checks for possible errors.
void Judahs_Init(int argc, char *argv[]) ;

//@brief A collectoins of free() funcions to call when exiting the program.
void masterFree() ;


//MPICH Meta values
int comSize;    //size of the MPI_COMM_WORLD

int rank ;      //rank within the MPI_COMM_WORLD


//Borders of the fractal plot
const double    xmin = -2,      //Dimension 0: Border of the fractal plot
                xmax = 0.5 ;    //Dimension 0: Border of the fractal plot
const double    ymin = -1.5,    //Dimension 1: Border of the fractal plot
                ymax = 1.5 ;    //Dimension 1: Border of the fractal plot

const double    dencity = 3072 ;

//The set lengths of x and Y for the fractal plot
int XEdgeSize ;                 //Dimension 0: set length of x for the fractal plot
int YEdgeSize ;                 //Dimension 1: set length of y for the fractal plot


//Needs to be given a value during runtime due to comSize being dependant of runtime.
//XEdgeParS = (int)(XEdgeSize / comSize) ;
int XEdgeParS ;                 //Dimension 0: partial set length of x for the fractal plot


//The variable for the set of X Di-0
double * Xset = NULL;           //The Di-0 set of x for the fractal plot.
double * XsetPar = NULL;        //The Di-0 PARTIAL set of X...divided evenly for each 
complex * XYmatrix = NULL;      //The XY matrix of complex cordinates
unsigned char * XYBool = NULL ; //The XYmatrix but converted into bool, casted as unsigned char.



void fractalXset()
{
     double tmp = (xmax - xmin) / (XEdgeSize-1) ; //
    Xset = (double*) malloc(sizeof(double) * XEdgeSize) ;
    for (int x = 0; x < XEdgeSize; x++)
    {
        Xset[x] = xmin + x * tmp ;
    }
}

void genXYmatrix()
{
    double tmp = (ymax - ymin) / (YEdgeSize - 1) ; //
    double setY[YEdgeSize] ;
    for (int y = 0; y < YEdgeSize; y++)
    {
        setY[y] = ymin + y * tmp ; // It was the differene between * and +
    }
    XYmatrix = (complex *) malloc (sizeof(complex) * XEdgeParS * YEdgeSize); 
    if (XYmatrix == NULL)
    {
        masterFree() ;
        MPI_Abort(MPI_COMM_WORLD, 2) ;
    }
    for (int x = 0; x < XEdgeParS; x++)
    {
        for (int y = 0; y < YEdgeSize; y++)
        { 
            XYmatrix[(x * YEdgeSize) + y] = XsetPar[x] + setY[y] * I ;
            //if (x == 1000 && y == 1000)
                //printf("rank: %d, x: %.04f, y: %.04f\n", rank,XsetPar[x], setY[y]) ;
        }
    }
}

void testFractal(int itteration)
{
    for (int itter = 0; itter < itteration; itter++)
    {
        for (int x = 0; x < XEdgeParS; x++)
        {
            for (int y = 0; y < YEdgeSize; y++)
            {
                if ((isinf(cimag(XYmatrix[(x * YEdgeSize) + y])))||(isinf(creal(XYmatrix[(x * YEdgeSize) + y]))))
                    continue ; // its already overflowed :/
                XYmatrix[(x * YEdgeSize) + y] = cpow(XYmatrix[(x * YEdgeSize) + y], 2) + XYmatrix[(x * YEdgeSize) + y] ;
            }
        }
    }
}

void XYComToBool()
{
    uint8_t white = 255 ;
    uint8_t black = 0 ;
    int testI = 0 ;
    XYBool = (unsigned char *) malloc (sizeof(unsigned char) * XEdgeParS * YEdgeSize) ;
    if (XYBool == NULL)
    {
        masterFree() ;
        MPI_Abort(MPI_COMM_WORLD, 2) ;
    }
    for (int x = 0; x < XEdgeParS; x++ )
    {
        for (int y = 0; y < YEdgeSize; y++ )
        {
            if ((isinf(cimag(XYmatrix[(x * YEdgeSize) + y])))||(isinf(creal(XYmatrix[(x * YEdgeSize) + y]))))
            {
                XYBool[(x * YEdgeSize) + y] = (unsigned char) white ;
                continue ;
            }
            else if (cabs(XYmatrix[(x * YEdgeSize) + y]) <= 2)
            {
                XYBool[(x * YEdgeSize) + y] = (unsigned char) black ;
                testI++;
            }
            else
                XYBool[(x * YEdgeSize) + y] = (unsigned char) white ;
        }
    }
    //printf("rank: %d= 1 count: %d\n", rank, testI) ;
}

void Judahs_Init(int argc, char *argv[])
{
    MPI_Init(&argc, &argv) ;
    MPI_Comm_size(MPI_COMM_WORLD, &comSize) ;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank) ;
    
    XEdgeSize = (int)((xmax - xmin) * dencity) ;
    YEdgeSize = (int)((ymax - ymin) * dencity) ;

    // if (dencity % comSize != 0) //making sure the 
    // {
    //     printf("error: density 00 comSize \n ") ;
    //     masterFree() ;
    //     MPI_Abort(MPI_COMM_WORLD, 1) ;
    //     exit(1) ; //this might not be needed
    // }

    if (XEdgeSize % comSize != 0)
    {
        printf("error: chunkT 00 comSize \n ") ;
        masterFree() ;
        MPI_Abort(MPI_COMM_WORLD, 1) ;
        exit(1) ; //this might not be needed
    }
}

void masterFree()
{
    free(XYBool) ;
    free(XYmatrix) ;
    free(Xset) ;
    free(XsetPar) ;
}

#endif