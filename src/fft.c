#include "../include/fft.h"

// Transformada de Fourier
int fft(double complex *sinal, double complex *saida, int N) {
    if (N < 1) {
        printf("ERRO: tamanho do sinal inválido\n");
        return 1;
    }

    if (!sinal) {
        printf("ERRO: sinal inválida\n");
        return 2;
    }

    if (!saida) {
        printf("ERRO: saida inválida\n");
        return 2;
    }

    for (int k = 0; k < N; k++) {
        double complex sum = 0.0;
        for (int n = 0; n < N; n++) {
            double angle = -2.0 * PI * k * n / N;
            sum += sinal[n] * cexp(I * angle);
        }
        saida[k] = sum;
    }

    return 0;
}

int ifft(double complex *sinal, double complex *saida, int N) {
    if (N < 1) {
        printf("ERRO: tamanho do sinal inválido\n");
        return 1;
    }

    if (!sinal) {
        printf("ERRO: sinal inválida\n");
        return 2;
    }

    if (!saida) {
        printf("ERRO: saida inválida\n");
        return 2;
    }

    for (int n = 0; n < N; n++) {
        double complex sum = 0.0;
        for (int k = 0; k < N; k++) {
            double angle = 2.0 * PI * k * n / N;
            sum += sinal[k] * cexp(I * angle);
        }
        saida[n] = sum / N;
    }

    return 0;
}

// Análise espectral
void fft_magnitude(double complex *spectrum, double *magnitude, int n);

void fft_power(double complex *spectrum, double *power, int n);

size_t fft_peak_frequency(const double *magnitude, int n);

double fft_bin_frequency(int bin, int n);

// Janelas
void fft_window_hann(double *samples, int n);

void fft_window_hamming(double *samples, int n);

// Filtros
int fft_low_pass(double complex *spectrum, int n, double sample_rate, double cutoff);

int fft_high_pass(double complex *spectrum, int n, double sample_rate, double cutoff);
