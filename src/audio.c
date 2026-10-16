#include<stdio.h>
#include<stdlib.h>
#include"../include/audio.h"

int arquivo_existe(char *caminho){
    FILE *file = fopen(caminho, "r");
    if(file == NULL) return 1;
    fclose(file);
    return 0;
}
// flags do ffmpeg
// -i indica o arquivo de entrada
// -ar indica a taxa de amostragem
// -ch_layout mono define o arquivo de áudio para mono
// f32le formato de cada amostras de áudio: float de 32 bits
// -f define o formato que o ffmpeg usará para ler ou escrever o arquivo. Também define
// um formato padrão para o ffmpeg caso o formato não tenha sido fornecido ou seja inválido
// nos arquivos de entrada e saída do comando
// -loglevel error mostra apenas as saídas de erro do comando

int convertermp3_wav(Audio *audio){

    if(audio == NULL || audio->arquivo_mp3 == NULL){
        printf("convertermp3_wav: erro, áudio ou arquivo .mp3 inválidos\n");
        return 1;
    }

    char *entrada = audio->arquivo_mp3;
    if(arquivo_existe(entrada) == 1){
        printf("convertermp3_wav: erro, arquivo .mp3 não existe\n");
        return 1;
    }

    char cmd[1024];
    char *saida = audio->arquivo_wav;

    snprintf(cmd, sizeof(cmd), "ffmpeg -loglevel error -i \"%s\" \"%s\"", entrada, saida);
    return system(cmd);
}
// Converte um arquivo .wav em .raw usando o ffmpeg
int converterwav_raw(Audio *audio){
    
    if(audio == NULL || audio->arquivo_wav == NULL){
        printf("converterwav_raw: erro, áudio ou arquivo .wav inválidos\n");
        return 1;
    }

    char *entrada = audio->arquivo_wav;
    
    if(arquivo_existe(entrada) == 1){
        printf("converterwav_raw: erro, arquivo .wav não existe\n");
        return 1;
    }

    char cmd[1024];
    char *saida = audio->arquivo_raw;

    snprintf(cmd, sizeof(cmd),
        "ffmpeg -loglevel error -i \"%s\" -f f32le -ch_layout mono -ar \"%f\" \"%s\"", entrada, 16000.0, saida);
    return system(cmd);
}

// Lê cada float do arquivo .raw e adiciona em um vetor
// Para isso, calculamos o número de bytes do binário resultante do arquivo .raw
// e alocamos um vetor de floats na memória com base nesse número de bytes

int converterraw_floats(Audio *audio){
    
    if(audio == NULL || audio->arquivo_raw == NULL){
        printf("converterraw_floats: erro, áudio ou arquivo .raw inválidos\n");
        return 1;
    }

    char *caminho = audio->arquivo_raw;

    if(arquivo_existe(caminho) == 1){
            printf("converterraw_floats: erro, arquivo .raw não existe\n");
            return 1;
        }

    long n;

    FILE *f = fopen(caminho, "rb");

    if (!f){
        printf("converterraw_floats: erro, ao tentar abrir o arquivo!\n");
        return 1;
    }

    // Faz o ponteiro que percorre o arquivo em sua leitura apontar para o final do arquivo (último byte)
    if (fseek(f,0,SEEK_END) != 0){
        printf("converterraw_floats: erro ao tentar percorrer o arquivo\n");
        fclose(f);
        return 1;
    }

    // Diz a quantidade de bytes dos elementos do arquivo até onde o ponteiro de leitura aponta
    // ftell retorna -1 caso não tenha conseguido ler os bytes corretamente

    long bytes = ftell(f); 
    if(bytes < 0){
        printf("converterraw_floats: erro ao tentar ler o número de bytes do .raw\n");
        fclose(f);
        return 1;
    }

    rewind(f);

    n = bytes/sizeof(float);
    audio->amostras = malloc(bytes);
    
    if(audio->amostras == NULL){
        printf("converterraw_floats: erro na alocação do vetor de audio!\n");
        return 1;
    }

    // Contamos quantos
    // fread salva os conteúdos de um arquivo dentro de um array e retorna a quantidade de elementos salvos. 
    long lidas = fread(audio->amostras, sizeof(float), n, f);
    fclose(f);

    if(n != lidas){
        printf("converterraw_floats: leitura incompleta do .raw\n");
        free(audio->amostras);
        return 1;
    }
    
    audio->tamanho = n;

    return 0;    
}

// Salva os floats de um vetor representando um áudio em um arquivo .raw  
int converterfloat_raw(Audio *audio){

    if(audio == NULL || audio->amostras == NULL){
        printf("converterfloat_raw: erro, áudio ou amostras do áudio inválidos\n");
        return 1;
    }

    char *caminho = audio->arquivo_raw;
    float *amostras = audio->amostras;
    long n = audio->tamanho;

    FILE *f = fopen(caminho,"wb");
    if (!f){
        printf("converterfloat_raw: erro, na escrita do arquivo!\n");
        return 1;
    }
    
    long escritos = fwrite(amostras, sizeof(float), n,f);

    if(fclose(f) != 0 || escritos != n){
        printf("converterfloat_raw: erro, nem todos as amostras foram salvas corretamente!\n");
        return 1;
    }
    return 0;
}

int converteraw_wav(Audio *audio){
    
    if(audio == NULL || audio->arquivo_raw == NULL){
        printf("converterraw_wav: erro, áudio ou arquivo .raw inválidos\n");
        return 1;
    }

    char *entrada = audio->arquivo_raw;
    
    if(arquivo_existe(entrada) == 1){
        printf("converterraw_wav: erro, arquivo .raw não existe\n");
        return 1;
    }

    char cmd[1024];
    char *saida = audio->arquivo_wav;

    snprintf(cmd, sizeof(cmd), "ffmpeg -loglevel error -f f32le -ar \"%f\" -ch_layout mono -i \"%s\" \"%s\"", 16000.0, entrada, saida);
    return system(cmd);
}

int converterwav_mp3(Audio *audio){

    if(audio == NULL || audio->arquivo_wav == NULL){
        printf("converterwav_mp3: erro, áudio ou arquivo .wav inválidos\n");
        return 1;
    }

    char *entrada = audio->arquivo_wav;
    if(arquivo_existe(entrada) == 1){
        printf("converterwav_mp3: erro, arquivo .wav não existe\n");
        return 1;
    }

    char cmd[1024];
    char *saida = audio->arquivo_mp3;

    snprintf(cmd, sizeof(cmd), "ffmpeg -loglevel error -i \"%s\" \"%s\"", entrada, saida);
    return system(cmd);
}

// Por padrão o .mp3 deve existir, se não existir é só converter ele usando um convertewav ou converteraw

Audio *criar_audio(char *arquivo_mp3, char *arquivo_wav, char *arquivo_raw){

    Audio *opt_novo_audio = malloc(sizeof (Audio));
    if(opt_novo_audio == NULL){
        printf("criar_audio: erro, na alocação do objeto do tipo Audio\n");
        return NULL;
    }

    opt_novo_audio->arquivo_mp3 = arquivo_mp3;
    opt_novo_audio->arquivo_wav = arquivo_wav;
    opt_novo_audio->arquivo_raw = arquivo_raw;

    return opt_novo_audio;
}
// mp3 : 4
// wav : 2 
// raw : 1

int destroi_audio(Audio *audio){
    if(audio == NULL || audio->amostras == NULL){
        printf("destroi_audio: erro, ponteiros inválidos para o audio ou para as amotras \n");
        return 1;
    }
    free(audio->amostras);
    free(audio);
    return 0;
}

int remove_arquivos(Audio *audio,int opcao){
    int erro = 0;

    if (opcao == 4 || opcao == 5 || opcao == 6 || opcao == 7 ){
        if(remove(audio->arquivo_mp3) == -1){
            printf("remove_audio: erro, não foi possível remover o .mp3\n");
            erro = 1;
        }
    }

    if (opcao == 1 || opcao == 5 || opcao == 3 || opcao == 7){
        if(remove(audio->arquivo_raw) == -1){
            printf("remove_audio: erro, não foi possível remover o .raw\n");
            erro = 1;
        }
    }

    if (opcao == 2 || opcao == 6 || opcao == 3 || opcao == 7){
        if(remove(audio->arquivo_wav) == -1){
            printf("remove_audio: erro, não foi possível remover o .wav\n");
            erro = 1;
        }
    }

    return erro;
}
// gerar arquivos temporários e depois dar rename
// Atualizar arquivos cria arquivos .mp3, .wav e .raw temporários e converte as amostras do áudio para
// representação nesses três formatos. Caso todas as conversões tenham sido bem-sucedidas, renomeamos os arquivos temporários para 
int atualizar_audio(Audio *audio){

    if(audio == NULL || audio->amostras == NULL ){
        printf("atualizar_audio: erro, áudio ou amostras do áudio inválidos\n");
        return 1;
    }

    int erro = 0;

    char *mp3_original = audio->arquivo_mp3;
    char *wav_original = audio->arquivo_wav;
    char *raw_original = audio->arquivo_raw;

    audio->arquivo_mp3 = "aux_audio.mp3";
    audio->arquivo_wav = "aux_audio.wav";
    audio->arquivo_raw = "aux_audio.raw";

    if (converterfloat_raw(audio) != 0){
        erro = 1;
        printf("atualizar_audio: erro na conversão para .raw\n");
    }
    
    if(converteraw_wav(audio) != 0){
        erro = 1;
        printf("atualizar_audio: erro na conversão para .wav\n");
    }
    
    if(converterwav_mp3(audio) != 0){
        erro = 1;
        printf("atualizar_audio: erro na conversão para .mp3\n");
    }
    
    if(erro != 1){
        if (rename(audio->arquivo_mp3, mp3_original) == -1) erro = 1;
        if (rename(audio->arquivo_wav, wav_original) == -1) erro = 1;
        if (rename(audio->arquivo_raw, raw_original) == -1) erro = 1;
        if(erro == 1){
            printf("atualizar_audio: erro na renomeação dos arquivos temporárioas\n");
        }
    }

    if(erro == 1){
        remove(audio->arquivo_mp3);
        remove(audio->arquivo_wav);
        remove(audio->arquivo_raw);
        return 1;
    }

    rename(audio->arquivo_mp3, mp3_original);
    rename(audio->arquivo_wav, wav_original);
    rename(audio->arquivo_raw, raw_original);

    audio->arquivo_mp3 = mp3_original;
    audio->arquivo_wav = wav_original;
    audio->arquivo_raw = raw_original;

    return 0;
}