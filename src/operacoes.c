#include<stdio.h>
#include<stdlib.h>
#include"../include/audio.h"
#include"../include/operacoes.h"

int alterar_intensidade(Audio *audio, float ganho){

    if (audio == NULL || audio->amostras == NULL || audio->tamanho <= 0){
            printf("Erro, áudio ou amostras do áudio inválidos \n");
            return 1;
        }

    if(ganho < 0){
        printf("Fator de ganho inválido, ele deve ser maior que 0\n");
        return 1;
    }

    int n = audio->tamanho;

    for(int i = 0; i < n; i++){
        audio->amostras[i] = audio->amostras[i]*ganho;
        if(audio->amostras[i] > 1.0) audio->amostras[i] = 1.0;
        if(audio->amostras[i] < -1.0) audio->amostras[i] = -1.0;
    }

    return 0;
}

// Podemos acelerar um áudio pulando amostras dentro do vetor com base em um certo fator, para isso podemos fazer com que o vetor
// apenas salve as amostras pares, com um fator igual a 2, tornano o áudio 2x mais rápido.

// Selecionmaos com base no fator os i's que serão usados ou para mesclar amostras ou inserir novas amostras entre as existentes;
// Podemos desacelerar um áudio adcionando amostras artificiais dentro do vetor, para isso podemos criar amostras artificiis com base
// em amostras adjascentes e adicionar as artificiais entre elas, será necessário um vetor maior, além do deslocamento das amostras
// já dentro do vetor original.

int alterar_velocidade(Audio *audio,float fator){

    if (audio == NULL || audio->amostras == NULL || audio->tamanho <= 0){
            printf("Erro, áudio ou amostras do áudio inválidos \n");
            return 1;
        }

    if(fator < 0){
        printf("Fator de velocidade inválido, ele deve ser maior que 0\n");
        return 1;
    }

    int n = audio->tamanho;
    int m = (int)(n/fator);

    float *aux = malloc(m * sizeof(float));
    
    if (aux == NULL){
        printf("Erro na alocação \n");
        return 1;
    }

    if (!aux) return 1;
    
    for(int i = 0; i < m;i++){

        float pos = i * fator;
        int i0 = (int)pos;
        int i1;
        if(i0 + 1 < n) i1 = i0 + 1;
        else i1 = i0;

        // Parte fracionária de pos, a posição intermediária em que colocamos a amostra artificial no caso da aceleração
        float frac = pos - i0; 
        
        // Também podemos definir o quanto de cada amostra adjascente (porcentagem) será usado para gerar a artificial.
        // No caso desse trabalho estou considerando a pos de colocação da amostra artificial no vetor final
        // com base no fator de velocidade escolhido.
        
        aux[i] = audio->amostras[i0] * (1.0 - frac) + audio->amostras[i1] * frac;
    }
    
    audio->tamanho = m;
    audio->amostras = aux;
    
    return 0;

    }

    // Pode diminuir a intensidade de cada áudio dependendo do ganho utilizado
    int mixar_audio(Audio *audio_mixado ,Audio *audio1, Audio *audio2, float ganho_audio1, float ganho_audio2, float ini_ad1, 
        float ini_ad2, float duracao){

    if (audio_mixado == NULL || audio1 == NULL || audio2 <= 0){
            printf("Erro, áudios inválidos \n");
            return 1;
        }
    
    if (audio1->amostras == NULL || audio2->amostras == NULL){
            printf("Erro, amostras de algum dos áudios inválidas \n");
            return 1;
        } 
    
    if(ganho_audio1 < 0 || ganho_audio2 < 0){
        printf("Ganho de áudio inválido, ele deve ser maior que 0\n");
        return 1;
    }

    if(ini_ad1 < 0  || ini_ad2 < 0){
        printf("Segundo de ínicio inváildo, ele deve ser maior que 0\n");
        return 1;
    }

    if(duracao < 0){
        printf("Duração inválida, ela deve ser maior que 0\n");
        return 1;
    }
    
    int tam_audio1 = audio1->tamanho;
    int tam_audio2 = audio2->tamanho;
    int fim_ad2 = ini_ad2*16000 + tam_audio2;
    int fim_ad1 = ini_ad1*16000 + tam_audio1;
    int tam_audiofinal = duracao*16000;
    
    if(fim_ad1 > tam_audiofinal || fim_ad2 > tam_audiofinal){
        printf("Essa mixagem excederá o tamanho do áudio final!\n");
        return 1;
    }

    float *mix = malloc(tam_audiofinal * sizeof(float));

    if(mix == NULL){
        printf("Erro na alocação \n");
        return 1;
    }

    int i=0;
    float parte_ad1;
    float parte_ad2;

    while(i < tam_audiofinal){
     
        if( ini_ad1*16000 <= i && i < fim_ad1 ){
            parte_ad1 = (ganho_audio1*audio1->amostras[(int)(i-ini_ad1*16000)])/(ganho_audio1 + ganho_audio2);
        }else parte_ad1 = 0.0;

        if (ini_ad2*16000 <= i && i < fim_ad2){
            parte_ad2 = (ganho_audio2*audio2->amostras[(int)(i-ini_ad2*16000)])/(ganho_audio1 + ganho_audio2);
        }else parte_ad2 = 0.0;

        mix[i] = parte_ad1 + parte_ad2;
        i++;
    }
  
    audio_mixado->amostras = mix;
    audio_mixado->tamanho = tam_audiofinal;

    return 0;
    }
    
    int cortar_audio(Audio *audio, float segundo_inicial, float segundo_final){
        
        if (audio == NULL || audio->amostras == NULL || audio->tamanho <= 0){
            printf("Erro, áudio ou amostras do áudio inválidos \n");
            return 1;
        }

        if (segundo_final <= 0 || segundo_inicial < 0){
            printf("Erro, uso: segundo_final > 0; segundo inicial >=0 ");
            return 1;
        }

        if(segundo_final <= segundo_inicial){
            printf("Erro, uso: segundo final > segundo_incial");
            return 1;
        }
        
        if(segundo_final*16000 > audio->tamanho){
            printf("O segundo final excede a duração do áudio \n");
            return 1;
        }

        int m = (segundo_final - segundo_inicial)*16000;

        if(m <= 0){
            printf("Trecho cortado vazio com o segundo_incial e segundo_final passados como parametro\n");
            return 1;
        }

        float *audio_cortado = malloc(m * sizeof(float))
        ;
        if(audio_cortado == NULL){
            printf("Erro na alocação \n");
            return 1;
        }

        int ini = (int)(segundo_inicial*16000);
        int fim =  (int)(segundo_final*16000);

        for(int i = ini; i < fim; i++){
            audio_cortado[i - ini] = audio->amostras[i];
        }
        
        free(audio->amostras);
        audio->tamanho = m;
        audio->amostras = audio_cortado;
        return 0;
    }

    int inverter_audio(Audio *audio){
        
        if (audio == NULL || audio->amostras == NULL || audio->tamanho <= 0){
            printf("Erro, áudio ou amostras do áudio inválidos \n");
            return 1;
        }

        int tam_audio = audio->tamanho;
        int i = 0;
        int j = tam_audio-1;

        while(i < j){
            float tmp = audio->amostras[i];
            audio->amostras[i] = audio->amostras[j];
            audio->amostras[j] = tmp;
            i++;
            j--;
        } 

        return 0;
        
    }
    
    // Fazer uma função que escolhe em qual segundo do áudio 1 encaixaremos o áudio 2