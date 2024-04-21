#pragma once

// Constants and functions for the 8 and 15 puzzle

#include <iostream>
#include <vector>
#include <cstdint>

#define BITS_PER_TILE 4 
#define GOAL_STATE_8 0b100001110110010101000011001000010000

// Define 8-puzzle state as 64-bit unsigned integer
// 4 bits per tile, 9 tiles, 4 bits unused
// 0 is the empty tile
typedef uint64_t puzzle_8_state;

// Define 15-puzzle state as 64-bit unsigned integer
// 4 bits per tile, 16 tiles
// 0 is the empty tile
typedef uint64_t puzzle_15_state;

// Define generic puzzle state as 64-bit unsigned integer
// 4 bits per tile, 9 or 16 tiles
// 0 is the empty tile
typedef uint64_t puzzle_state;

// Converts a 9-element array to a puzzle_state
puzzle_state array_to_8_puzzle_state(int* arr);

// Converts as 16-element array to a puzzle_state
puzzle_state array_to_15_puzzle_state(int* arr);

// Enum representing the possible actions in the tile puzzle
enum type_action {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
    NONE = 4, // No action, e.g. for the root node
    UNSOLVABLE = 5 // No solution exists
};

// Define action-state pair type
typedef std::pair<type_action, puzzle_state> action_state;

// Get all possible next states from a given state
std::vector<action_state> get_next_states_8_puzzle(puzzle_state state);
std::vector<puzzle_state> get_next_states_15_puzzle(puzzle_state state);

// Get the manhattan distance between two states
uint manhattan_distance_8_puzzle(puzzle_state state, puzzle_state goal_state);
uint manhattan_distance_15_puzzle(puzzle_state state, puzzle_state goal_state);

// Print the state of the puzzle
void print_puzzle_8_state(puzzle_state state);
void print_puzzle_15_state(puzzle_state state);

