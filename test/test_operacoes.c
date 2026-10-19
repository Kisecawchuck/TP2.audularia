#include "../include/audio.h"
#include "../include/operacoes.h"
#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>
#include<assert.h>
#include "../include/propriedades.h"
#define SAMPLE_RATE 16000

static void test_inverter_audio(){
    Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
    carregar_amostras(Mario);
    assert(inverter_audio(Mario) == 0);
    destruir_audio(Mario);
}

static void test_cortar_audio(){
    Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
    carregar_amostras(Mario);
    assert(cortar_audio(Mario,2.0,6.0) == 0);
    destruir_audio(Mario);
}

static void test_alterar_intensidade(){
    Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
    carregar_amostras(Mario);
    assert(alterar_intensidade(Mario,2.0) == 0);
    destruir_audio(Mario);
}

static void test_alterar_velocidade(){
    Audio *Mario = criar_audio("../audio/Super_mario.mp3",SAMPLE_RATE);
    carregar_amostras(Mario);
    assert(alterar_velocidade(Mario,2.0) == 0);
    destruir_audio(Mario);
}

int main(){
    test_inverter_audio();
    test_alterar_intensidade();
    test_cortar_audio();
    test_alterar_velocidade();
    return 0;
}