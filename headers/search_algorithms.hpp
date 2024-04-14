#pragma once

#include "puzzle.hpp"

std::vector<type_action> bfs(puzzle_state start_state);
std::vector<type_action> dfs_limited_depth(puzzle_state state, uint16_t depth_limit);
std::vector<type_action> id_dfs(puzzle_state start_state, uint16_t max_depth);
std::vector<type_action> a_star(puzzle_state start_state);
std::vector<type_action> id_a_star(puzzle_state start_state);
std::vector<type_action> gbfs(puzzle_state start_state);