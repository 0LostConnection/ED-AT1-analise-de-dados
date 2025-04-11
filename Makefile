# MAKEFILE compatível com Linux e Windows

# Definição de variáveis
CC = gcc
CFLAGS = -Wall -I./include
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
DATA_DIR = data

# Detecta o sistema operacional
ifeq ($(OS),Windows_NT)
    RM = del /Q
    MKDIR = mkdir
    TARGET = $(BIN_DIR)/programa.exe
    EXECUTABLE = programa.exe
else
    RM = rm -f
    MKDIR = mkdir -p
    TARGET = $(BIN_DIR)/programa.out
    EXECUTABLE = programa.out
endif

# Arquivos de origem
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/processo.c
OBJS = $(SRCS:.c=.o)

# Regra principal
all: $(TARGET)

# Compilação do programa
$(TARGET): $(OBJS)
	@$(MKDIR) $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "Programa compilado com sucesso!"

# Compilação dos arquivos .c para .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Executar o programa
run: $(TARGET)
	@cd $(BIN_DIR) && ./$(EXECUTABLE)

# Limpar arquivos compilados
clean:
	@$(RM) $(SRC_DIR)/*.o
	@echo "Arquivos objeto removidos."

# Limpar tudo (arquivos objeto e executável)
cleanall: clean
	@$(RM) $(TARGET)
	@echo "Executável removido."

# Regra para recompilar tudo
rebuild: cleanall all

# Exibir informações do projeto
info:
	@echo "Estrutura do Projeto:"
	@echo "- Pasta bin: Contém o executável ($(EXECUTABLE))"
	@echo "- Pasta data: Contém os dados (arquivo.csv)"
	@echo "- Pasta include: Contém os cabeçalhos (processo.h)"
	@echo "- Pasta src: Contém os códigos-fonte (main.c, processo.c)"

# Makefile antigo. Remover após confirmar que o novo funciona no Windows também.

# # Definição de variáveis
# CC = gcc
# CFLAGS = -Wall -I./include
# SRC_DIR = src
# INC_DIR = include
# BIN_DIR = bin
# DATA_DIR = data

# # Arquivos de origem
# SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/processo.c
# OBJS = $(SRCS:.c=.o)
# TARGET = $(BIN_DIR)/programa.exe

# # Regra principal
# all: $(TARGET)

# # Compilação do programa
# $(TARGET): $(OBJS)
# 	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
# 	$(CC) $(CFLAGS) $^ -o $@
# 	@echo Programa compilado com sucesso!

# # Compilação dos arquivos .c para .o
# $(SRC_DIR)/%.o: $(SRC_DIR)/%.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Executar o programa
# run: $(TARGET)
# 	@cd $(BIN_DIR) && programa.exe

# # Limpar arquivos compilados
# clean:
# 	@del /Q $(SRC_DIR)\*.o
# 	@echo Arquivos objeto removidos.

# # Limpar tudo (arquivos objeto e executável)
# cleanall: clean
# 	@del /Q $(BIN_DIR)\programa.exe
# 	@echo Executável removido.
# # Regra para recompilar tudo
# rebuild: cleanall all

# # Exibir informações do projeto
# info:
# 	@echo Estrutura do Projeto:
# 	@echo - Pasta bin: Contém o executável (programa.exe)
# 	@echo - Pasta data: Contém os dados (arquivo.csv)
# 	@echo - Pasta include: Contém os cabeçalhos (processo.h)
# 	@echo - Pasta src: Contém os códigos-fonte (main.c, processo.c)