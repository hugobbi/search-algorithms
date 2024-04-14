#include <iostream>
#include <string.h> // strcmp is slightly faster
#include <stdlib.h>
// TTY control codes for pretty printing (:
#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// TODO SECTION
int convert_to_state(char *state[], int size){
    for(int i = 0; i < size; i++){
	std::cout << state[i] << "\n";
    }
    return 0;
}

void bfs(int start_state) { std::cout << "bfs\n"; }
void dfs(int start_state) { std::cout << "dfs\n"; }

typedef void(*operation)(int);

operation dispatcher(char* fn_name){
    if(strcmp(fn_name, "bfs") == 0){
	return bfs;
    }else if(strcmp(fn_name, "dfs") == 0){
	return dfs;
    }
    exit(-1);
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

int main(int argc, char* argv[]){
    // Check if algorithm was provided
    if(argc < 2){
	std::cout << RED << "[ERROR]" << NRM << " No algorithm provided\n";
	return -1;
    }

    // (weak) check if the initial state is valid    
    if(argc != 11 && argc != 18){
	std::cout << RED << "[ERROR]" << NRM <<" Please provide a valid [9 or 16 numbers long] initial state\n";
	return -1;
    }

    char* state_args[16];
    int size_states = argc-2;
    for(int i = 0; i < size_states; i++){
	state_args[i] = argv[i+2];
    }
    int state = convert_to_state(state_args, size_states);

    // Dispatch algorithm based on provided arg
    dispatcher(argv[1])(state);
    return 0;
}
