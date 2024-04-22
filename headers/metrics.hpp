#pragma once

#include <iostream>
#include <chrono>

struct Evaluation {
    uint expanded_nodes = 0;
    uint optimal_solution_length = 0;
    double time_to_solution = 0;
    std::chrono::system_clock::time_point start_time;
    double average_heuristic_value = 0;
    uint heuristic_calculation_count = 0;
    uint initial_heuristic_value = 0;
};
