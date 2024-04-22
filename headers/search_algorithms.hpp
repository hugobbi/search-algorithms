#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <algorithm>
#include "puzzle.hpp"
#include "metrics.hpp"

#define H_INFINITY 1000000
#define F_INFINITY 1000000
#define F_LIMIT_NONE 10000000
#define MAX_DEPTH 1000

// Define solution type
typedef std::vector<type_action> type_solution; 

struct Node {
    Node* father_node;
    puzzle_state state;
    type_action action;
    // Used in heuristic search (TODO: should another node struct be created for astar and gbfs?)
    //uint f; // possible optimization: use g and h to get f
    uint g = 0;
    uint h = 0;
};

// TODO: how to implement LIFO as third comparator? If node a is already present, then b 
// will only be swapped if it is strictly better. I think this is right and it solves LIFO
// as third comparator 
struct AStarCompare {
    bool operator()(const Node& a, const Node& b) const {
        uint af = a.g + a.h;
        uint bf = b.g + b.h;

        if (af == bf) {
            return b.h < a.h;
        }

        return bf < af;
    }
};

struct GBFSCompare {
    bool operator()(const Node& a, const Node& b) const {
        if (a.h == b.h) {
            return b.g < a.g;
        }

        return b.h < a.h;
    }
};

type_solution extract_path(Node* n);
type_solution bfs(puzzle_state start_state, Evaluation& eval);
type_solution dfs_limited_depth(puzzle_state state, uint16_t depth_limit, Evaluation& eval);
type_solution idfs(puzzle_state start_state, Evaluation& eval);
type_solution astar(puzzle_state start_state, Evaluation& eval);
type_solution idastar(puzzle_state start_state, Evaluation& eval);
std::pair<uint, type_solution> recursive_search(Node n, uint f_limit, Evaluation& eval);
type_solution gbfs(puzzle_state start_state, Evaluation& eval);
double calculate_elapsed_time(std::chrono::system_clock::time_point start_time);
void print_evaluation(const Evaluation& eval);
