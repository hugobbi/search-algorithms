CC = g++
CFLAGS = -Wall -std=c++11
OUTPUT = main
SOURCES = src/main.cpp src/search_algorithms.cpp

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)