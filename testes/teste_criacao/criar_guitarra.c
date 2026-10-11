#include<stdio.h>
#include<stdlib.h>
#include"../../include/audio.h"
#include"../../include/operacoes.h"

int main(){
    Audio *opt_nova_guitarra;

    opt_nova_guitarra = criar_audio("Guitarra1.mp3", "Guitarra1.wav", "Guitarra1.raw" );
    alterar_velocidade(opt_nova_guitarra,2.0);
    atualizar_audio(opt_nova_guitarra);

    system("ffplay -f f32le -ar 16000 -ch_layout mono Guitarra1.raw");
    destroi_audio(opt_nova_guitarra, 3);
    return 0;
}