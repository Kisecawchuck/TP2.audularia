#include<stdio.h>
#include<stdlib.h>
#include"../include/audio.h"

int convertermp3_wav(Audio *audio){
    char cmd[1024];

    char *entrada = audio->arquivo_mp3;
    char *saida = audio->arquivo_wav;

    snprintf(cmd, sizeof(cmd), "ffmpeg -i \"%s\" \"%s\"", entrada, saida);
    return system(cmd);
    
}
// Converte um arquivo .wav em .raw usando o ffmpeg
int converterwav_raw(Audio *audio){
    char cmd[1024];
    char *entrada = audio->arquivo_wav;
    char *saida = audio->arquivo_raw;

    snprintf(cmd, sizeof(cmd),
        "ffmpeg -y -loglevel error -i \"%s\" -f f32le -ch_layout mono -ar \"%f\" \"%s\"", entrada, 16000.0, saida);
    return system(cmd);
}

// Lê cada float do arquivo .raw e adiciona em um vetor
// Para isso, calculamos o número de bytes do binário resultante do arquivo .raw
// e alocamos um vetor de floats na memória com base nesse número de bytes

float *converterraw_floats(Audio *audio){
    char *caminho = audio->arquivo_raw;
    int *n = &audio->tamanho;

    FILE *f = fopen(caminho, "rb");

    if (!f){
        printf("Erro ao tentar abrir o arquivo!\n");
        return NULL;
    }

    // Faz o ponteiro que percorre o arquivo em sua leitura apontar para o final do arquivo (último byte)
    fseek(f,0,SEEK_END);

    // Diz a quantidade de bytes dos elementos do arquivo até onde o ponteiro de leitura aponta
    long bytes = ftell(f); 
    rewind(f);

    audio->amostras = malloc(bytes);
    
    if(audio->amostras == NULL){
        printf("Erro na alocação do vetor de audio!\n");
        return NULL;
    }

    // Contamos quantos
    // fread salva os conteúdos de um arquivo dentro de um array e retorna a quantidade de elementos salvos. 
    *n = fread(audio->amostras, sizeof(float), bytes/sizeof(float), f);
    fclose(f);
    return audio->amostras;

}

// Salva os floats de um vetor representando um áudio em um aruivo .raw
// Uso: salvar_raw(../pasta/arquivo.raw, audio, tamanho_audio)  
int salvar_raw(const char *caminho, const float *audio, int n){
    FILE *f = fopen(caminho,"wb");
    if (!f){
        printf("Erro na escrita do arquivo!\n");
        return -1;
    }
    int escritos = fwrite(audio, sizeof(float), n,f);
    fclose(f);

    if(escritos == n){
        return 0;
    }else{
        return -1;
    }
}