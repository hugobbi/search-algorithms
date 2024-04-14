#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include "../headers/search_algorithms.hpp"
#include "../headers/puzzle.hpp"

std::vector<type_action> bfs(puzzle_state start_state) {
    if (start_state == GOAL_STATE_8) {
        return std::vector<type_action>();
    }

    Node starting_node = {NULL, start_state, 0};
    std::queue<Node> open;
    open.push(starting_node);
    std::set<puzzle_state> closed;
    closed.insert(start_state);

    while (!open.empty()) {
        Node n = open.front();
        open.pop();
        for (Node succ : successors_8puzzle(n)) { // a possible optimization is to only calculate new node after checking if previous one is goal
            if (succ.state == GOAL_STATE_8) {
                return extract_path(succ);
            }
            else {
                if (closed.find(succ.state) == closed.end()) {
                    open.push(succ);
                    closed.insert(succ.state);
                }
            }
        }
    }

    return std::vector<type_action>(-1); // unsolvable
}