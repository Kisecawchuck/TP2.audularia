#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/audio.h"

bool arquivo_existe(char *caminho) {
    FILE *file = fopen(caminho, "r");
    if (!file) return false;

    fclose(file);
    return true;
}

// flags do ffmpeg
// -i indica o arquivo de entrada
// -ar indica a taxa de amostragem
// -ch_layout mono define o arquivo de áudio para mono
// f32le formato de cada amostras de áudio: float de 32 bits
// -f define o formato que o ffmpeg usará para ler ou escrever o arquivo. Também define
// um formato padrão para o ffmpeg caso o formato não tenha sido fornecido ou seja inválido
// nos arquivos de entrada e saída do comando
// -loglevel ERROr mostra apenas as saídas de ERRO do comando

Audio *criar_audio(char *filename, double sample_rate) {
    Audio *audio = malloc(sizeof(Audio));
    if (!audio) {
        printf("criar_audio: ERRO, na alocação do objeto do tipo Audio\n");
        return NULL;
    }

    audio->filename = filename;
    audio->sample_rate = sample_rate;
    audio->tamanho = 0;
    carregar_amostras(audio);

    return audio;
}

int destruir_audio(Audio *audio) {
    if (!audio) {
        printf("destruir_audio: ERRO, áudio inválido\n");
        return 1;
    }
    if (audio->amostras) {
        free(audio->amostras);
    }

    free(audio);
    return 0;
}

int convert_to_wav(Audio *audio, const char *out) {
    if (!audio || !audio->filename || !out) {
        printf("convert_to_wav: ERRO, argumentos inválidos\n");
        return 1;
    }

    if (!arquivo_existe(audio->filename)) {
        printf("convert_to_wav: ERRO, arquivo %s não existe\n", audio->filename);
        return 2;
    }

    if (audio->sample_rate <= 0 || audio->sample_rate > INT32_MAX) {
        printf("convert_to_wav: ERRO, taxa de amostragem inválida\n");
        return 3;
    }

    char cmd[1024];
    int erro = snprintf(cmd, sizeof(cmd),
            "ffmpeg -y -i '%s' -vn -ar '%.0f' -f wav '%s'", audio->filename, audio->sample_rate, out
    );

    if (erro < 0 || (size_t) erro > sizeof(cmd)) {
        printf("convert_to_wav: ERRO, comando muito longo\n");
        return 4;
    }

    int status = system(cmd);
    if (status == -1) {
        printf("convert_to_wav: ERRO, falha ao executar FFmpeg\n");
        return 5;
    }
    if (status != 0) {
        printf("convert_to_wav: ERRO, FFmpeg retornou erro\n");
        return 6;
    }

    return 0;
}

int convert_to_mp3(Audio *audio, const char *out) {
    if (!audio || !audio->filename || !out) {
        printf("convert_to_mp3: ERRO, argumentos inválidos\n");
        return 1;
    }

    if (!arquivo_existe(audio->filename)) {
        printf("convert_to_mp3: ERRO, arquivo %s não existe\n", audio->filename);
        return 2;
    }

    if (audio->sample_rate <= 0 || audio->sample_rate > INT32_MAX) {
        printf("convert_to_mp3: ERRO, taxa de amostragem inválida\n");
        return 3;
    }

    char cmd[1024];
    int erro = snprintf(cmd, sizeof(cmd),
            "ffmpeg -y -i '%s' -vn -ar '%.0f' -f mp3 '%s'", audio->filename, audio->sample_rate, out
    );

    if (erro < 0 || (size_t) erro > sizeof(cmd)) {
        printf("convert_to_mp3: ERRO, comando muito longo\n");
        return 4;
    }

    int status = system(cmd);
    if (status == -1) {
        printf("convert_to_mp3: ERRO, falha ao executar FFmpeg\n");
        return 5;
    }
    if (status != 0) {
        printf("convert_to_mp3: ERRO, FFmpeg retornou erro\n");
        return 6;
    }

    return 0;
}

int convert_to_raw(Audio *audio, const char *out) {
    if (!audio || !audio->filename || !out) {
        printf("convert_to_raw: ERRO, argumentos inválidos\n");
        return 1;
    }

    if (!arquivo_existe(audio->filename)) {
        printf("convert_to_raw: ERRO, arquivo %s não existe\n", audio->filename);
        return 2;
    }

    if (audio->sample_rate <= 0 || audio->sample_rate > INT32_MAX) {
        printf("convert_to_raw: ERRO, taxa de amostragem inválida\n");
        return 3;
    }

    char cmd[1024];
    int erro = snprintf(cmd, sizeof(cmd),
            "ffmpeg -y -i '%s' -vn -ar '%.0f' -f s16le -acodec pcm_s16le '%s'", audio->filename, audio->sample_rate, out
            );

    if (erro < 0 || (size_t) erro > sizeof(cmd)) {
        printf("convert_to_raw: ERRO, comando muito longo\n");
        return 4;
    }

    int status = system(cmd);
    if (status == -1) {
        printf("convert_to_raw: ERRO, falha ao executar FFmpeg\n");
        return 5;
    }
    if (status != 0) {
        printf("convert_to_raw: ERRO, FFmpeg retornou erro\n");
        return 6;
    }

    return 0;
}

// Lê cada float do arquivo .raw e adiciona em um vetor
// Para isso, calculamos o número de bytes do binário resultante do arquivo .raw
// e alocamos um vetor de floats na memória com base nesse número de bytes

int carregar_amostras(Audio *audio) {
    if (!audio || !audio->filename) {
        printf("carregar_amostras: ERRO, áudio ou arquivo %s inválidos\n", audio->filename);
        return 1;
    }

    char cmd[1024];
    int erro = snprintf(cmd, sizeof(cmd),
            "ffmpeg -v error -i '%s' -f s16le -ac 1 -",
            audio->filename);

    if (erro < 0 || (size_t) erro >= sizeof(cmd)) {
        printf("carregar_amostras: ERRO, comando muito longo\n");
        return 2;
    }

    FILE *pipein = popen(cmd, "r");
    if (!pipein) {
        printf("carregar_amostras: ERRO, falha ao abrir arquivo\n");
        return 3;
    }

    size_t capacidade = 4096;
    size_t n = 0;
    int16_t buffer[4096];

    double *amostras = malloc(capacidade * sizeof(*amostras));
    if (!amostras) {
        printf("carregar_amostras: ERRO, falha ao alocar amostras.\n");
        pclose(pipein);
        return 4;
    }

    size_t lidos;
    while ((lidos = fread(buffer, sizeof(char), 4096, pipein)) > 0) {
        if (n + lidos > capacidade) {
            size_t nova_capacidade = capacidade;
            while (nova_capacidade < n + lidos)
                nova_capacidade *= 2;
            double *tmp = realloc(amostras, nova_capacidade * sizeof(double));

            if (!tmp) {
                free(amostras);
                pclose(pipein);
                printf("carregar_amostras: ERRO, falha ao alocar buffer temporário.\n");
                return 5;
            }

            amostras = tmp;
            capacidade = nova_capacidade;
        }

        for (size_t i = 0; i < lidos; i++)
            amostras[n + i] = buffer[i] / 32768.0;

        n += lidos;
    }

    int erro_leitura = ferror(pipein);
    int status = pclose(pipein);

    if (erro_leitura || status == -1 || !WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        free(amostras);
        printf("carregar_amostras: ERRO, falha ao ler o áudio.\n");
        return 6;
    }

    free(audio->amostras);
    audio->amostras = amostras;
    audio->tamanho = n;

    return 0;
}

int remover_arquivo_audio(Audio *audio) {
    if (!audio || !audio->filename)
        return 1;

    if (remove(audio->filename) == 0) {
        printf("remove_audio: Arquivo %s removido com sucesso.\n", audio->filename);
        return 0;
    }

    printf("remove_audio: ERRO, não foi possível remover %s\n", audio->filename);
    return 1;
}

// gera arquivos temporários e os renomeia
// Atualizar arquivos cria arquivos .mp3, .wav e .raw temporários e converte as amostras do áudio para
// representação nesses três formatos. Caso todas as conversões tenham sido bem-sucedidas, renomeamos os arquivos temporários para 
int atualizar_audio(Audio *audio, char *filename, double sample_rate) {
    if (!audio || !audio->amostras) {
        printf("atualizar_audio: ERRO, áudio ou amostras do áudio inválidos\n");
        return 1;
    }

    if (audio->amostras) {
        free(audio->amostras);
    }
    audio->filename = filename;
    audio->sample_rate = sample_rate;
    audio->amostras = NULL;
    audio->tamanho = 0;

    return 0;
}
