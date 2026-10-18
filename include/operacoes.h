#ifndef OPERACOES_H
#define OPERACOES_H

#include "audio.h"
// Cuidado, não usar fatores extremamente pequenos

// documenta alterar_velocidade
/**
 * @brief Altera a velocidade de um áudio com base no fator
 * de alterção passado
 * @param *audio Um ponteiro para o áudio que queremos alterar
 * @param fator Float que controla a velocidade da alteração
 * @return 0 se a alteração foi bem-sucedida e 1 caso contrário
 * @pre audio->amostra != NULL
 * @pre 0.0 < fator <= 10
 * @post O vetor de amostras do áudio passado agora representa o 
 * mesmo áudio mas em velocidade diferente
 * @note fatores maiores que 1 aceleram o áudio, enquanto fatores
 * menores que 1 e maiores que 0 o desaceleram
*/
int alterar_velocidade(Audio *audio, float fator);

// Documenta inverter_audio
/**
 * @brief Inverte o áudio mudando a ordem de suas amostras
 * @param *p Um ponteiro para o áudio que queremos alterar
 * @return 0 se a alteração foi bem-sucedida e 1 caso contrário
 * @pre audio->amostras != NULL
 * @post O vetor de amostras do áudio passado agora representa o 
 * mesmo áudio mas na ordemo reversa
*/
int inverter_audio(Audio *audio);

// documenta alterar_intensidade 
/**
 * @brief Aumenta ou diminui o volume padrão de um áudio
 * @param *p Um ponteiro para o áudio que queremos alterar
 * @return 0 se a alteração foi bem-sucedida e 1 caso contrário
 * @pre audio->amostras != NULL
 * @pre 0.0 < ganho <= 10
 * @post O vetor de amostras do áudio passado agora representa o 
 * mesmo áudio mas com o volume alterado
 * @note ganhos entre 1 e 0 diminuem o volume final do áudio, e 
 * ganhos maiores que 1 aumentam seu volume
*/
int alterar_intensidade(Audio *audio, float ganho);

// documenta mixar_audio
/**
 * @brief Mixa (junta) dois áudios em um novo áudio
 * @param *audio1 Um ponteiro para um áudio
 * @param *audio2 Um ponteiro para um áudoo
 * @param ganho_audio1 ganho do audio1 na mixagem final
 * @param ganho_audio2 ganho do audio2 na mixagem final
 * @param ini_ad1 segundo de inicio do audio1 na mixagem final
 * @param ini_ad2 segundo de inicio do audio2 na mixagem final
 * @param duracao duracao total em segundos da mixagem final 
 * @return 0 se a alteração foi bem-sucedida e 1 caso contrário
 * @pre audio->amostras != NULL
 * @pre 0.0 < ganho1 <= 10
 * @pre 0.0 < ganho2 <= 10 
 * @post O objeto áudio mixado agora representa um áudio que contém
 * audio1 e audio2
*/
int mixar_audio(Audio *audio_mixado ,Audio *audio1, Audio *audio2, float ganho_audio1, float ganho_audio2, float ini_ad1, 
        float ini_ad2, float duracao);

// documenta cortar_audio
/**
 * @brief Corta o audio em apenas uma parte dele
 * @param *audio Um ponteiro para um áudio
 * @param segundo_inicial incio do corte
 * @param segundo_final fim do corte
 * @return 0 se a alteração foi bem-sucedida e 1 caso contrário
 * @pre audio->amostras != NULL
 * @pre 0.0 <= segundo inicial <= audio_mixado->tamanho
 * @pre 0.0 <= segundo_final <= audio_mixado->tamanho
 * @post O vetor de amostras do áudio passado agora representa 
 * apenas um trecho do áudio original com base no intervalo
 * passado como parâmetro.
*/
int cortar_audio(Audio *audio, float segundo_inicial, float segundo_final);

// documenta alterar_frequencia
//int alterar_frequencia(Audio *p);

#endif
