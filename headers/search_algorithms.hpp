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
#define NO_SOLUTION -1

struct Node {
    Node* father_node;
    puzzle_state state;
    type_action action; // TODO: still needs to define what an action is exactly: struct{direction, tile (2 bits, 4 bits)}
    // Used in heuristic search (TODO: should another node struct be created for astar and gbfs?)
    //uint f; // possible optimization: use g and h to get f
    uint g;
    uint h;
};

struct FLimitSolution {
    uint f_limit;
    std::vector<type_action> solution;
};

std::vector<type_action> extract_path(Node& n);

std::vector<type_action> bfs(puzzle_state start_state);
std::vector<type_action> dfs_limited_depth(puzzle_state state, uint16_t depth_limit);
std::vector<type_action> id_dfs(puzzle_state start_state, uint16_t max_depth);
std::vector<type_action> a_star(puzzle_state start_state);
std::vector<type_action> id_a_star(puzzle_state start_state);
FLimitSolution recursive_search(Node n, uint f_limit);
std::vector<type_action> gbfs(puzzle_state start_state);

uint manhattan_distance(puzzle_state state);

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