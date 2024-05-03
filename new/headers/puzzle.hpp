#pragma once

#include <vector>
#include <stdint.h>

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