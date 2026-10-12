# Compilador e flags
CC = gcc # Compilador do projeto
CFLAGS = -Wall -Wextra -Iinclude -fPIC # flags de warning, onde procurar os arquivos.h e geração de bibliotecas dinâmicas
PREFIX ?= /tmp/audio_lib 
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
OBJS = $(patsubst $(SRC_DIR)/*.c, $(BUILD_DIR)/*.o, $(SRCS)) # (Padrão ordige, Padrão destino, Lista)

# Alvos "falsos"
.PHONY: all clean install run test

# Alvos
all: $(STATIC_LIB) $(DYNAMIC_LIB) $(BIN_DIR)/main_app $(BIN_DIR)/test_run

# Compilação incremental
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(STATIC_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(DYNAMIC_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	$(CC) -shared $^ -o $@

$(BIN_DIR)/main_app: $(APP_DIR)/main_app.c $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/test_run: $(TEST_DIR)/test_run.c $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BIN_DIR) $(LIB_DIR) $(BUILD_DIR)

run: $(BIN_DIR)/app_main
	./$(BIN_DIR)/app_main

test: $(BIN_DIR)/test_run
	./$(BIN_DIR)/test_run

install: 
	mkdir -p $(PREFIX)/include $(PREFIX)/lib
	cp include/*.h $(PREFIX)/include/
	cp $(LIB_DIR)/* $(PREFIX)/lib/

