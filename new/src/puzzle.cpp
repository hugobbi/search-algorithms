#include "../headers/puzzle.hpp"
#include <iostream>
#include <cmath>
#include <array>

void print_state(state_t state){
    for(int i = 8; i >= 0; i--){  // Start from the last tile position and go backward
        std::cout << ((state >> (4*i)) & 0xF) << " ";
        if(i % 3 == 0){
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

void print_state_15(state_t state){
    for(int i = 15; i >= 0; i--){  // Start from the last tile position and go backward
        // print with 2 digits
        if(((state >> (4*i)) & 0xF) < 10){
            std::cout << "0";
        }
        std::cout << ((state >> (4*i)) & 0xF) << " ";
        if(i % 4 == 0){
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

int get_empty_pos(state_t state){
    int empty_pos = 0;
    for(int i = 0; i < 9; i++){
        if(((state >> (4*i)) & 0xF) == 0){
            empty_pos = 8 - i; // Reverse the index to match visual/top-to-bottom left-to-right order
            return empty_pos;
        }
    }
    return -1;
}

int get_empty_pos_15(state_t state){
    int empty_pos = 0;
    for(int i = 0; i < 16; i++){
        if(((state >> (4*i)) & 0xF) == 0){
            empty_pos = 15 - i; // Reverse the index to match visual/top-to-bottom left-to-right order
            return empty_pos;
        }
    }
    return -1;
}


static const std::array<std::vector<action_t>, 9> valid_actions_map = {
    { {RIGHT, DOWN}, {LEFT, RIGHT, DOWN}, {LEFT, DOWN}, {UP, RIGHT, DOWN}, {UP, LEFT, RIGHT, DOWN}, {UP, LEFT, DOWN}, {UP, RIGHT}, {UP, LEFT, RIGHT}, {UP, LEFT} }
};

std::vector<action_t> valid_actions(state_t state){

    int empty_pos = 0;
    for(int i = 0; i < 9; i++){
        if(((state >> (4*i)) & 0xF) == 0){
            empty_pos = 8 - i; // Reverse the index to match visual/top-to-bottom left-to-right order
            break;
        }
    }    
   return valid_actions_map[empty_pos];
}


static const std::array<std::vector<action_t>, 16> valid_actions_16_map = {
    { {RIGHT, DOWN}, {LEFT, RIGHT, DOWN}, {LEFT, RIGHT, DOWN}, {LEFT, DOWN},
      {UP, RIGHT, DOWN}, {UP, LEFT, RIGHT, DOWN}, {UP, LEFT, RIGHT, DOWN}, {UP, LEFT, DOWN},
      {UP, RIGHT, DOWN}, {UP, LEFT, RIGHT, DOWN}, {UP, LEFT, RIGHT, DOWN}, {UP, LEFT, DOWN},
      {UP, RIGHT}, {UP, LEFT, RIGHT}, {UP, LEFT, RIGHT}, {UP, LEFT} }
};
std::vector<action_t> valid_actions_15(state_t state){
    int empty_pos = 0;
    for(int i = 0; i < 16; i++){
        if(((state >> (4*i)) & 0xF) == 0){
            empty_pos = 15 - i; // Reverse the index to match visual/top-to-bottom left-to-right order
            break;
        }
    }
    return valid_actions_16_map[empty_pos];
}


void print_action(action_t action){
    switch(action){
        case UP:
            std::cout << "UP" << std::endl;
            break;
        case DOWN:
            std::cout << "DOWN" << std::endl;
            break;
        case LEFT:
            std::cout << "LEFT" << std::endl;
            break;
        case RIGHT:
            std::cout << "RIGHT" << std::endl;
            break;
        case NONE:
            std::cout << "NONE" << std::endl;
            break;
    }
}

int get_index_row(int index) {
    return index / 3;  // Dividing by number of columns
}

int get_index_col(int index) {
    return index % 3;  // Modulus by number of columns
}


int tilePosition(uint64_t state, uint64_t tile) {
    for (int i = 0; i < 9; i++) {
        if (((state >> (4 * i)) & 0xF) == tile) {
            return i;
        }
    }
    return -1;  // If tile is not found, which should not happen
}

int tilePosition_15(uint64_t state, uint64_t tile) {
    for (int i = 0; i < 16; i++) {
        if (((state >> (4 * i)) & 0xF) == tile) {
            return i;
        }
    }
    return -1;  // If tile is not found, which should not happen
}

static double total_heuristic = 0;
static double heuristic_count = 0;

int calculate_manhattan_distance(uint64_t current_state, uint64_t goal_state) {
    int distance = 0;
    for (int tile = 1; tile <= 8; tile++) {  // Assuming tiles are numbered 1 to 8
        int current_pos = tilePosition(current_state, tile);
        int goal_pos = tilePosition(goal_state, tile);
        if (current_pos == -1 || goal_pos == -1) {
            std::cerr << "Error: Tile not found" << std::endl;
            continue;
        }
        int currRow = get_index_row(current_pos);
        int currCol = get_index_col(current_pos);
        int goalRow = get_index_row(goal_pos);
        int goalCol = get_index_col(goal_pos);

        distance += std::abs(currRow - goalRow) + std::abs(currCol - goalCol);
    }
    total_heuristic += distance;
    heuristic_count++;
    return distance;
}

double average_manhattan_distance() {
    return total_heuristic / heuristic_count;
}
void reset_average_manhattan_distance(){
    total_heuristic = 0;
    heuristic_count = 0;
}

int calculate_manhattan_distance_15(uint64_t current_state, uint64_t goal_state){
    int distance = 0;
    for(int tile = 1; tile <= 15; tile++){
        int current_pos = tilePosition_15(current_state, tile);
        int goal_pos = tilePosition_15(goal_state, tile);
        if(current_pos == -1 || goal_pos == -1){
            std::cerr << "Error: Tile not found" << std::endl;
            continue;
        }
        int currRow = current_pos/4;
        int currCol = current_pos%4;
        int goalRow = (goal_pos/4);
        int goalCol = (goal_pos%4);

        distance += std::abs(currRow - goalRow) + std::abs(currCol - goalCol);
    }
    total_heuristic += distance;
    heuristic_count++;
    return distance;
}

static const std::array<int, 4> action_offset = { -3, 3, -1, 1 };
uint64_t apply_action(uint64_t state, action_t action) {
    int empty_pos = get_empty_pos(state);
    int target_pos = empty_pos + action_offset[action]; 
    /**
    switch (action) {
        case UP:
            target_pos = empty_pos - 3;
            break;
        case DOWN:
            target_pos = empty_pos + 3;
            break;
        case LEFT:
            target_pos = empty_pos - 1;
            break;
        case RIGHT:
            target_pos = empty_pos + 1;
            break;
        default:
            return state;
    }
    **/
    int empty_index = 8 - empty_pos;
    int target_index = 8 - target_pos;

    uint64_t empty_tile = (state >> (4 * empty_index)) & 0xF;
    uint64_t target_tile = (state >> (4 * target_index)) & 0xF;

    // Swap the tiles
    state &= ~((uint64_t)0xF << (4 * empty_index));
    state &= ~((uint64_t)0xF << (4 * target_index));
    state |= (empty_tile << (4 * target_index));
    state |= (target_tile << (4 * empty_index));

    return state;
}

static const std::array<int, 4> action_offset_15 = { -4, 4, -1, 1 };
uint64_t apply_action_15(uint64_t state, action_t action){
    int empty_pos = get_empty_pos_15(state);
    int target_pos = empty_pos + action_offset_15[action];

    int empty_index = 15 - empty_pos;
    int target_index = 15 - target_pos;

    uint64_t empty_tile = (state >> (4 * empty_index)) & 0xF;
    uint64_t target_tile = (state >> (4 * target_index)) & 0xF;

    // Swap the tiles
    state &= ~((uint64_t)0xF << (4 * empty_index));
    state &= ~((uint64_t)0xF << (4 * target_index));
    state |= (empty_tile << (4 * target_index));
    state |= (target_tile << (4 * empty_index));

    return state;
}