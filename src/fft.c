#include <stdio.h>
#include <math.h>
#include "../include/fft.h"

#define PI 3.14159265358979323846

// função auxiliar para Radix-2 DIT verificar potência de 2;
int is_power_of_two(int x) {
    return (x & (x - 1)) == 0;
}

// transformada de Fourier Radix-2 DIT;
int fft_core(double complex *entrada, double complex *saida, int N, int sinal) {
    if (N < 1 || !is_power_of_two(N)) {
        fprintf(stderr, "fft_core: ERRO, tamanho do entrada inválido\n");
        return 1;
    }

    if (!entrada || !saida) {
        fprintf(stderr, "fft_core: ERRO, argumentos inválidos\n");
        return 2;
    }

    if (entrada != saida)
        for (int i = 0; i < N; i++) saida[i] = entrada[i];

    // permutação bit-reversal;
    for (int i = 1, j = 0; i < N; i++) {
        int bit = N >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) {
            double complex t = saida[i];
            saida[i] = saida[j];
            saida[j] = t;
        }
    }

    // borboletas;
    for (int len = 2; len <= N; len <<= 1) {
        double ang = sinal * 2.0 * PI / len;
        for (int i = 0; i < N; i += len) {
            for (int j = 0; j < len / 2; j++) {
                double complex z = cexp(I * ang * j);
                double complex w = saida[i + j];
                double complex v = saida[i + j + len / 2] * z;
                saida[i + j] = w + v;
                saida[i + j + len / 2] = w - v;
            }

        }
    }

    return 0;
}

int fft(double complex *entrada, double complex *saida, int N) {
    return fft_core(entrada, saida, N, -1);
}

int ifft(double complex *entrada, double complex *saida, int N) {
    int r = fft_core(entrada, saida, N, 1);
    if (r)
        return r;
    // normalizamos as entradas por N;
    for (int i = 0; i < N; i++)
        saida[i] /= N;
    return 0;
}

// análise espectral;
void fft_magnitude(double complex *spectrum, double *magnitude, int n) {
    for (int k = 0; k < n; k++) {
        magnitude[k] = cabs(spectrum[k]);
    }
}

void fft_power(double complex *spectrum, double *power, int n) {
    for (int k = 0; k < n; k++) {
        double re = creal(spectrum[k]);
        double im = cimag(spectrum[k]);
        power[k] = re * re + im * im;
    }
}

double fft_peak_frequency(const double *magnitude, int n, double sample_rate) {
    int melhor = 1;
    for (int k = 2; k < n / 2; k++)
        if (magnitude[k] > magnitude[melhor]) melhor = k;
    return fft_bin_frequency(melhor, n, sample_rate);
}

double fft_bin_frequency(int bin, int n, double sample_rate) {
    if (n == 0 || bin >= n || sample_rate <= 0.0) {
        fprintf(stderr, "fft_bin_frequency: ERRO, argumentos inválidos.\n");
        return -1;
    }

    if (bin <= n / 2)
        return (double) bin * sample_rate / n;

    return ((double) bin - n) * sample_rate / n;
}

// janelamento;
void fft_window_hann(double *samples, int n) {
    if (!samples || n < 2)
        return;


    for (int k = 0; k < n; k++) {
        samples[k] *= 0.5 * (1.0 - cos(2.0 * PI * k / (n - 1)));
    }
}

void fft_window_hamming(double *samples, int n) {
    if (!samples || n < 2)
        return;

    for (int k = 0; k < n; k++) {
        samples[k] *= 0.54 - 0.46 * cos(2.0 * PI * k / (n - 1));
    }
}

// filtros;
int fft_low_pass(double complex *spectrum, int n, double sample_rate, double cutoff) {
    if (!spectrum || n < 1 || sample_rate <= 0.0 || cutoff < 0.0 || cutoff > sample_rate / 2.0) {
        fprintf(stderr, "fft_low_pass: ERRO, argumentos inválidos.\n");
        return 1;
    }

    for (int k = 0; k < n; k++) {
        double freq = (k <= n / 2) 
            ? (double) k * sample_rate / n
            : (double) (k - n) * sample_rate / n;

        if (fabs(freq) > cutoff)
            spectrum[k] = 0.0;
    }

    return 0;
}

int fft_high_pass(double complex *spectrum, int n, double sample_rate, double cutoff) {
    if (!spectrum || n < 1 || sample_rate <= 0.0 || cutoff < 0.0 || cutoff > sample_rate / 2.0) {
        fprintf(stderr, "fft_high_pass: ERRO, argumentos inválidos.\n");
        return 1;
    }

    for (int k = 0; k < n; k++) {
        double freq = (k <= n / 2)
            ? (double) k * sample_rate / n
            : (double) (k - n) * sample_rate / n;
        if (fabs(freq) < cutoff)
            spectrum[k] = 0.0;
    }

    return 0;
}
