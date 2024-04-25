CC = g++
CFLAGS = -std=c++17 -Wall -g
OUTPUT = main
SOURCES = src/main.cpp src/search_algorithms.cpp src/puzzle.cpp src/parser.cpp src/metrics.cpp

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)