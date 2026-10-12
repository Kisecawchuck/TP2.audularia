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

// cria a estrutura do tipo Audio e retorna;
Audio *criar_audio(char *arquivo_mp3, char *arquivo_wav, char *arquivo_raw);

// Apaga os arquivos criados pelas funçõs implementadas acima e libera o espaço 
// alocado pela estrutura audio
int destroi_audio(Audio *audio,int opcao);


// Atualiza os arquivos correspondentes a audio com base no estado atual do vetor
// das amostras 
int atualizar_audio(Audio *audio);

//




#endif
