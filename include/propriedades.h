#ifndef PROPRIEDADES_H
#define PROPRIEDADES_H

#include "audio.h"

// documenta imprimir_samples
int imprimir_amostras(Audio *audio);
// documenta salvar_arquivo_wav
int salvar_amostras(const Audio *audio,const char *out);

#endif
