//
// vectorf_mul_example.c
//
// This example demonstrates the interface to the floating-point 
// vector multiplication function.
//

#include <stdio.h>
#include "liquid.h"

#define VLEN 40

int main() {
    unsigned int i;

    // input array 1
    float x[VLEN];
    for (i=0; i<VLEN; i++) {
        x[i] = i+1.0f;
    }

    // input array 2
    float y[VLEN];
    for (i=0; i<VLEN; i++) {
        y[i] = (i%2 == 0) ? 1.0f : -1.0f;
    }

    // result
    float z[VLEN];

    // run vector multiplication
    liquid_vectorf_mul(x, y, VLEN, z);

    printf("z: [");
    for (i=0; i<VLEN-1; i++) {
        printf("%0.1f, ", z[i]);
    }
    printf("%0.1f]\n", z[VLEN-1]);

    return 0;
}


