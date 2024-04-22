#include "../headers/search_algorithms.hpp"
#include "../headers/puzzle.hpp"

type_solution extract_path(Node& n) {
    type_solution path;
    while (n.father_node != NULL) {
        path.push_back(n.action);
        n = *n.father_node;
    }

    path.push_back(n.action); // Root node

    return path;
}

type_solution bfs(puzzle_state start_state, Evaluation& eval) {
    // Early goal test
    if (start_state == GOAL_STATE_8) {
        return type_solution{type_action::NONE};
    }

    // Initializing open and closed
    Node starting_node = {NULL, start_state, type_action::NONE};
    std::queue<Node> open;
    open.push(starting_node);
    std::set<puzzle_state> closed;
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
            if (closed.find(succ.state) == closed.end()) { // Insert into open if node not in closed
                open.push(succ);
                closed.insert(succ.state);
            }
        }
    }

    return type_solution{type_action::UNSOLVABLE}; // Unsolvable
}

type_solution dfs_limited_depth(puzzle_state state, uint16_t depth_limit, Evaluation& eval) {
    // Early goal test
    if (state == GOAL_STATE_8) {
        return type_solution{type_action::NONE};
    }

    // Expands nodes if depth limit is not reached
    if (depth_limit != 0) {
        for (const auto &[action, new_state] : get_next_states_8_puzzle(state)) {
            type_solution path = dfs_limited_depth(new_state, depth_limit-1, eval); // Call dfs on child node

            // If a solution is found, return the path
            if (path.front() != type_action::UNSOLVABLE) {
                path.push_back(action);
                return path;
            }
        }
    }

    return type_solution{type_action::UNSOLVABLE}; // No solution in this branch with this depth limite
}

type_solution idfs(puzzle_state start_state, Evaluation& eval) {
    // Call dfs with limited depth for each depth limit
    for (uint16_t depth_limit = 0; depth_limit < MAX_DEPTH; depth_limit++) {
        type_solution path = dfs_limited_depth(start_state, depth_limit, eval);
        // If a solution is found, return the path
        if (path.front() != type_action::UNSOLVABLE) {
            return path;
        }
    }

    return type_solution{type_action::UNSOLVABLE}; // Unsolvable with this max depth
}

type_solution astar(puzzle_state start_state, Evaluation& eval) {
    // Initializing open
    std::priority_queue<Node, std::vector<Node>, AStarCompare> open;
    uint start_h = manhattan_distance_8_puzzle(start_state, GOAL_STATE_8);
    if (start_h < H_INFINITY) {
        Node starting_node = {NULL, start_state, type_action::NONE, 0, start_h}; 
        open.push(starting_node);
    }

    // Initializing distances hashmap for reopening and duplicate detection
    std::unordered_map<puzzle_state, uint> distances;

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

    return type_solution{type_action::UNSOLVABLE}; // Unsolvable
}

type_solution gbfs(puzzle_state start_state, Evaluation& eval) {
    // Initializing open
    std::priority_queue<Node, std::vector<Node>, GBFSCompare> open;
    uint start_h = manhattan_distance_8_puzzle(start_state, GOAL_STATE_8);
    if (start_h < H_INFINITY) {
        Node starting_node = {NULL, start_state, type_action::NONE, 0, start_h};
        open.push(starting_node);
    }

    // Initializing distances hashmap for reopening and duplicate detection
    std::unordered_map<puzzle_state, uint> distances;

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

    return type_solution{type_action::UNSOLVABLE}; // Unsolvable
}

type_solution idastar(puzzle_state start_state, Evaluation& eval) {
    Node starting_node = {NULL, start_state, type_action::NONE, 0, manhattan_distance_8_puzzle(start_state, GOAL_STATE_8)};
    uint f_limit = starting_node.h;
    while (f_limit != H_INFINITY) {
        std::pair<uint, type_solution> f_limit_solution = recursive_search(starting_node, f_limit, eval);
        if (f_limit_solution.second.front() != type_action::UNSOLVABLE) {
            return f_limit_solution.second;
        }
    }
    
    return type_solution{type_action::UNSOLVABLE}; // Unsolvable
}

std::pair<uint, type_solution> recursive_search(Node n, uint f_limit, Evaluation& eval) {
    uint fn = n.g + n.h;
    if (fn > f_limit) {
        return std::pair<uint, type_solution>{fn, type_solution{type_action::UNSOLVABLE}};
    }

    if (n.state == GOAL_STATE_8) {
        return std::pair<uint, type_solution>{F_LIMIT_NONE, extract_path(n)};
    }

    uint next_limit = F_INFINITY;
    for (const auto &[action, new_state] : get_next_states_8_puzzle(n.state)) {
        uint succ_h = manhattan_distance_8_puzzle(new_state, GOAL_STATE_8);
        if (succ_h < H_INFINITY) {
            Node n = Node{&n, new_state, action, n.g+1, succ_h};
            std::pair<uint, type_solution> f_limit_solution = recursive_search(n, f_limit, eval);
            if (f_limit_solution.second.front() != type_action::UNSOLVABLE) {
                return std::pair<uint, type_solution>{F_LIMIT_NONE, f_limit_solution.second};
            }
            next_limit = std::min(next_limit, f_limit_solution.first);
        }
    }

    return std::pair<uint, type_solution>{next_limit, type_solution{type_action::UNSOLVABLE}};
}
