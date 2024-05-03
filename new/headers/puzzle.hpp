#pragma once

#include <vector>
#include <stdint.h>

/**
 * The state is represented as a 64-bit integer where each 4 bits represent a tile.
 * Such that the state 0x012345678 represents the following board:
 * 0 1 2
 * 3 4 5
 * 6 7 8
 * 
 * If the state is 0x0123456789abcdef, the board is:
 * 00 01 02 03
 * 04 05 06 07
 * 08 09 10 11
 * 12 13 14 15
 * 
 * This has the neat property that the hex representation of the state is the same as the board.
*/
typedef uint64_t state_t;
#define GOAL_STATE 0x012345678
#define GOAL_STATE_15 0x0123456789abcdef

typedef enum action_t {UP, DOWN, LEFT, RIGHT, NONE} action_t;

const action_t opposite_action[] = {DOWN, UP, RIGHT, LEFT, NONE};

#define get_opposite_action(action) opposite_action[action]


void print_state(state_t state);
std::vector<action_t> valid_actions(state_t state);
std::vector<action_t> valid_actions_15(state_t state);
uint64_t apply_action(uint64_t state, action_t action);
uint64_t apply_action_15(uint64_t state, action_t action);
void print_action(action_t action);
int calculate_manhattan_distance(uint64_t current_state, uint64_t goal_state);
int calculate_manhattan_distance_15(uint64_t current_state, uint64_t goal_state);
double average_manhattan_distance();
void reset_average_manhattan_distance();