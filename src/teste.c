#include<stdio.h>
#include<stdlib.h>
#include"../include/audio.h"

int main(){
    Audio Mario;
    Audio *opt_Mario = &Mario;
    Mario.arquivo_mp3 = "../audio/Super_mario.mp3";
    Mario.arquivo_wav = "../audio/Super_mario.wav";
    Mario.arquivo_raw = "../audio/Super_mario.raw";

    printf("%d \n", convertermp3_wav(opt_Mario));
    printf("%d \n", converterwav_raw(opt_Mario));
    printf("%d \n",converterraw_floats(opt_Mario));
    for(int i = 0; i < opt_Mario->tamanho;i++){ 
        printf("%f\n",opt_Mario->amostras[i]);
    }
    printf("%d \n",converterfloat_raw(opt_Mario));
    return 0;
}