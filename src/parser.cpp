#include "../headers/parser.hpp"

operation dispatcher(char* fn_name){
    if(strcmp(fn_name, "-bfs") == 0){
	return bfs;
    }else if(strcmp(fn_name, "-idfs") == 0){
	return idfs;
    }else if(strcmp(fn_name, "-astar") == 0){
	return astar;
    }else if(strcmp(fn_name, "-gbfs") == 0){
	return gbfs;
    }else if(strcmp(fn_name, "-idastar") == 0){
	return idastar;
    }
    
    std::cout << RED << "[ERROR]" << NRM << " Invalid algorithm provided\n";
    exit(-1);
}

state_args* parse_args(int argc, char* argv[]) {
    state_args* args = new state_args();
    args->state_args_size = argc-2;
    args->is_8_puzzle = args->state_args_size % 9 == 0;
    args->size_states = args->is_8_puzzle ? 9 : 16;
    args->n_states = args->state_args_size / args->size_states;

    for (int i = 0; i < args->n_states; i++) {
        for (int j = 0; j < args->size_states; j++) {
            char* number = argv[i*args->size_states + j + 2];
            if (j == args->size_states-1 && i != args->n_states-1) {
                char* comma = strrchr(number, ','); 
                *comma = '\0';
            }
            args->states[i][j] = atoi(number);
        }
    }

    return args;
}
