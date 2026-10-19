#include "../include/audio.h"
#include "../include/operacoes.h"
#include <stdio.h>
#include <stdlib.h>
#include<assert.h>
#define SAMPLE_RATE 16000

static void test_criar_audio(int opt){
    if(opt == 1){
        assert(criar_audio(NULL,SAMPLE_RATE));

    }else if(opt == 2){
        assert(criar_audio("../audio/Super_mario.mp3",-9));
        }
    }


static void test_destruir_audio(int opt){
    assert(destruir_audio(NULL) == 0);
}

static void test_carregar_amostras(int opt){
    if(opt == 1){
        assert(carregar_amostras(NULL) == 0);

    }else if(opt == 2){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
            Mario->filename = NULL;
            assert(carregar_amostras(Mario) == 0);

    }else if(opt == 3){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",-9);
        assert(carregar_amostras(Mario) == 0);
    }
}

static void test_remover_arquivo_audio(int opt){
    if(opt == 1){
        assert(remover_arquivo_audio(NULL) == 0);

    }else if(opt == 2){
        Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
        Mario->filename = NULL;
        assert(remover_arquivo_audio(Mario) == 0);

    }else if(opt == 3){
        Audio *Mario = criar_audio("Arquivo inexistente",SAMPLE_RATE);
        assert(remover_arquivo_audio(Mario) == 0);
    }
}

int main(){
    test_destruir_audio(2);
    return 0;
}
