#include "../include/audio.h"
#include "../include/operacoes.h"
#include <stdio.h>
#include <stdlib.h>
#include<assert.h>
#define SAMPLE_RATE 16000

static void test_inverter_audio(int opt){
    if(opt == 1){
        Audio *Mario = NULL;
        assert(inverter_audio(Mario) == 0);

    }else if(opt == 2){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
        Mario->amostras = NULL;
        assert(inverter_audio(Mario) == 0);
        destruir_audio(Mario);

        }

    }

static void test_cortar_audio(int opt){
    if(opt == 1){
        Audio *Mario = NULL;
        assert(cortar_audio(Mario,0.0, 2.0) == 0);

    }else if(opt == 2){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
        Mario->amostras = NULL;
        assert(cortar_audio(Mario,0.0, 2.0) == 0);
        destruir_audio(Mario);

    }else if(opt == 3){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
        carregar_amostras(Mario);
        assert(cortar_audio(Mario,-1, -1) == 0);
        destruir_audio(Mario);

    }else if(opt == 4){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
        carregar_amostras(Mario);
        assert(cortar_audio(Mario,2.0, 0.0) == 0);
        destruir_audio(Mario);

    }else if(opt == 5){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
        carregar_amostras(Mario);
        double tam_erro = (Mario->tamanho/SAMPLE_RATE)*10.0;
        assert(cortar_audio(Mario,0.0, tam_erro) == 0);
        destruir_audio(Mario);

    }else if(opt == 6){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
        carregar_amostras(Mario);
        assert(cortar_audio(Mario,2.0, 2.0) == 0);
        destruir_audio(Mario);
        }
}

static void test_alterar_intensidade(int opt){
    if (opt == 1 ){
        Audio *Mario = NULL;
        assert(alterar_intensidade(Mario,2.0) == 0);

    }else if(opt == 2){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
        Mario->amostras = NULL;
        assert(alterar_intensidade(Mario, 2.0) == 0);
        destruir_audio(Mario);

    }else if(opt == 3){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
        carregar_amostras(Mario);
        assert(alterar_intensidade(Mario, -1) == 0);
        destruir_audio(Mario);
    }

}

static void test_alterar_velocidade(int opt){
    if (opt == 1){
        Audio *Mario = NULL;
        assert(alterar_velocidade(Mario,2.0) == 0);

    }else if(opt == 2){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
        Mario->amostras = NULL;
        assert(alterar_velocidade(Mario, 2.0) == 0);
        destruir_audio(Mario);

    }else if(opt == 3){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
        carregar_amostras(Mario);
        assert(alterar_velocidade(Mario, -1) == 0);
        destruir_audio(Mario);
    }
}

int main(){
    // test_funcao(opcao_de_erro)
    test_alterar_velocidade(1);
    return 0;
}