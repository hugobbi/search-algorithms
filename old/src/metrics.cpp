#include "../headers/metrics.hpp"

double calculate_elapsed_time(const std::chrono::system_clock::time_point& start_time) {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count() / 1000000.0;
}

void print_evaluation(const Evaluation& eval) {
    // std::cout << "Expanded nodes: " << eval.expanded_nodes << std::endl;
    // std::cout << "Optimal solution length: " << eval.optimal_solution_length << std::endl;
    // std::cout << "Time to solution: " << eval.time_to_solution << std::endl;
    // std::cout << "Average heuristic value: " << eval.average_heuristic_value << std::endl;
    // std::cout << "Initial heuristic value: " << eval.initial_heuristic_value << std::endl;

    std::cout << eval.expanded_nodes << ",";
    std::cout << eval.optimal_solution_length << ",";
    std::cout << eval.time_to_solution << ",";
    std::cout << eval.average_heuristic_value << ",";
    std::cout << eval.initial_heuristic_value << std::endl;
}

void write_evaluations(Evaluation* evals, int n_states, char* algname, bool is_8_puzzle) {
    std::filesystem::path dir("results/");

    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir);
    }

    std::ofstream out_file;
    std::string puzzle_type = is_8_puzzle ? "8puzzle" : "15puzzle";
    std::string filename = "results/" + std::string(get_algname(algname)) + puzzle_type + ".csv";

    out_file.open(filename, std::ios::app | std::ios::out);
    
    if (!out_file.is_open()) {
        std::cout << RED << "[ERROR]" << NRM << " Could not open file " << filename << std::endl;
        exit(-1);
    }
    
    for (int i = 0; i < n_states; i++) {
        out_file << evals[i].expanded_nodes << ",";
        out_file << evals[i].optimal_solution_length << ",";
        out_file << evals[i].time_to_solution << ",";
        out_file << evals[i].average_heuristic_value << ",";
        out_file << evals[i].initial_heuristic_value << std::endl;
    }  
}

std::string get_algname(char* algname) {
    std::string name = std::string(algname);
    return name.substr(1);
}
