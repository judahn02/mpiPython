#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <float.h>

int main() {
    double complex z1 = (DBL_MAX) + (DBL_MAX * I) ;  // Example complex number with positive infinity
    double complex z2 = (-1.0 / 0.0) + (DBL_MAX * I); // Example complex number with negative infinity
    z1 = z1 + z1 ;

    // Check for overflow in the real part
    if (isinf(creal(z1))) {
        printf("Real part of z1 has overflowed to infinity\n");
        // Handle overflow here
    }

    // Check for overflow in the imaginary part
    if (isinf(cimag(z1))) {
        printf("Imaginary part of z1 has overflowed to infinity\n");
        // Handle overflow here
    }

    // Check for overflow in the real part of z2
    if (isinf(creal(z2))) {
        printf("Real part of z2 has overflowed to negative infinity\n");
        // Handle overflow here
    }

    // Check for overflow in the imaginary part of z2
    if (isinf(cimag(z2))) {
        printf("Imaginary part of z2 has overflowed to negative infinity\n");
        // Handle overflow here
    }

    // Declare complex numbers
     z1 = 3.0 + 4.0 * I;
     z2 = 1.0 - 2.0 * I;

    // Perform operations on complex numbers
    double complex sum = z1 + z2;
    double complex product = z1 * z2;

    // Access real and imaginary parts
    double realPart = creal(sum);
    double imagPart = cimag(sum);

    // Print the results
    printf("z1 = %.2f + %.2fi\n", creal(z1), cimag(z1));
    printf("z2 = %.2f + %.2fi\n", creal(z2), cimag(z2));
    printf("Sum = %.2f + %.2fi\n", realPart, imagPart);
    printf("Product = %.2f + %.2fi\n", creal(product), cimag(product));

    return 0;
}
