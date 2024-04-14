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
        for (ActionState succ_action_state : successors_8puzzle(n.state)) { // a possible optimization is to only calculate new action/state after checking if previous state is goal
            Node succ = Node{&n, succ_action_state.state, succ_action_state.action};
            if (succ.state == GOAL_STATE_8) {
                return extract_path(succ);
            }
            if (closed.find(succ.state) == closed.end()) {
                open.push(succ);
                closed.insert(succ.state);
            }
        }
    }

    return std::vector<type_action>(NO_SOLUTION); // unsolvable
}

std::vector<type_action> dfs_limited_depth(puzzle_state state, uint16_t depth_limit) {
    if (state == GOAL_STATE_8) {
        return std::vector<type_action>();
    }

    if (depth_limit != 0) {
        for (ActionState action_state : successors_8puzzle(state)) {
            std::vector<type_action> path = dfs_limited_depth(action_state.state, depth_limit-1);

            if (path.front() != NO_SOLUTION) {
                path.push_back(action_state.action);
                return path;
            }
        }
    }

    return std::vector<type_action>(NO_SOLUTION); // no solution in this branch with this depth limite
}

std::vector<type_action> id_dfs(puzzle_state start_state, uint16_t max_depth) {
    for (uint16_t depth_limit = 0; depth_limit < max_depth; depth_limit++) {
        std::vector<type_action> path = dfs_limited_depth(start_state, depth_limit);
        if (path.front() != NO_SOLUTION) {
            return path;
        }
    }

    return std::vector<type_action>(NO_SOLUTION); // unsolvable with this max depth
}