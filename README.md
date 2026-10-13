# Processamento de Áudio em C - Trabalho 2

Projeto desenvolvido para a disciplina de Programação, focado em processamento e manipulação de arquivos de áudio em linguagem C.

## 📁 Estrutura do Projeto

- `include/`: Arquivos de cabeçalho (.h) da API pública.
- `src/`: Implementação dos módulos (.c).
- `app/`: Aplicação principal/cliente (`main.c`).
- `tests/`: Suíte de testes automatizados.
- `lib/`: Bibliotecas geradas (`.a` estática e `.so` dinâmica).
- `bin/`: Executáveis compilados.

## 🛠️ Pré-requisitos

- Compilador `gcc`
- Utilitário `make`
- `ffmpeg` (para reprodução/suporte a formatos de áudio)

## 🚀 Uso da ferramento com o makefile

O projeto utiliza um `Makefile` configurado para compilação incremental.

- **Compila a biblioteca e a acplicação:**
  ```bash
  # O "all" é opcional
  make all 

- **Remover os artefatos**
  ```bash
  make clean

- **Compila e executa os testes**
  ```bash
  make test

- **Compila (caso necessário) e executa a aplicação**
  ```bash
  make run
  
- **Instala a biblioteca e os arquivos de interface**
  ```bash
  make install