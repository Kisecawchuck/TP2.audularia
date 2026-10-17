#include <assert.h>
#include <complex.h>
#include <stdio.h>
#include "../include/fft.h"

#define EPSILON 1e-9

int main() {
    double complex sinal1[] = {1, 2, 3, 4};
    double complex sinal2[4];
    fft(sinal1, sinal2, 4);
    double complex sinal3[4];
    ifft(sinal2, sinal3, 4);

    for (int i = 0; i < 4; i++) {
        assert(cabs(sinal1[i] - sinal3[i]) < EPSILON);
    }
}
