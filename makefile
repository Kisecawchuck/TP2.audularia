# Compilador e flags
CC = gcc
AR = ar

CFLAGS = -Wall -Wextra -Iinclude -fPIC # flags de warning, onde procurar os arquivos.h e geração de bibliotecas dinâmicas
PREFIX ?= /tmp/audio_lib 
LDLIBS = -lm

# Bibliotecas
STATIC_LIB = $(LIB_DIR)/libaudio.a
DYNAMIC_LIB = $(LIB_DIR)/libaudio.so

# Diretórios
SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib
BIN_DIR = bin
TEST_DIR = test
APP_DIR = app

# Mapeamento
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS)) # (Padrão ordige, Padrão destino, Lista)

# Alvos "falsos"
.PHONY: all clean run test install

# Alvos
all: $(STATIC_LIB) $(DYNAMIC_LIB) $(BIN_DIR)/main $(BIN_DIR)/test_run

# Compilação dos objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

# Biblioteca estática
$(STATIC_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	$(AR) rcs $@ $^

# Biblioteca dinâmica
$(DYNAMIC_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	$(CC) -shared $^ -o $@

$(BIN_DIR)/main: $(APP_DIR)/main.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(STATIC_LIB) $(LDLIBS) -o $@

$(BIN_DIR)/test_run: $(TEST_DIR)/test_run.c $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -rf $(BIN_DIR) $(LIB_DIR) $(BUILD_DIR)

run: $(BIN_DIR)/main
	./$(BIN_DIR)/main

test: $(BIN_DIR)/test_run
	./$(BIN_DIR)/test_run

install: 
	mkdir -p $(PREFIX)/include $(PREFIX)/lib
	cp include/*.h $(PREFIX)/include/
	cp $(LIB_DIR)/* $(PREFIX)/lib/

