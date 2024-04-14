#pragma once

// Constants and functions for the 8 and 15 puzzle

#include <iostream>
#include <vector>

#define BITS_PER_TILE 4 
#define GOAL_STATE_8 0b100001110110010101000011001000010000
#define NO_SOLUTION -1

typedef unsigned long long puzzle_state;
typedef int type_action;
struct Node {
    Node* father_node;
    puzzle_state state;
    type_action action; // still needs to define what an action is exactly: struct{direction, tile (2 bits, 4 bits)}
};
struct ActionState {
    type_action action;
    puzzle_state state;
};

std::vector<ActionState> successors_8puzzle(puzzle_state& n);
std::vector<type_action> extract_path(Node& n);