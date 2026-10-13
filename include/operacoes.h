#ifndef OPERACOES_H
#define OPERACOES_H

#include "audio.h"

// documenta alterar_velocidade
/**
 * @brief Altera a velocidade de um áudio com base no fator
 * de alterção passado
 * @param *p Um ponteiro para o áudio que queremos alterar
 * @param fator Float que controla a velocidade da alteração
 * @return 0 se a alteração foi bem-sucedida e 1 caso contrário
 * @pre fator > 0.0
 * @post O vetor de amostras do áudio passado agora representa o 
 * mesmo áudio mas em velocidade diferente
 * @note fatores maiores que 1 aceleram o áudio, enquanto fatores
 * menores que 1 e maiores que 0 o desaceleram
*/

int inverter_audio(Audio *audio);

int alterar_velocidade(Audio *p, float fator);

// documenta alterar_frequencia
//int alterar_frequencia(Audio *p);

// documenta alterar_intensidade 
int alterar_intensidade(Audio *p, float ganho);
// documenta mixar_audio

int cortar_audio(Audio *p, float segundo_inicial, float segundo_final);

int mixar_audio(Audio *audio_mixado ,Audio *audio1, Audio *audio2, float ganho_audio1, float ganho_audio2, float ini_ad1, 
        float ini_ad2, float duracao);


#endif
