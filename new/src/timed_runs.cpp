#include "../headers/search.hpp"
#include "../headers/puzzle.hpp"

#include <iostream>
#include <chrono>


void run_bfs_graph(state_t state){
    auto start = std::chrono::high_resolution_clock::now();
    int expanded_nodes = 0, found_cost = 0, start_heuristic = 0;
    bfs_graph(state, expanded_nodes, found_cost, start_heuristic);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << expanded_nodes << "," << found_cost << "," << duration.count() << "," << 0 << "," << start_heuristic << std::endl;
    reset_average_manhattan_distance();
}

void run_ids(state_t state){
    auto start = std::chrono::high_resolution_clock::now();
    int expanded_nodes = 0, found_cost = 0, start_heuristic = 0;
    ids(state, expanded_nodes, found_cost, start_heuristic);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << expanded_nodes << "," << found_cost << "," << duration.count() << "," << 0 << "," << start_heuristic << std::endl;
    reset_average_manhattan_distance();
}


void run_a_star(state_t state){
    auto start = std::chrono::high_resolution_clock::now();
    int expanded_nodes = 0, found_cost = 0, start_heuristic = 0;
    a_star(state, expanded_nodes, found_cost, start_heuristic);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << expanded_nodes << "," << found_cost << "," << duration.count() << "," << average_manhattan_distance() << "," << start_heuristic << std::endl;
    reset_average_manhattan_distance();
}

void run_a_star_15(state_t state){
    auto start = std::chrono::high_resolution_clock::now();
    int expanded_nodes = 0, found_cost = 0, start_heuristic = 0;
    a_star_15(state, expanded_nodes, found_cost, start_heuristic);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << expanded_nodes << "," << found_cost << "," << duration.count() << "," << average_manhattan_distance() << "," << start_heuristic << std::endl;
    reset_average_manhattan_distance();
}

void run_gbfs(state_t state){
    auto start = std::chrono::high_resolution_clock::now();
    int expanded_nodes = 0, found_cost = 0, start_heuristic = 0;
    gbfs(state, expanded_nodes, found_cost, start_heuristic);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << expanded_nodes << "," << found_cost << "," << duration.count() << "," << average_manhattan_distance() << "," << start_heuristic << std::endl;
    reset_average_manhattan_distance();
}

void run_ida_star(state_t state){
    auto start = std::chrono::high_resolution_clock::now();
    int expanded_nodes = 0, found_cost = 0, start_heuristic = 0;
    ida_star(state, expanded_nodes, found_cost, start_heuristic);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << expanded_nodes << "," << found_cost << "," << duration.count() << "," << average_manhattan_distance() << "," << start_heuristic << std::endl;
    reset_average_manhattan_distance();
}

