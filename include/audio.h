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

// Essas funções usam a bilbioteca ffmpeg do bash e só estão aqui 
// para vconversão dos arquivos com que iremos trabalhar, não as implementamos: 

int convertermp3_wav(Audio *p);

int converterwav_raw(Audio *p);

int converteraw_wav(Audio *p);

int converterwav_mp3(Audio *p);

//

// Somente implementamos essas:

int converterraw_floats(Audio *p);
int converterfloat_raw(Audio *p);


//




#endif
