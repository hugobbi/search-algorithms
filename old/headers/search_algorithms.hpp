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
#define NONE_STATE 0 // Used to represent the root node's father state

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

// TODO: is LIFO as third comparator respected? 
struct AStarCompare {
    bool operator()(const Node& a, const Node& b) const {
        uint af = a.g + a.h;
        uint bf = b.g + b.h;

        if (bf == af) {
            return b.h <= a.h;
        }

        return bf < af;
    }
};

struct GBFSCompare {
    bool operator()(const Node& a, const Node& b) const {
        if (b.h == a.h) {
            return b.g >= a.g;
        }

        return b.h < a.h;
    }
};

// Search algorithms
type_solution extract_path(Node* n);
type_solution bfs(const puzzle_state& start_state, Evaluation& eval);
type_solution idfs(const puzzle_state& start_state, Evaluation& eval);
type_solution dfs_limited_depth(const puzzle_state& state, const puzzle_state& father_state, uint16_t depth_limit, Evaluation& eval);
type_solution astar(const puzzle_state& start_state, Evaluation& eval);
type_solution gbfs(const puzzle_state& start_state, Evaluation& eval);
type_solution idastar(const puzzle_state& start_state, Evaluation& eval);
std::pair<uint, type_solution> recursive_search(Node& n, uint f_limit, Evaluation& eval);
