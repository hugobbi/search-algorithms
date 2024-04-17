#pragma once

// Constants and functions for the 8 and 15 puzzle

#include <iostream>
#include <vector>

#define BITS_PER_TILE 4 
#define GOAL_STATE_8 0b100001110110010101000011001000010000

typedef unsigned long long puzzle_state;
typedef int type_action;

struct ActionState {
    type_action action;
    puzzle_state state;
};

std::vector<ActionState> successors_8puzzle(puzzle_state& n);
