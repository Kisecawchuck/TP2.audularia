#ifndef AUDIO_H
#define AUDIO_H

#include <stdlib.h>

// define o tipo AUDIO
typedef struct {
    float *amostras;
    char *arquivo_mp3;
    char *arquivo_wav;
    char *arquivo_raw;
    size_t tamanho;
} Audio;


// Essas funções usam a bilbioteca ffmpeg do bash e só estão aqui 
// para conversão dos arquivos com que iremos trabalhar, não as implementamos: 

// documenta convertemp3_raw
/**
 * @brief Converte um arquivo .mp3 em um arquivo .wav
 * @param *audio Um ponteiro para um áudio
 * @return 0 se a conversão foi bem-sucedida e 1 caso contrário
 * @pre audio->arquivo_wav != NULL
 * @pre audio->arquivo_mp3 existe
 * @pre audio->arquivo_mp3 != NULL 
*/
int convertermp3_wav(Audio *p);


// documenta convertewav_raw
/**
 * @brief Converte um arquivo .wav em um arquivo .raw
 * @param *audio Um ponteiro para um áudio
 * @return 0 se a conversão foi bem-sucedida e 1 caso contrário
 * @pre audio->arquivo_raw != NULL
 * @pre audio->arquivo_wav existe
 * @pre audio->arquivo_wav != NULL 
*/
int converterwav_raw(Audio *p);


// documenta converteraw_wav
/**
 * @brief Converte um arquivo .raw em um arquivo .wav
 * @param *audio Um ponteiro para um áudio
 * @return 0 se a conversão foi bem-sucedida e 1 caso contrário
 * @pre audio->arquivo_raw != NULL
 * @pre audio->arquivo_raw existe
 * @pre audio->arquivo_wav != NULL 
*/
int converteraw_wav(Audio *p);


// documenta convertewav_mp3
/**
 * @brief Converte um arquivo .wav em um arquivo .mp3
 * @param *audio Um ponteiro para um áudio
 * @return 0 se a conversão foi bem-sucedida e 1 caso contrário
 * @pre audio->arquivo_mp3 != NULL
 * @pre audio->arquivo_wav existe 
 * @pre audio->arquivo_wav != NULL 
*/

int converterwav_mp3(Audio *p);
//

// Somente implementamos essas:

// documenta converteraw_floats
/**
 * @brief Salva as amostras de áudio dentro do arquivo em fomrato raw
 * dentro de um array de floats
 * @param *audio Um ponteiro para um áudio
 * @return 0 se a alteração foi bem-sucedida e 1 caso contrário
 * @pre audio->arquivo_raw != NULL
 * @post audio->amostras agora representa as amostras do áudio dentro de raw
 * em vários floats de um array
*/

int converterraw_floats(Audio *p);


// documenta convertefloat_raw
/**
 * @brief Salva as amostras dentro do array audio->amostras em um arquivo .raw
 * @param *audio Um ponteiro para um áudio
 * @return 0 se a alteração foi bem-sucedida e 1 caso contrário
 * @pre audio->amostras != NULL
 * @pre audio->arquivo_raw != NULL
 * @post audio->amostras agora representa as amostras do áudio dentro de raw
 * em vários floats de um array
*/
int converterfloat_raw(Audio *p);


// documenta criar_audio
/**
 * @brief Cria um objeto do tipo áudio, converte um audio no formato .mp3 ou .wav
 * nos formatos que serão utilizados no processamento do áudio: array, raw, wav e mp3
 * e retorna o objeto criado
 * @param arquivo_mp3 caminho do arquivo .mp3
 * @param arquivo_wav caminho do arquivo .wav
 * @param arquivo_raw caminho do arquivo .raw 
 * @return Um objeto do tipo Audio que representa o arquivo de áudio passado como
 * parametro com os nomes de arquivo passados como parâmetro, ou null 
 * caso a criação não tenha sido bem-sucedida
*/

Audio *criar_audio(char *arquivo_mp3, char *arquivo_wav, char *arquivo_raw);


// documenta remove_arquivos
/**
 * @brief Remove todos os aruivos relacionados ao objeto áudio com base na opcao 
 * passada como parâmetro.
 * @param audio ponteiro para um objeto do tipo áudio
 * @param opcao representa a opcao de exclusão dos arquivos
 * @return 0 caso os arquivos requisitados para exclusão tenham sido apagados com 
 * sucesso, e 1 caso contrário.
 * @note Com o parametro opcao podemos definir as seguintes opções de exclusão de arquivos
 * opcao = 4: arquivo mp3 
 * opcao = 2: arquivo wav 
 * opcao = 1: arquivo raw
 * Se opcao receber a soma dos valores que representam cada arquivo, cada um desses arquivos 
 * será apagado.
 * exemplo: opcao = 6: apga o arquivo .mp3 e o arquivo .wav
 * opcao = 7: apaga todos os aruivos
 * opcao = 1: apaga somente o arquivo .raw
*/

int remove_arquivos(Audio *audio,int opcao);


// documenta atualizar_audio
/**
 * @brief Atualiza os arquivos .mp3, .wav e .raw de um objeto do tipo Audio
 * com base no estado atual de seu vetor de amostras.
 * @param audio ponteiro para um objeto do tipo Audio
 * @return 0 se todas as conversões de arquivosforam bem sucedidas e 1 caso contrário
 * @pre audio->amostras != NULL
*/

int atualizar_audio(Audio *audio);


// documenta destroi_audio
/**
 * @brief Libera o espaço alocado por audio
 * @param audio ponteiro para um objeto do tipo Audio
 * @return 1 caso a alocação não ponteiros para as amostras ou 
 * para o audio sejam in válidos (== NULL)
*/

int destroi_audio(Audio *audio);

#endif
