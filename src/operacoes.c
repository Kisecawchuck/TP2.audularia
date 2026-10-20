#include <stdio.h>
#include <stdlib.h>
#include "audio.h"

int alterar_intensidade(Audio *audio, double ganho) {
    if (audio == NULL || audio->amostras == NULL || audio->tamanho <= 0) {
        printf("alterar_intensidade: erro, áudio ou amostras do áudio inválidos \n");
        return 1;
    }

    if (ganho <= 0) {
        printf("alterar_intensidade: erro, fator de ganho inválido, ele deve ser maior que 0\n");
        return 1;
    }

    for (size_t i = 0, n = audio->tamanho; i < n; i++) {
        audio->amostras[i] = audio->amostras[i] * ganho;
        if (audio->amostras[i] > 1.0) audio->amostras[i] = 1.0;
        if (audio->amostras[i] < -1.0) audio->amostras[i] = -1.0;
    }

    return 0;
}

// Podemos acelerar um áudio pulando amostras dentro do vetor com base em um certo fator, para isso podemos fazer com que o vetor
// apenas salve as amostras pares, com um fator igual a 2, tornano o áudio 2x mais rápido.

// Selecionmaos com base no fator os i's que serão usados ou para mesclar amostras ou inserir novas amostras entre as existentes;
// Podemos desacelerar um áudio adcionando amostras artificiais dentro do vetor, para isso podemos criar amostras artificiis com base
// em amostras adjascentes e adicionar as artificiais entre elas, será necessário um vetor maior, além do deslocamento das amostras
// já dentro do vetor original.

int alterar_velocidade(Audio *audio, double fator) {
    if (audio == NULL || audio->amostras == NULL || audio->tamanho <= 0) {
        printf("alterar_velocidade: erro, áudio ou amostras do áudio inválidos \n");
        return 1;
    }

    if (fator <= 0) {
        printf("alterar_velocidade: fator de velocidade inválido, ele deve ser maior que 0\n");
        return 1;
    }

    size_t n = audio->tamanho;
    size_t m = (size_t) (n / fator);

    if (m < 1) {
        printf("alterar_velocidade: erro, o fator passado torna o tamanho do áudio final inválido \n");
        return 1;
    }

    double *aux = malloc(m * sizeof(double));

    if (aux == NULL) {
        printf("Erro na alocação \n");
        return 1;
    }

    for (size_t i = 0; i < m; i++) {
        double pos = i * fator;
        size_t i0 = (size_t) pos;
        size_t i1 = i0 + 1 * (i0 + 1 < n);

        // Parte fracionária de pos, a posição intermediária em que colocamos a amostra artificial no caso da aceleração
        double frac = pos - i0; 

        // Também podemos definir o quanto de cada amostra adjascente (porcentagem) será usado para gerar a artificial.
        // No caso desse trabalho estou considerando a pos de colocação da amostra artificial no vetor final
        // com base no fator de velocidade escolhido.

        aux[i] = audio->amostras[i0] * (1.0 - frac) + audio->amostras[i1] * frac;
    }

    free(audio->amostras);
    audio->tamanho = m;
    audio->amostras = aux;

    return 0;
}

// Pode diminuir a intensidade de cada áudio dependendo do ganho utilizado
int mixar_audio(Audio *audio_mixado, Audio *audio1, Audio *audio2, double ganho_audio1, double ganho_audio2, double ini_ad1, 
        double ini_ad2, double duracao) {

    if (audio1->tamanho <= 0 || audio2->tamanho <= 0) {
        printf("mixar_audio: erro, tamanhos dos áudios inválidos \n");
        return 1;
    } 

    if (audio1->amostras == NULL || audio2->amostras == NULL ) {
        printf("mixar_audio: erro, amostras de algum dos áudios inválidas \n");
        return 1;
    } 

    if (ganho_audio1 <= 0 || ganho_audio2 <= 0) {
        printf("mixar_audio: erro, ganho de áudio inválido, ele deve ser maior que 0\n");
        return 1;
    }

    if (ini_ad1 < 0  || ini_ad2 < 0) {
        printf("mixar_audio: erro, segundo de ínicio inváildo, ele deve ser maior que 0\n");
        return 1;
    }

    if (duracao < 0) {
        printf("mixar_audio: erro, duração inválida, ela deve ser maior que 0\n");
        return 1;
    }

    size_t ini1 = (size_t) (ini_ad1 * audio1->sample_rate);
    size_t ini2 = (size_t) (ini_ad2 * audio2->sample_rate);

    size_t tam_audio1 = audio1->tamanho;
    size_t tam_audio2 = audio2->tamanho;

    size_t fim_ad2 = ini2 + tam_audio2;
    size_t fim_ad1 = ini1 + tam_audio1;

    size_t tam_audiofinal = (size_t) (duracao * audio_mixado->sample_rate);

    if (fim_ad1 > tam_audiofinal || fim_ad2 > tam_audiofinal) {
        printf("mixar_audio:erro, essa mixagem excederá o tamanho do áudio final!\n");
        return 1;
    }

    double *mix = malloc(tam_audiofinal * sizeof(double));

    if (mix == NULL) {
        printf("Erro na alocação \n");
        return 1;
    }

    double parte_ad1;
    double parte_ad2;

    for (size_t i = 0; i < tam_audiofinal; i++) {
        if (ini1 <= i && i < fim_ad1) {
            parte_ad1 = (ganho_audio1*audio1->amostras[(i-ini1)])/(ganho_audio1 + ganho_audio2);
        } else parte_ad1 = 0.0;

        if (ini2 <= i && i < fim_ad2) {
            parte_ad2 = (ganho_audio2*audio2->amostras[(i-ini2)])/(ganho_audio1 + ganho_audio2);
        } else parte_ad2 = 0.0;

        mix[i] = parte_ad1 + parte_ad2;
    }

    audio_mixado->amostras = mix;
    audio_mixado->tamanho = tam_audiofinal;

    return 0;
}

int cortar_audio(Audio *audio, double segundo_inicial, double segundo_final) {
    if (audio == NULL || audio->amostras == NULL || audio->tamanho <= 0) {
        printf("cortar_audio: erro, áudio ou amostras do áudio inválidos \n");
        return 1;
    }

    if (segundo_final <= 0 || segundo_inicial < 0) {
        printf("coratr_audio: erro, uso: segundo_final > 0; segundo inicial >=0 ");
        return 1;
    }

    if (segundo_final <= segundo_inicial) {
        printf("cortar_audio: erro, uso: segundo final > segundo_incial");
        return 1;
    }

    if (segundo_final*audio->sample_rate > audio->tamanho) {
        printf("cortar_audio: erro segundo final excede a duração do áudio \n");
        return 1;
    }

    size_t m = (segundo_final - segundo_inicial) * audio->sample_rate;

    if (m <= 0) {
        printf("cortar_audio: erro, trecho cortado vazio com o segundo_incial e segundo_final passados como parametro\n");
        return 1;
    }

    double *audio_cortado = malloc(m * sizeof(double));

    if (audio_cortado == NULL) {
        printf("Erro na alocação \n");
        return 1;
    }

    size_t ini = (size_t) (segundo_inicial * audio->sample_rate);
    size_t fim =  (size_t) (segundo_final * audio->sample_rate);

    for (size_t i = ini; i < fim; i++) {
        audio_cortado[i - ini] = audio->amostras[i];
    }

    free(audio->amostras);
    audio->tamanho = m;
    audio->amostras = audio_cortado;
    return 0;
}

int inverter_audio(Audio *audio) {
    if (audio == NULL || audio->amostras == NULL || audio->tamanho <= 0){
        printf("inverter_audio: erro, áudio ou amostras do áudio inválidos \n");
        return 1;
    }

    for (size_t i = 0, j = audio->tamanho - 1; i < j; i++, j--) {
        double tmp = audio->amostras[i];
        audio->amostras[i] = audio->amostras[j];
        audio->amostras[j] = tmp;
    } 

    return 0;
}

// Fazer uma função que escolhe em qual segundo do áudio 1 encaixaremos o áudio 2
