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