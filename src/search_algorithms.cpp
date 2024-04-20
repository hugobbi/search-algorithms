#include "../headers/search_algorithms.hpp"
#include "../headers/puzzle.hpp"

std::vector<type_action> extract_path(Node& n) {
    std::vector<type_action> path;
    while (n.father_node != NULL) {
        path.push_back(n.action);
        n = *n.father_node;
    }

    return path;
}

std::vector<type_action> bfs(puzzle_state start_state) {
    // Early goal test
    if (start_state == GOAL_STATE_8) {
        return std::vector<type_action>();
    }

    // Initializing open and closed
    Node starting_node = {NULL, start_state, 0}; // TODO: define action
    std::queue<Node> open;
    open.push(starting_node);
    std::set<puzzle_state> closed;
    closed.insert(start_state);

    // Main search loop
    while (!open.empty()) {
        Node n = open.front();
        open.pop();
        for (ActionState succ_action_state : successors_8puzzle(n.state)) { // A possible optimization is to only calculate new action/state after checking if previous state is goal
            Node succ = Node{&n, succ_action_state.state, succ_action_state.action};
            if (succ.state == GOAL_STATE_8) {  // Early goal test
                return extract_path(succ);
            }
            if (closed.find(succ.state) == closed.end()) { // Insert into open if nodes not in closed
                open.push(succ);
                closed.insert(succ.state);
            }
        }
    }

    return std::vector<type_action>() = {NO_SOLUTION}; // Unsolvable
}

std::vector<type_action> dfs_limited_depth(puzzle_state state, uint16_t depth_limit) {
    // Early goal test
    if (state == GOAL_STATE_8) {
        return std::vector<type_action>();
    }

    // Expands nodes if depth limit is not reached
    if (depth_limit != 0) {
        for (ActionState action_state : successors_8puzzle(state)) {
            std::vector<type_action> path = dfs_limited_depth(action_state.state, depth_limit-1); // Call dfs on child node

            // If a solution is found, return the path
            if (path.front() != NO_SOLUTION) {
                path.push_back(action_state.action);
                return path;
            }
        }
    }

    return std::vector<type_action>() = {NO_SOLUTION}; // No solution in this branch with this depth limite
}

std::vector<type_action> id_dfs(puzzle_state start_state, uint16_t max_depth) {
    // Call dfs with limited depth for each depth limit
    for (uint16_t depth_limit = 0; depth_limit < max_depth; depth_limit++) {
        std::vector<type_action> path = dfs_limited_depth(start_state, depth_limit);
        // If a solution is found, return the path
        if (path.front() != NO_SOLUTION) {
            return path;
        }
    }

    return std::vector<type_action>() = {NO_SOLUTION}; // Unsolvable with this max depth
}

std::vector<type_action> a_star(puzzle_state start_state) {
    // Initializing open
    std::priority_queue<Node, std::vector<Node>, AStarCompare> open;
    uint start_h = manhattan_distance(start_state);
    if (start_h < H_INFINITY) {
        Node starting_node = {NULL, start_state, 0, 0, start_h}; // TODO: define action
        open.push(starting_node);
    }

    // Initializing distances hashmap
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
            for (ActionState succ_action_state : successors_8puzzle(n.state)) {
                uint succ_h = manhattan_distance(succ_action_state.state);
                if (succ_h < H_INFINITY) {
                    Node succ = Node{&n, succ_action_state.state, succ_action_state.action, n.g+1, succ_h}; // Considering every cost to be 1
                    open.push(succ);
                }
            }
        }
    }

    return std::vector<type_action>() = {NO_SOLUTION}; // Unsolvable
}

std::vector<type_action> gbfs(puzzle_state start_state) {
    // Initializing open
    std::priority_queue<Node, std::vector<Node>, GBFSCompare> open;
    uint start_h = manhattan_distance(start_state);
    if (start_h < H_INFINITY) {
        Node starting_node = {NULL, start_state, 0, 0, start_h}; // TODO: define action
        open.push(starting_node);
    }

    // Initializing distances hashmap
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
            for (ActionState succ_action_state : successors_8puzzle(n.state)) {
                uint succ_h = manhattan_distance(succ_action_state.state);
                if (succ_h < H_INFINITY) {
                    Node succ = Node{&n, succ_action_state.state, succ_action_state.action, n.g+1, succ_h}; // Considering every cost to be 1
                    open.push(succ);
                }
            }
        }
    }

    return std::vector<type_action>() = {NO_SOLUTION}; // Unsolvable
}

std::vector<type_action> id_a_star(puzzle_state start_state) {
    Node n = {NULL, start_state, 0, 0, manhattan_distance(start_state)};
    uint f_limit = n.h;
    while (f_limit != H_INFINITY) {
        FLimitSolution f_limit_solution = recursive_search(n, f_limit);
        if (f_limit_solution.solution.front() != NO_SOLUTION) {
            return f_limit_solution.solution;
        }
    }
    
    return std::vector<type_action>() = {NO_SOLUTION}; // Unsolvable
}

FLimitSolution recursive_search(Node n, uint f_limit) {
    uint fn = n.g + n.h;
    if (fn > f_limit) {
        return FLimitSolution{fn, std::vector<type_action>() = {NO_SOLUTION}};
    }

    if (n.state == GOAL_STATE_8) {
        return FLimitSolution{F_LIMIT_NONE, extract_path(n)};
    }

    uint next_limit = F_INFINITY;
    for (ActionState succ_action_state : successors_8puzzle(n.state)) {
        uint succ_h = manhattan_distance(succ_action_state.state);
        if (succ_h < H_INFINITY) {
            Node n = Node{&n, succ_action_state.state, succ_action_state.action, n.g+1, succ_h};
            FLimitSolution f_limit_solution = recursive_search(n, f_limit);
            if (f_limit_solution.solution.front() != NO_SOLUTION) {
                return FLimitSolution{F_LIMIT_NONE, f_limit_solution.solution};
            }
            next_limit = std::min(next_limit, f_limit_solution.f_limit);
        }
    }
    return FLimitSolution{next_limit, std::vector<type_action>() = {NO_SOLUTION}};
}


uint manhattan_distance(puzzle_state state) {


    return 0;
}