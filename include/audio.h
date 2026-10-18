#ifndef AUDIO_H
#define AUDIO_H

#include <stddef.h>
#include <stdint.h>

// define o tipo AUDIO
typedef struct {
    double *amostras;
    char *filename;
    size_t tamanho;
    double sample_rate;
} Audio;

/*
 * @brief Cria um objeto do tipo áudio, converte um audio no formato .mp3 ou .wav
 * nos formatos que serão utilizados no processamento do áudio: array, raw, wav e mp3
 * e retorna o objeto criado
 * @param filename caminho do arquivo
 * @param sample_rate taxa de amostras
 * @return Um objeto do tipo Audio que representa o arquivo de áudio passado como
 * parametro com os nomes de arquivo passados como parâmetro, ou null 
 * caso a criação não tenha sido bem-sucedida
*/
Audio *criar_audio(char *filename, double sample_rate);


/*
 * @brief Libera o espaço alocado por audio
 * @param audio ponteiro para um objeto do tipo Audio
 * @return 1 caso a alocação não ponteiros para as amostras ou 
 * para o audio sejam in válidos (== NULL)
*/
int destruir_audio(Audio *audio);


// Essas funções usam a bilbioteca ffmpeg do bash e só estão aqui 
// para conversão dos arquivos com que iremos trabalhar, não as implementamos: 

/**
 * @brief Converte um arquivo de áudio em um arquivo .mp3
 * @param *audio Um ponteiro para um áudio
 * @return 0 se a conversão foi bem-sucedida e 1 caso contrário
 * @pre audio->filename existe
*/
int convert_to_mp3(Audio *p, const char *out);


/**
 * @brief Converte um arquivo de áudio em um arquivo .wav
 * @param *audio Um ponteiro para um áudio
 * @return 0 se a conversão foi bem-sucedida e 1 caso contrário
 * @pre audio->filename existe
*/
int convert_to_wav(Audio *p, const char *out);

// Somente implementamos essas:

/**
 * @brief Salva as amostras de áudio dentro do arquivo em fomrato raw
 * dentro de um array de floats
 * @param *audio Um ponteiro para um áudio
 * @return 0 se a alteração foi bem-sucedida e 1 caso contrário
 * @pre audio->filename existe 
 * @post audio->amostras agora representa as amostras do áudio dentro de raw
 * em vários floats de um array
*/
int convert_to_floats(Audio *p);


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


/**
 * @brief Remove todos os aruivos relacionados ao objeto áudio com base na opcao 
 * passada como parâmetro.
 * @param audio ponteiro para um objeto do tipo áudio
 * @param opcao representa a opcao de exclusão dos arquivos
 * @return 0 caso os arquivos requisitados para exclusão tenham sido apagados com 
 * sucesso, e 1 caso contrário.
*/
int remover_arquivo_audio(Audio *audio);


/*
 * @brief Atualiza os arquivos .mp3, .wav e .raw de um objeto do tipo Audio
 * com base no estado atual de seu vetor de amostras.
 * @param audio ponteiro para um objeto do tipo Audio
 * @return 0 se todas as conversões de arquivosforam bem sucedidas e 1 caso contrário
 * @pre audio->amostras != NULL
*/
int atualizar_audio(Audio *audio, char *filename, double sample_rate);

#endif
