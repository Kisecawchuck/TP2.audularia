#ifndef VETOR_H
#define VETOR_H

// define o tipo AUDIO
typedef struct {
    float *amostras;
    char *arquivo_mp3;
    char *arquivo_wav;
    char *arquivo_raw;
    int tamanho;
    
} Audio;

// cria a estrutura do tipo Audio e retorna;
// void criar_audio();

// libera a estrutura AUDIO
// void destruir_audio(Audio *p);

int convertermp3_wav(Audio *p);

int converterwav_raw(Audio *p);

int converterraw_floats(Audio *p);

int converterfloat_raw(Audio *p);


#endif
