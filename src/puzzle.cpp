#include "../headers/puzzle.hpp"

std::vector<ActionState> successors_8puzzle(puzzle_state& state) {
    return std::vector<ActionState>();
}

std::vector<type_action> extract_path(Node& n) {
    std::vector<type_action> path;
    while (n.father_node != NULL) {
        path.push_back(n.action);
        n = *n.father_node;
    }

    return path;
}