#include <stdlib.h>
#include <stdio.h>
#include "fft_audio.h"
#include "fft.h"

static int proxima_pot2(size_t n) {
    int p = 1;
    while ((size_t) p < n) p <<= 1;
    return p;
}

int audio_para_complexo(const Audio *audio, double complex **saida, int *N) {
    if (!audio || !audio->amostras || audio->tamanho == 0 || !saida || !N ) {
        fprintf(stderr, "audio_para_complexo: ERRO, argumento inválido.\n");
        return 1;
    }

    int n = proxima_pot2(audio->tamanho);
    double complex *buf = calloc(n, sizeof(double complex)); // calloc inicializa todos os espaços em 0;
    if (!buf) {
        fprintf(stderr, "audio_para_complexo: ERRO, falha ao alocar buffer.\n");
        return 2;
    }

    for (size_t i = 0; i < audio->tamanho; i++)
        buf[i] = audio->amostras[i];
    *saida = buf;
    *N = n;
    return 0;
}

int complexo_para_audio(const double complex *entrada, Audio *audio) {
    if (!entrada || !audio || !audio->amostras) {
        printf("complexo_para_audio: ERRO, argumentos inválidos.\n");
        return 1;
    }

    for (size_t i = 0; i < audio->tamanho; i++)
        audio->amostras[i] = creal(entrada[i]);
    return 0;
}

int audio_espectro(const Audio *audio, double **mag, int *N) {
    double complex *z, *w;
    if (audio_para_complexo(audio, &z, N)) {
        fprintf(stderr, "audio_espectro: ERRO, falha em converter audio para complexo.\n");
        return 1;
    }

    w = malloc(*N * sizeof(double complex));
    *mag = malloc(*N * sizeof(double));
    if (!w || !mag || fft(z, w, *N)) {
        free(z);
        free(w);
        free(mag);
        fprintf(stderr, "audio_espectro: ERRO, falha ao obter espectro.\n");
        return 2;
    }

    fft_magnitude(w, *mag, *N);
    free(z);
    free(w);

    return 0;
}

double audio_frequencia_dominante(const Audio *audio) {
    double *mag;
    int N;
    if (audio_espectro(audio, &mag, &N)) {
        fprintf(stderr, "audio_frequencia_dominante: ERRO, falha em audio_espectro.\n");
        return -1;
    }
    double f = fft_peak_frequency(mag, N, audio->sample_rate);
    free(mag);
    return f;
}

int audio_passa_baixo(Audio *audio, double cutoff) {
    double complex *x;
    int N;
    if (audio_para_complexo(audio, &x, &N)) {
        fprintf(stderr, "audio_passa_baixo: ERRO, falha em audio_para_complexo.\n");
        return 1;
    }

    int erro = fft(x, x, N) || fft_low_pass(x, N, audio->sample_rate, cutoff)
        || ifft(x, x, N) || complexo_para_audio(x, audio);
    free(x);
    return erro;
}

int audio_passa_alto(Audio *audio, double cutoff) {
    double complex *x;
    int N;
    if (audio_para_complexo(audio, &x, &N)) {
        fprintf(stderr, "audio_passa_alto: ERRO, falha em audio_para_complexo.\n");
        return 1;
    }

    int erro = fft(x, x, N) || fft_high_pass(x, N, audio->sample_rate, cutoff)
        || ifft(x, x, N) || complexo_para_audio(x, audio);
    free(x);
    return erro;
}
