#include<stdio.h>
#include<stdlib.h>
#include"../include/audio.h"
#include"../include/operacoes.h"

int alterar_intensidade(Audio *audio, float ganho){
    int n = audio->tamanho;
    for(int i = 0; i < n; i++){

        audio->amostras[i] = audio->amostras[i]*ganho;
        if(audio->amostras[i] > 1.0) audio->amostras[i] = 1.0;
        if(audio->amostras[i] < -1.0) audio->amostras[i] = -1.0;
    }
    return 0;
}

int alterar_velocidade(Audio *audio,float fator){
    int n = audio->tamanho;
    int m = (int)(n/fator);

    float *aux = malloc(m * sizeof(float));
    if (!aux) return 1;
    
    for(int i = 0; i < m;i++){

        // selecionmaos de com base no fator os i's que serão usados ou para mesclar amostras ou inserir novas amostras entre as existentes;

        // Podemos acelerar um áudio pulando amostras dentro do vetor com base em um certo fator, para isso podemos fazer com que o vetor
        // apenas salve as amostras pares, com um fator igual a 2, tornano o áudio 2x mais rápido.

        // Podemos desacelerar um áudio adcionando amostras artificiais dentro do vetor, para isso podemos criar amostras artificiis com base
        // em amostras adjascentes e adicionar as artificiais entre elas, será necessário um vetor maior, além do deslocamento das amostras
        // já dentro do vetor original.

        float pos = i * fator;
        int i0 = (int)pos;
        int i1;
        if(i0 + 1 < n) i1 = i0 + 1;
        else i1 = i0;
        float frac = pos - i0; 
        
        // parte fracionária de pos, a posição intermediária em que colocamos a amostra artificial no caso da aceleração
        // também podemos definir o quanto de cada amostra adjascente (porcentagem) será usado para gerar a artificial.
        
        aux[i] = audio->amostras[i0] * (1.0 - frac) + audio->amostras[i1] * frac;
        // No caso da desaceleração, pegará para i o piso do valor de pos
    }
   
    audio->tamanho = m;
    audio->amostras = aux;
    
    return 0;

    }
    // Pode diminuir a intensidade de cada áudio dependendo do ganho utilizado
    int mixar_audio(Audio *audio_mixado ,Audio *audio1, Audio *audio2, float ganho_audio1, float ganho_audio2){
        int maior_tamanho;
        int menor_tamanho;
        int tam_audio1 = audio1->tamanho;
        int tam_audio2 = audio2->tamanho;

        float menor_ganho = 1.0;
        float maior_ganho = 1.0;

    if(tam_audio2 < tam_audio1){
        maior_tamanho = tam_audio1;
    }
    else{
        maior_tamanho = tam_audio2;
    }

    float *mix = malloc(maior_tamanho * sizeof(float));
    if(mix == NULL){
        printf("Erro na alocação \n");
        return 1;
    }
    int i = 0;
    int j = 0;

    while (i < tam_audio1 || j < tam_audio2)
    {
        if(i >= tam_audio1){
            mix[j] = (ganho_audio2*audio2->amostras[j])/(ganho_audio1 + ganho_audio2);
            j++;
        }else if( j >= tam_audio2){
            mix[i] = (ganho_audio1*audio1->amostras[i])/(ganho_audio1 + ganho_audio2);
            i++;
        }else{
            mix[i] = (ganho_audio1*audio1->amostras[i] + ganho_audio2*audio2->amostras[i])/(ganho_audio1 + ganho_audio2);
            i++;
            j++;
        }
    }
    
   
    audio_mixado->amostras = mix;
    audio_mixado->tamanho = maior_tamanho;

    return 0;
}

    int cortar_audio(Audio *audio, float segundo_inicial, float segundo_final){
        
        if(segundo_final > audio->tamanho/16000){
            printf("O segundo final excede a duração do áudio \n");
            return 1;
        }

        int m = (segundo_final - segundo_inicial)*16000;
        float *audio_cortado = malloc(m * sizeof(float));
        if(audio_cortado == NULL){
            return 1;
        }

        int ini = (int)(segundo_inicial*16000);
        int fim =  (int)(segundo_final*16000);

        for(int i = (int)(segundo_inicial*16000); i < (int)(segundo_final*16000); i++){
            audio_cortado[i - ini] = audio->amostras[i];
        }

        audio->tamanho = m;
        audio->amostras = audio_cortado;
        return 0;
    }