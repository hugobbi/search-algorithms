#pragma once
#include "puzzle.hpp"
#include <vector>

// As the tests do not require printing the whole path, we can simplify the search node
// to only store the state, action, heuristic, cost, total_cost and sequence number
typedef struct search_node_t {
    state_t state;
    action_t action;
    int heuristic;
    int cost;
    int total_cost;
    int seq;
} search_node_t;

void bfs_graph(state_t state, int& expanded_nodes, int& found_cost, int& start_heuristic);
void ids(state_t state, int& expanded_nodes, int& found_cost, int& start_heuristic);
void a_star(state_t state, int& expanded_nodes, int& found_cost, int& start_heuristic);
void a_star_15(state_t state, int& expanded_nodes, int& found_cost, int& start_heuristic);
void gbfs(state_t state, int& expanded_nodes, int& found_cost, int& start_heuristic);
void ida_star(state_t state, int& expanded_nodes, int& found_cost, int& start_heuristic);

void run_bfs_graph(state_t state);
void run_ids(state_t state);
void run_a_star(state_t state);
void run_a_star_15(state_t state);
void run_gbfs(state_t state);
void run_ida_star(state_t state);
