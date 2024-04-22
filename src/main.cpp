#include <iostream>
#include "../headers/puzzle.hpp"
#include "../headers/search_algorithms.hpp"
#include "../headers/parser.hpp"

int main(int argc, char* argv[]){
    // Check if algorithm was provided
    if(argc < 2){
	std::cout << RED << "[ERROR]" << NRM << " Wrong input!\n";
	return -1;
    }

    state_args* args = parse_args(argc, argv);
    Evaluation* evaluations = new Evaluation[args->n_states]; // Used to evaluate each algorithm execution
    type_solution solutions[args->n_states];

    // Dispatch algorithm based on provided arg
    for (int i = 0; i < args->n_states; i++) {
        puzzle_state state;
        if (args->is_8_puzzle) {
            state = array_to_8_puzzle_state(args->states[i]);
            print_puzzle_8_state(state);
        } else {
            state = array_to_15_puzzle_state(args->states[i]);
            print_puzzle_15_state(state);
        }

        solutions[i] = dispatcher(argv[1])(state, evaluations[i]);
        evaluations[i].time_to_solution = calculate_elapsed_time(evaluations[i].start_time);
        evaluations[i].optimal_solution_length = solutions[i].size()-1; // -1 because the first action is NONE
        print_evaluation(evaluations[i]);
    }

    delete[] args;

    return 0;
}
