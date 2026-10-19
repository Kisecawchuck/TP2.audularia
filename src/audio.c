#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/wait.h>
#include "audio.h"

// flags do ffmpeg
// -i indica o arquivo de entrada
// -ar indica a taxa de amostragem
// -ch_layout mono define o arquivo de áudio para mono
// f16le formato de cada amostras de áudio: float de 16 bits
// -f define o formato que o ffmpeg usará para ler ou escrever o arquivo. Também define
// um formato padrão para o ffmpeg caso o formato não tenha sido fornecido ou seja inválido
// nos arquivos de entrada e saída do comando
// -loglevel ERROr mostra apenas as saídas de ERRO do comando

Audio *criar_audio(const char *filename, double sample_rate) {
    if (!filename || sample_rate <= 0.0) {
        fprintf(stderr, "criar_audio: argumentos inválidos.\n");
        return NULL;
    }

    Audio *audio = calloc(1, sizeof(Audio));
    if (!audio) {
        fprintf(stderr, "criar_audio: ERRO, falha na alocação do objeto do tipo Audio.\n");
        return NULL;
    }

    audio->filename = strdup(filename);
    audio->sample_rate = sample_rate;
    if (!audio->filename || carregar_amostras(audio)) {
        destruir_audio(audio);
        return NULL;
    }

    return audio;
}

int destruir_audio(Audio *audio) {
    if (!audio) {
        fprintf(stderr, "destruir_audio: ERRO, áudio inválido\n");
        return 1;
    }
    free(audio->amostras);
    free(audio->filename);
    free(audio);

    return 0;
}

// Lê cada float do arquivo .raw e adiciona em um vetor
// Para isso, calculamos o número de bytes do binário resultante do arquivo .raw
// e alocamos um vetor de floats na memória com base nesse número de bytes

int carregar_amostras(Audio *audio) {
    if (!audio || !audio->filename || audio->sample_rate <= 0.0) {
        fprintf(stderr, "carregar_amostras: ERRO, argumentos inválidos.\n");
        return 1;
    }

    char cmd[1024];
    int erro = snprintf(cmd, sizeof(cmd),
            "ffmpeg -v error -i '%s' -f s16le -ac 1 -",
            audio->filename);

    if (erro < 0 || (size_t) erro >= sizeof(cmd)) {
        fprintf(stderr, "carregar_amostras: ERRO, comando muito longo\n");
        return 2;
    }

    FILE *pipein = popen(cmd, "r");
    if (!pipein) {
        fprintf(stderr, "carregar_amostras: ERRO, falha ao abrir arquivo\n");
        return 3;
    }

    size_t capacidade = 4096;
    size_t n = 0;
    int16_t buffer[4096];

    double *amostras = malloc(capacidade * sizeof(*amostras));
    if (!amostras) {
        fprintf(stderr, "carregar_amostras: ERRO, falha ao alocar amostras.\n");
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
                fprintf(stderr, "carregar_amostras: ERRO, falha ao alocar buffer temporário.\n");
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
        fprintf(stderr, "carregar_amostras: ERRO, falha ao ler o áudio.\n");
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
        fprintf(stderr, "remove_audio: Arquivo %s removido com sucesso.\n", audio->filename);
        return 0;
    }

    fprintf(stderr, "remove_audio: ERRO, não foi possível remover %s\n", audio->filename);
    return 1;
}
