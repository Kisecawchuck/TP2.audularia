#include "../include/fft.h"

// Função auxiliar para Radix-2 DIT
int is_power_of_two(int x) {
    return (x & (x - 1)) == 0;
}

// Transformada de Fourier
int fft(double complex *sinal, double complex *saida, int N) {
    if (N < 1 || !is_power_of_two(N)) {
        printf("ERRO: tamanho do sinal inválido\n");
        return 1;
    }

    if (!sinal) {
        printf("ERRO: sinal inválido\n");
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

double fft_peak_frequency(const double *magnitude, int n) {
    double x = -1;
    for (int k = 0; k < n; k++)
        if (magnitude[k] > x) x = magnitude[k];
    return x;
}

double fft_bin_frequency(int bin, int n, double sample_rate) {
    if (n == 0 || bin >= n || sample_rate <= 0.0)
        return -1;

    if (bin <= n / 2)
        return (double) bin * sample_rate / n;

    return ((double) bin - n) * sample_rate / n;
}

// Janelas
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

// Filtros
int fft_low_pass(double complex *spectrum, int n, double sample_rate, double cutoff) {
    if (!spectrum || n < 1 || sample_rate < 0.0 || cutoff < 0.0 || cutoff > sample_rate / 2.0)
        return 1;

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
    if (!spectrum || n < 1 || sample_rate < 0.0 || cutoff < 0.0 || cutoff > sample_rate / 2.0)
        return 1;

    for (int k = 0; k < n; k++) {
        double freq = (k <= n / 2)
            ? (double) k * sample_rate / n
            : (double) (k - n) * sample_rate / n;
        if (fabs(freq) < cutoff)
            spectrum[k] = 0.0;
    }

    return 0;
}
