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

int converterraw_floats(Audio *audio){
    
    char *caminho = audio->arquivo_raw;
    int *n = &audio->tamanho;

    FILE *f = fopen(caminho, "rb");

    if (!f){
        printf("Erro ao tentar abrir o arquivo!\n");
        return 1;
    }

    // Faz o ponteiro que percorre o arquivo em sua leitura apontar para o final do arquivo (último byte)
    fseek(f,0,SEEK_END);

    // Diz a quantidade de bytes dos elementos do arquivo até onde o ponteiro de leitura aponta
    long bytes = ftell(f); 
    rewind(f);

    audio->amostras = malloc(bytes);
    
    if(audio->amostras == NULL){
        printf("Erro na alocação do vetor de audio!\n");
        return 1;
    }

    // Contamos quantos
    // fread salva os conteúdos de um arquivo dentro de um array e retorna a quantidade de elementos salvos. 
    *n = fread(audio->amostras, sizeof(float), bytes/sizeof(float), f);
    fclose(f);
    return 0;    
}

// Salva os floats de um vetor representando um áudio em um aruivo .raw  
int converterfloat_raw(Audio *audio){
    char *caminho = audio->arquivo_raw;
    float *amostras = audio->amostras;
    int n = audio->tamanho;

    FILE *f = fopen(caminho,"wb");
    if (!f){
        printf("Erro na escrita do arquivo!\n");
        return 1;
    }
    int escritos = fwrite(amostras, sizeof(float), n,f);
    fclose(f);

    if(escritos == n){
        return 0;
    }else{
        printf("Nem todos as amostras foram salvas corretamente!\n");
        return 1;
    }
}

int converteraw_wav(Audio *audio){
    char cmd[1024];

    char *entrada = audio->arquivo_raw;
    char *saida = audio->arquivo_wav;

    snprintf(cmd, sizeof(cmd), "ffmpeg -f f32le -ar 16000 -ac 1 -i \"%s\" \"%s\"", entrada, saida);
    return system(cmd);
}

int converterwav_mp3(Audio *audio){
    char cmd[1024];

    char *entrada = audio->arquivo_wav;
    char *saida = audio->arquivo_mp3;

    snprintf(cmd, sizeof(cmd), "ffmpeg  -i \"%s\" \"%s\"", entrada, saida);
    return system(cmd);
}

int arquivo_existe(char *caminho){
    FILE *file = fopen(caminho, "r");
    if(file == NULL) return 1;
    return 0;
}

// Por padrão o .mp3 deve existir, se não existir é só converter ele usando um convertewav ou converteraw
Audio *criar_audio(char *arquivo_mp3, char *arquivo_wav, char *arquivo_raw){

    Audio *opt_novo_audio = malloc(sizeof (Audio));

    opt_novo_audio->arquivo_mp3 = arquivo_mp3;
    opt_novo_audio->arquivo_wav = arquivo_wav;
    opt_novo_audio->arquivo_raw = arquivo_raw;

    int existe[3] = {0,0,0}; 
    
    if(arquivo_existe(arquivo_mp3) == 0) existe[0] = 1;
    else existe[0] = 0;

    if(arquivo_existe(arquivo_wav) == 0) existe[1] = 1;
    else existe[1] = 0;

    if(arquivo_existe(arquivo_raw) == 0) existe[2] = 1;
    else existe[2] = 0;

    if(existe[0] == 0 && existe[1] == 0){
        printf("Sem arquivo .mp3 ou .wav\n");
        return NULL;
    }

    if(existe[0] == 0){
        converterwav_mp3(opt_novo_audio);
    }

    if(existe[1] == 0){
        convertermp3_wav(opt_novo_audio);
    }

    
    if(existe[1] == 0){
        converterwav_raw(opt_novo_audio);
    }

    converterraw_floats(opt_novo_audio);
    
    return opt_novo_audio;
}
// mp3 : 4
// wav : 2 
// raw : 1


int destroi_audio(Audio *audio,int opcao){
    if (opcao == 4 || opcao == 5 || opcao == 6 || opcao == 7 ){
        if(remove(audio->arquivo_mp3) == -1){
            printf("Não foi possível remover o .mp3\n");
            return 1;
        }
    }

    if (opcao == 1 || opcao == 5 || opcao == 3 || opcao == 7){
        if(remove(audio->arquivo_raw) == -1){
            printf("Não foi possível remover o .raw\n");
            return 1;
        }
    }

    if (opcao == 2 || opcao == 6 || opcao == 3 || opcao == 7){
        if(!remove(audio->arquivo_wav) == -1){
            printf("Não foi possível remover o .wav\n");
            return 1;
        }
    }
    
    free(audio->amostras);
    free(audio);

    return 0;

}

int atualizar_audio(Audio *audio){
    if(audio->amostras == NULL){
        printf("Representação do áudio em forma de array inválida\n");
        return 1;
    }
    if (converterfloat_raw(audio) != 0){
        printf("Erro na conversão para .raw");
    }
    
    if(converteraw_wav(audio) != 0){
        printf("Erro na conversão para .wav");
    }
    
    if(converterwav_mp3(audio) != 0){
        printf("Erro na conversão para .mp3\n");
        return 1;
    }
    
    return 0;
}