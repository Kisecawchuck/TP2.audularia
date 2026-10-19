#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "propriedades.h"

#define CHUNK 4096

int imprimir_amostras(Audio *audio) {
    if (!audio || !audio->amostras || audio->tamanho == 0) {
        fprintf(stderr, "imprimir_amostras: ERRO, argumentos inválidos.\n");
        return 1;
    }

    for (size_t i = 0; i < audio->tamanho; i++)
        printf("%f\n", audio->amostras[i]);
    return 0;
}


int salvar_amostras(Audio *audio, char *out) {
    if (!audio || !audio->amostras || audio->tamanho == 0 || audio->sample_rate || !out) {
        fprintf(stderr, "salvar_amostras: ERRO, argumentos inválidos.\n");
        return 1;
    }

    char cmd[1024];
    int n = snprintf(cmd, sizeof(cmd), "ffmpeg -y -loglevel error -f s16le -ar '%.0lf' -ac 1 -i - '%s'", audio->sample_rate, out);
    if (n < 0 || (size_t) n >= sizeof(cmd)) {
        fprintf(stderr, "salvar_amostras: caminho muito longo.\n");
        return 1;
    }

    FILE *pipeout = popen(cmd, "w");
    if (!pipeout) {
        fprintf(stderr, "salvar_amostras: não foi possível executar o FFmpeg.\n");
        return 2;
    }

    int16_t buf[CHUNK];
    int erro = 0;
    for (size_t i = 0; i < audio->tamanho && !erro; i += CHUNK) {
        size_t m = audio->tamanho - i;
        if (m > CHUNK) m = CHUNK;

        for (size_t j = 0; j < m; j++) {
            double v = audio->amostras[i + j];
            // clamp nos valores;
            if (v > 1.0) v = 1.0;
            if (v < -1.0) v = -1.0;
            // arredonda para o inteiro mais próximo;
            buf[j] = (int16_t) lrint(v * 32767.0);
        }
        if (fwrite(buf, sizeof(buf[0]), m, pipeout) != m) erro = 2;
    }
    if (pclose(pipeout) != 0) erro = 2;

    return erro;
}
