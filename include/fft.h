#ifndef FFT_H
#define FFT_H

#include <complex.h>

// transformada de Fourier Radix-2 DIT, n deve ser uma potência de 2;
int fft(double complex *sinal, double complex *saida, int n);

int ifft(double complex *sinal, double complex *saida, int n);

// análise espectral
void fft_magnitude(double complex *spectrum, double *magnitude, int n);

void fft_power(double complex *spectrum, double *power, int n);

// retorna maior frequência;
double fft_peak_frequency(const double *magnitude, int n, double sample_rate);

// retorna frequência média das amostras em sucesso, -1 caso contrário
double fft_bin_frequency(int bin, int n, double sample_rate);

// Janelas
void fft_window_hann(double *samples, int n);
void fft_window_hamming(double *samples, int n);

// Filtros
int fft_low_pass(double complex *spectrum, int n, double sample_rate, double cutoff);

int fft_high_pass(double complex *spectrum, int n, double sample_rate, double cutoff);

#endif
