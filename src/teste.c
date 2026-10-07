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

    alterar_velocidade(opt_Mario,2.0);

    printf("%d \n",converterfloat_raw(opt_Mario));

    system("ffplay -f f32le -ar 16000 -ch_layout mono ../audio/Super_mario.raw");
    
    free(opt_Mario->amostras);
    return 0;
}