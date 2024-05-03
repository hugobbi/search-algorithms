#pragma once

#include <string.h> // strcmp is slightly faster
#include <stdlib.h>
#include <fstream>
#include "../headers/puzzle.hpp"
#include "../headers/search_algorithms.hpp"

#define MAX_STATES 100

typedef type_solution(*operation)(const puzzle_state&, Evaluation&);

struct state_args {
    int state_args_size;
    bool is_8_puzzle;
    int size_states;
    int n_states;
    int states[MAX_STATES][16];
};

operation dispatcher(char* fn_name);
state_args* parse_args(int argc, char* argv[]);
