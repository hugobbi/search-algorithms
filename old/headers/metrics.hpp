#pragma once

#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>
#include "../headers/printing.hpp"

struct Evaluation {
    uint expanded_nodes = 0;
    uint optimal_solution_length = 0;
    double time_to_solution = 0;
    std::chrono::system_clock::time_point start_time;
    double average_heuristic_value = 0;
    uint heuristic_calculation_count = 0;
    uint initial_heuristic_value = 0;
};

double calculate_elapsed_time(const std::chrono::system_clock::time_point& start_time);
void print_evaluation(const Evaluation& eval);
void write_evaluations(Evaluation* evals, int n_states, char* algname, bool is_8_puzzle);
std::string get_algname(char* algname);
