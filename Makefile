# Makefile para compilar um programa C++

# Nome do compilador
CC = g++

# Opções de compilação
CFLAGS = -Wall -std=c++11

# Nome do arquivo de saída
OUTPUT = main

# Lista de arquivos fonte
SOURCES = src/main.cpp src/search_algorithms.cpp

# Comando para compilar o programa
all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUTPUT)

# Comando para limpar os arquivos de compilação
clean:
	rm -f $(OUTPUT)