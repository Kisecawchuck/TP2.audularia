#ifndef AUDIO_H
#define AUDIO_H

#include <stddef.h>

/*
 * - amostras: tamnho de valores double em [-1.0, 1.0]
 * - tamanho: número de amostras (não de bytes)
*/
typedef struct {
    double *amostras;
    char *filename;
    size_t tamanho;
    double sample_rate; // Hz
} Audio;

/*
 * @brief carrega arquivo de áudio suportado pelo FFmpeg, converte para mono
 * e para a taxa sample_rate
 * @param filename caminho do arquivo
 * @param sample_rate taxa de amostras
 * @return novo Audio, ou NULL em erro (usário deve liberar com destruir_audio)
 */
Audio *criar_audio(const char *filename, double sample_rate);


/*
 * @brief Libera audio, amostras e filename
 * @param audio ponteiro para um objeto do tipo Audio
 * @return 1 caso a alocação não ponteiros para as amostras ou 
 * para o audio sejam in válidos (== NULL), 0 em sucesso
*/
int destruir_audio(Audio *audio);


/**
 * @brief Salva as amostras do arquivo de áudio dentro de um array de doubles
 * @param *audio Um ponteiro para um áudio
 * @return 0 se a alteração foi bem-sucedida,
 * 1 se audio for inválido;
 * 2 comando longo demais;
 * 3 falha ao abrir o arquivo de áudio;
 * 4 falha ao alocar amostras;
 * 5 falha ao alocar buffer temporário;
 * 6 falha de leitura;
 * @post audio->amostras e audio->tamanho refletem o arquivo; permanecem inalterado
 * em caso de erro
*/
int carregar_amostras(Audio *audio);


/**
 * @brief Remove todos os arquivos relacionados ao objeto áudio
 * passada como parâmetro.
 * @param audio ponteiro para um objeto do tipo áudio
 * @return 0 caso os arquivos requisitados para exclusão tenham sido apagados com 
 * sucesso, e 1 caso contrário.
*/
int remover_arquivo_audio(Audio *audio);


#endif
