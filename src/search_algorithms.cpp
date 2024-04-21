#include "../headers/search_algorithms.hpp"
#include "../headers/puzzle.hpp"

solution extract_path(Node& n) {
    solution path;
    while (n.father_node != NULL) {
        path.push_back(n.action);
        n = *n.father_node;
    }

    return path;
}

solution bfs(puzzle_8_state start_state) {
    // Early goal test
    if (start_state == GOAL_STATE_8) {
        return solution();
    }

    // Initializing open and closed
    Node starting_node = {NULL, start_state, type_action::NONE};
    std::queue<Node> open;
    open.push(starting_node);
    std::set<puzzle_8_state> closed;
    closed.insert(start_state);

    // Main search loop
    while (!open.empty()) {
        Node n = open.front();
        open.pop();
        for (const auto &[action, new_state] : get_next_states_8_puzzle(n.state)) { // A possible optimization is to only calculate new action/state after checking if previous state is goal
            Node succ = Node{&n, new_state, action};
            if (succ.state == GOAL_STATE_8) {  // Early goal test
                return extract_path(succ);
            }
            if (closed.find(succ.state) == closed.end()) { // Insert into open if nodes not in closed
                open.push(succ);
                closed.insert(succ.state);
            }
        }
    }

    return solution{NO_SOLUTION}; // Unsolvable
}

solution dfs_limited_depth(puzzle_8_state state, uint16_t depth_limit) {
    // Early goal test
    if (state == GOAL_STATE_8) {
        return solution();
    }

    // Expands nodes if depth limit is not reached
    if (depth_limit != 0) {
        for (const auto &[action, new_state] : get_next_states_8_puzzle(state)) {
            solution path = dfs_limited_depth(new_state, depth_limit-1); // Call dfs on child node

            // If a solution is found, return the path
            if (path.front() != NO_SOLUTION) {
                path.push_back(action);
                return path;
            }
        }
    }

    return solution{NO_SOLUTION}; // No solution in this branch with this depth limite
}

solution id_dfs(puzzle_8_state start_state, uint16_t max_depth) {
    // Call dfs with limited depth for each depth limit
    for (uint16_t depth_limit = 0; depth_limit < max_depth; depth_limit++) {
        solution path = dfs_limited_depth(start_state, depth_limit);
        // If a solution is found, return the path
        if (path.front() != NO_SOLUTION) {
            return path;
        }
    }

    return solution{NO_SOLUTION}; // Unsolvable with this max depth
}

solution a_star(puzzle_8_state start_state) {
    // Initializing open
    std::priority_queue<Node, std::vector<Node>, AStarCompare> open;
    uint start_h = manhattan_distance_8_puzzle(start_state, GOAL_STATE_8);
    if (start_h < H_INFINITY) {
        Node starting_node = {NULL, start_state, type_action::NONE, 0, start_h}; 
        open.push(starting_node);
    }

    // Initializing distances hashmap
    std::unordered_map<puzzle_8_state, uint> distances;

    // Main search loop
    while (!open.empty()) {
        Node n = open.top();
        open.pop();
        // Reopening
        if (distances.find(n.state) == distances.end() || n.g < distances[n.state]) {
            distances[n.state] = n.g;
            // Early goal test
            if (n.state == GOAL_STATE_8) { // TODO: pass goal state as parameter to avoid hardcoding 15 puzzle
                return extract_path(n);
            }
            for (const auto &[action, new_state] : get_next_states_8_puzzle(n.state)) {
                uint succ_h = manhattan_distance_8_puzzle(new_state, GOAL_STATE_8);
                if (succ_h < H_INFINITY) {
                    Node succ = Node{&n, new_state, action, n.g+1, succ_h}; // Considering every cost to be 1
                    open.push(succ);
                }
            }
        }
    }

    return solution{NO_SOLUTION}; // Unsolvable
}

solution gbfs(puzzle_8_state start_state) {
    // Initializing open
    std::priority_queue<Node, std::vector<Node>, GBFSCompare> open;
    uint start_h = manhattan_distance_8_puzzle(start_state, GOAL_STATE_8);
    if (start_h < H_INFINITY) {
        Node starting_node = {NULL, start_state, type_action::NONE, 0, start_h};
        open.push(starting_node);
    }

    // Initializing distances hashmap
    std::unordered_map<puzzle_8_state, uint> distances;

    // Main search loop
    while (!open.empty()) {
        Node n = open.top();
        open.pop();
        // Reopening
        if (distances.find(n.state) == distances.end() || n.g < distances[n.state]) {
            distances[n.state] = n.g;
            // Early goal test
            if (n.state == GOAL_STATE_8) { 
                return extract_path(n);
            }
            for (const auto &[action, new_state] : get_next_states_8_puzzle(n.state)) {
                uint succ_h = manhattan_distance_8_puzzle(new_state, GOAL_STATE_8);
                if (succ_h < H_INFINITY) {
                    Node succ = Node{&n, new_state, action, n.g+1, succ_h}; // Considering every cost to be 1
                    open.push(succ);
                }
            }
        }
    }

    return solution{NO_SOLUTION}; // Unsolvable
}

solution id_a_star(puzzle_8_state start_state) {
    Node starting_node = {NULL, start_state, type_action::NONE, 0, manhattan_distance_8_puzzle(start_state, GOAL_STATE_8)};
    uint f_limit = starting_node.h;
    while (f_limit != H_INFINITY) {
        std::pair<uint, solution> f_limit_solution = recursive_search(starting_node, f_limit);
        if (f_limit_solution.second.front() != NO_SOLUTION) {
            return f_limit_solution.second;
        }
    }
    
    return solution{NO_SOLUTION}; // Unsolvable
}

std::pair<uint, solution> recursive_search(Node n, uint f_limit) {
    uint fn = n.g + n.h;
    if (fn > f_limit) {
        return std::pair<uint, solution>{fn, solution{NO_SOLUTION}};
    }

    if (n.state == GOAL_STATE_8) {
        return std::pair<uint, solution>{F_LIMIT_NONE, extract_path(n)};
    }

    uint next_limit = F_INFINITY;
    for (const auto &[action, new_state] : get_next_states_8_puzzle(n.state)) {
        uint succ_h = manhattan_distance_8_puzzle(new_state, GOAL_STATE_8);
        if (succ_h < H_INFINITY) {
            Node n = Node{&n, new_state, action, n.g+1, succ_h};
            std::pair<uint, solution> f_limit_solution = recursive_search(n, f_limit);
            if (f_limit_solution.second.front() != NO_SOLUTION) {
                return std::pair<uint, solution>{F_LIMIT_NONE, f_limit_solution.second};
            }
            next_limit = std::min(next_limit, f_limit_solution.first);
        }
    }

    return std::pair<uint, solution>{next_limit, solution{NO_SOLUTION}};
}