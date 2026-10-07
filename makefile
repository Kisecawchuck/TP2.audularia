# Compilador e flags
CC = gcc # Compilador do projeto
CFLAGS = -Wall -Wextra -Iinclude -fPIC # flags de warning, onde procurar os arquivos.h e geração de bibliotecas dinâmicas

# Diretórios
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
BIN_DIR = bin
TEST_DIR = test
APP_DIR = app

# Mapeamento
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/*.c, $(BUILD_DIR)/*.o, $(SRCS)) # (Padrão ordige, Padrão destino, Lista)

# Alvos "falsos"
.PHONY: all clean install run test

# Alvos


all : $(BIN_DIR)/main_app $(BIN_DIR)/test_run

# Compilação incremental
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(cc) $(CFLAGS) -c $< -o $@ 

$(BIN_DIR)/main_app : $(BUILD_DIR)/%.o	 

