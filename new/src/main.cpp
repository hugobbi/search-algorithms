#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>



#include "../headers/search.hpp"
#include "../headers/puzzle.hpp"

bool contains_comma(const std::string& str) {
    return str.find(',') != std::string::npos;
}

int main(int argc, char* argv[]) {
    // Check the minimum number of arguments (at least one algorithm and one state)
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " -algorithm state1, state2, ..." << std::endl;
        return 1;
    }

    // First argument should be the algorithm
    std::string algorithm = argv[1];
    std::vector<state_t> states;
    int count = 0;
    bool is_15 = false;
    state_t current_state = 0;
    // Parse each subsequent argument as a state
    for (int i = 2; i < argc; ++i) {
        std::string state_str(argv[i]);

        // If there is a comma in the string, this is the last number of the state
        if(contains_comma(state_str)) {
            std::cout << count << std::endl;
            if(count == 15) {
                is_15 = true;
            }
            // Remove the comma
            state_str.pop_back();
            // Parse the number
            current_state = (current_state << (state_t)4) + (state_t) std::stoi(state_str);
            // Add the number to the state
            states.push_back(current_state);
            // Reset the current state
            current_state = 0;
            count = 0;
        } else {
            // Parse the number
            current_state = (current_state << 4) + std::stoi(state_str);
            count++;
            if(count == 15) {
                is_15 = true;
            }
        }
    }
    states.push_back(current_state);

    if(algorithm == "-bfs_graph") {
        for(state_t state : states) {
            run_bfs_graph(state);
        }
    } else if(algorithm == "-ids") {
        for(state_t state : states) {
            run_ids(state);
        }
    } else if(algorithm == "-a_star") {
        for(state_t state : states) {
            if(is_15) {
                run_a_star_15(state);
            } else {
                run_a_star(state);
            }
        }
    } else if(algorithm == "-a_star_15") {
        for(state_t state : states) {
            run_a_star_15(state);
        }
    } else if(algorithm == "-gbfs") {
        for(state_t state : states) {
            run_gbfs(state);
        }
    } else if(algorithm == "-ida_star") {
        for(state_t state : states) {
            run_ida_star(state);
        }
    } else {
        std::cerr << "Invalid algorithm: " << algorithm << std::endl;
        return 1;
    }

    return 0;
}