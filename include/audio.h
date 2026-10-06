#ifndef VETOR_H
#define VETOR_H

// define o tipo AUDIO
typedef struct {
    int samples[128];
} Audio;

// cria a estrutura do tipo Audio e retorna;
void criar_audio();

// libera a estrutura AUDIO
void destruir_audio(Audio *p);

#endif
