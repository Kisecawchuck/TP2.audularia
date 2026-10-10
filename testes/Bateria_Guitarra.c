#include<stdio.h>
#include<stdlib.h>
#include"../include/audio.h"
#include"../include/operacoes.h"

int main(){
    Audio BASS;
    Audio *opt_BASS = &BASS;

    //BASS.arquivo_mp3 = "../audio/Bateria1.mp3";
    BASS.arquivo_wav = "../audio/BASS.wav";
    BASS.arquivo_raw = "../audio/BASS.raw";

    //printf("%d \n", convertermp3_wav(opt_Mario));
    printf("%d \n", converterwav_raw(opt_BASS));
    printf("%d \n",converterraw_floats(opt_BASS));
    cortar_audio(opt_BASS, 0, 20);

    Audio GTR;
    Audio *opt_GTR = &GTR;

    //GTR.arquivo_mp3 = "../audio/GTR.mp3";

    GTR.arquivo_wav = "../audio/GTR.wav";
    GTR.arquivo_raw = "../audio/GTR.raw";

    //printf("%d \n", convertermp3_wav(opt_GTR));
    printf("%d \n", converterwav_raw(opt_GTR));
    printf("%d \n",converterraw_floats(opt_GTR));
    cortar_audio(opt_GTR,0,20);

    Audio DRUMS;
    Audio *opt_DRUMS = &DRUMS;

    //DRUMS.arquivo_mp3 = "../audio/DRUMS.mp3";

    DRUMS.arquivo_wav = "../audio/DRUMS.wav";
    DRUMS.arquivo_raw = "../audio/DRUMS.raw";

    //convertermp3_wav(opt_DRUMS);
    converterwav_raw(opt_DRUMS);
    converterraw_floats(opt_DRUMS);

    cortar_audio(opt_DRUMS, 3, 23);
    
    Audio novo_mix;
    Audio *opt_novo_mix = &novo_mix;
    
    mixar_audio(opt_novo_mix,opt_DRUMS,opt_GTR,0.5,1.1,0.0,20.0,40);

    //mixar_audio(opt_novo_mix,opt_BASS,opt_novo_mix,0.4,1.1,0,0,40);
    opt_novo_mix->arquivo_raw = "../audio/novo_mix.raw";

    //alterar_velocidade(opt_novo_mix,2.0);
    converterfloat_raw(opt_novo_mix);
    system("ffplay -f f32le -ar 16000 -ch_layout mono ../audio/novo_mix.raw");

    free(opt_BASS->amostras);
    free(opt_novo_mix->amostras);
    free(opt_GTR->amostras);

    return 0;
}