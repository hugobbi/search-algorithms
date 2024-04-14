#include "../headers/puzzle.hpp"

std::vector<Node> successors_8puzzle(Node& n) {
    return std::vector<Node>();
}

std::vector<type_action> extract_path(Node& n) {
    std::vector<type_action> path;
    while (n.father_node != NULL) {
        path.push_back(n.action);
        n = *n.father_node;
    }

    return path;
}