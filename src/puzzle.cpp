#include "../headers/puzzle.hpp"
#include <array>

// Converts a 9-element array to a puzzle_state
puzzle_state array_to_8_puzzle_state(int* arr){
    puzzle_state state = 0;
    for(int i = 0; i < 9; i++){
    state |= (puzzle_state)arr[i] << (i * BITS_PER_TILE);
    }
    return state;
}

// Converts as 16-element array to a puzzle_state
puzzle_state array_to_15_puzzle_state(int* arr){
    puzzle_state state = 0;
    for(int i = 0; i < 16; i++){
    state |= (puzzle_state)arr[i] << (i * BITS_PER_TILE);
    }
    return state;
}


// Helper function to swap tiles
puzzle_state swap_tiles(puzzle_state state, int tile1, int tile2){
    int shift1 = tile1 * BITS_PER_TILE;
    int shift2 = tile2 * BITS_PER_TILE;
    uint64_t value1 = (state >> shift1) & static_cast<uint64_t>(0xF);
    uint64_t value2 = (state >> shift2) & static_cast<uint64_t>(0xF);

    // Clear the bits at both positions
    // Basically: move 1111 to the right position and negate it to make it 0000 only at that position
    // But do that for each tile we are swapping
    // ALSO we need to use the static_cast or we will have an overflow and everything will be wrong and we will cry
    state &= ~((static_cast<uint64_t>(0xF) << shift1) | (static_cast<uint64_t>(0xF) << shift2));

    // Set the new values
    // just move the thing to the right position and set it.
    state |= (value1 << shift2) | (value2 << shift1);
    return state;
}

// Get all possible next states from a given state
std::vector<action_state> get_next_states_8_puzzle(const puzzle_state& state, const puzzle_state& father_state){ // , puzzle_state father_state
    std::vector<action_state> action_state_pairs;
    int empty_tile = -1;

    // Find the empty tile
    for(int i = 0; i < 9; i++){
        if(((state >> (i * BITS_PER_TILE)) & 0xF) == 0){
            empty_tile = i;
            break;
        }
    }

    if(empty_tile == -1){
        return action_state_pairs;  // No empty tile found, return empty vector
    }

    // Define movement and boundary checks      // Obs: it doesn't look like it is expanding in the right order
    std::array<int, 4>  moves = {-3, -1, 1, 3}; // up, left, right, down
    std::array<bool, 4> can_move = {
        empty_tile > 2,           // Can move up
        empty_tile % 3 != 0,      // Can move left
        empty_tile % 3 != 2,      // Can move right
        empty_tile < 6            // Can move down
    };

    // Generate new states for valid moves
    for (int i = 0; i < 4; ++i) {
        if (can_move[i]) {
            puzzle_state new_state = swap_tiles(state, empty_tile, empty_tile + moves[i]);
            if (new_state != father_state) {
                action_state_pairs.push_back(action_state{static_cast<type_action>(i), new_state});
            }
        }
    }

    return action_state_pairs;
}

std::vector<puzzle_state> get_next_states_15_puzzle(puzzle_state state){
    // Fixing some bugs (':
    std::vector<puzzle_state> next_states;
    return next_states;
}

// Get the manhattan distance between two states
uint manhattan_distance_8_puzzle(const puzzle_state& state, const puzzle_state& goal_state, Evaluation& eval){
    // TODO: determine if state is solvable
    uint distance = 0;
    for(int i = 0; i < 9; i++){
        uint tile = (state >> (i * BITS_PER_TILE)) & 0xF;
        if(tile == 0){
            continue;
        }
        for(int j = 0; j < 9; j++){
            if(((goal_state >> (j * BITS_PER_TILE)) & 0xF) == tile){
                distance += abs(i % 3 - j % 3) + abs(i / 3 - j / 3);
                break;
            }
        }
    }

    eval.average_heuristic_value += distance;
    eval.heuristic_calculation_count++;

    return distance;
}

uint manhattan_distance_15_puzzle(puzzle_state state, puzzle_state goal_state){
    uint distance = 0;
    for(int i = 0; i < 16; i++){
        uint tile = (state >> (i * BITS_PER_TILE)) & 0xF;
        if(tile == 0){
            continue;
        }
        for(int j = 0; j < 16; j++){
            if(((goal_state >> (j * BITS_PER_TILE)) & 0xF) == tile){
                distance += abs(i % 4 - j % 4) + abs(i / 4 - j / 4);
                break;
            }
        }
    }
    return distance;
}

// Print the state of the puzzle
void print_puzzle_8_state(const puzzle_state& state){
    for(int i = 0; i < 9; i++){
        std::cout << ((state >> (i * BITS_PER_TILE)) & 0xF) << " ";
        if(i % 3 == 2){
            std::cout << std::endl;
        }
    }
}
void print_puzzle_15_state(puzzle_state state){
    for(int i = 0; i < 16; i++){
        std::cout << ((state >> (i * BITS_PER_TILE)) & 0xF) << " ";
        if(i % 4 == 3){
            std::cout << std::endl;
        }
    }
}
