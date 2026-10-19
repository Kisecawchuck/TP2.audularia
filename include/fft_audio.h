#ifndef FFT_AUDIO_H
#define FFT_AUDIO_H

#include <complex.h>
#include "audio.h"

/*
 * converte amostras (double) para complexos (complex double), preenche
 * com zeros até a próxima potência de 2;
 * aloca *saida (responsabilidade do usuário liberar);
 * retorna 0 em caso de sucesso sucesso, numero positivo caso contrário;
*/
int audio_para_complexo(const Audio *audio, double complex *saida, int *N);

// converte de volta, cortando até audio->tamanho;
int complexo_para_audio(const double complex *entrada, Audio *audio);

// calcula o espectro de magnitude (aloga mag);
int audio_espectro(const Audio *audio, double *mag, int *N);

// frequência dominante em Hz, retorna -1 em erro;
double audio_frequencia_dominante(const Audio *audio);

// filtros aplicados diretamente em audio;
int audio_passa_baixo(Audio *audio, double cutoff);
int audio_passa_alto(Audio *audio, double cutoff);

#endif
