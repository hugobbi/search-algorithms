#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_map>
#include <algorithm>
#include "puzzle.hpp"

#define H_INFINITY 1000000
#define F_INFINITY 1000000
#define F_LIMIT_NONE 10000000

// Define solution type
typedef std::vector<type_action> type_solution; 

struct Node {
    Node* father_node;
    puzzle_8_state state;
    type_action action;
    // Used in heuristic search (TODO: should another node struct be created for astar and gbfs?)
    //uint f; // possible optimization: use g and h to get f
    uint g;
    uint h;
};

type_solution extract_path(Node& n);

type_solution bfs(puzzle_8_state start_state);
type_solution dfs_limited_depth(puzzle_8_state state, uint16_t depth_limit);
type_solution id_dfs(puzzle_8_state start_state, uint16_t max_depth);
type_solution a_star(puzzle_8_state start_state);
type_solution id_a_star(puzzle_8_state start_state);
std::pair<uint, type_solution> recursive_search(Node n, uint f_limit);
type_solution gbfs(puzzle_8_state start_state);

// TODO: how to implement LIFO as third comparator?
struct AStarCompare {
    bool operator()(Node& a, Node& b) const {
        uint af = a.g + a.h;
        uint bf = b.g + b.h;
        if (af == bf) {
            return a.h < b.h;
        }

        return af < bf;
    }
};

struct GBFSCompare {
    bool operator()(Node& a, Node& b) const {
        if (a.h == b.h) {
            return a.g > b.g;
        }

        return a.h < b.h;
    }
};