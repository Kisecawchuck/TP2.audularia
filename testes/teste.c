#include<stdio.h>
#include<stdlib.h>
#include"../include/audio.h"
#include"../include/operacoes.h"

int main(){
    Audio Mario;
    Audio *opt_Mario = &Mario;

    Mario.arquivo_mp3 = "../audio/Super_mario.mp3";
    Mario.arquivo_wav = "../audio/Super_mario.wav";
    Mario.arquivo_raw = "../audio/Super_mario.raw";

    printf("%d \n", convertermp3_wav(opt_Mario));
    printf("%d \n", converterwav_raw(opt_Mario));
    printf("%d \n",converterraw_floats(opt_Mario));

    Audio Pokemon;
    Audio *opt_Pokemon = &Pokemon;

    Pokemon.arquivo_mp3 = "../audio/Pokemon_evolution.mp3";
    Pokemon.arquivo_wav = "../audio/Pokemon_evolution.wav";
    Pokemon.arquivo_raw = "../audio/Pokemon_evolution.raw";

    printf("%d \n", convertermp3_wav(opt_Pokemon));
    printf("%d \n", converterwav_raw(opt_Pokemon));
    printf("%d \n",converterraw_floats(opt_Pokemon));


    Audio novo_mix;
    Audio *opt_novo_mix = &novo_mix;
    mixar_audio(opt_novo_mix,opt_Mario,opt_Pokemon,8.0,4.0);
    
    opt_novo_mix->arquivo_raw = "../audio/novo_mix.raw";

    //alterar_velocidade(opt_novo_mix,2.0);
    printf("%d \n",converterfloat_raw(opt_novo_mix));
    system("ffplay -f f32le -ar 16000 -ch_layout mono ../audio/novo_mix.raw");
    free(opt_Pokemon->amostras);
    //free(opt_novo_mix->amostras);
    free(opt_Mario->amostras);
    return 0;
}