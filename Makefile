CC = g++
CFLAGS = -std=c++17 #-Wall -std=c++11
OUTPUT = main
SOURCES = src/main.cpp src/search_algorithms.cpp src/puzzle.cpp

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)