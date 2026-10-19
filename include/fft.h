#ifndef FFT_H
#define FFT_H

#include <complex.h>

#define PI 3.14159265358979323846

// Transformada de Fourier
int fft(double complex *sinal, double complex *saida, int n);

int ifft(double complex *sinal, double complex *saida, int n);

// Análise espectral
void fft_magnitude(double complex *spectrum, double *magnitude, int n);

void fft_power(double complex *spectrum, double *power, int n);

double fft_peak_frequency(const double *magnitude, int n, double sample_rate);

double fft_bin_frequency(int bin, int n, double sample_rate);

// Janelas
void fft_window_hann(double *samples, int n);
void fft_window_hamming(double *samples, int n);

// Filtros
int fft_low_pass(double complex *spectrum, int n, double sample_rate, double cutoff);

int fft_high_pass(double complex *spectrum, int n, double sample_rate, double cutoff);

#endif
