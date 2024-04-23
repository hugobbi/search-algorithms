#include "../headers/search_algorithms.hpp"
#include "../headers/puzzle.hpp"

type_solution extract_path(Node* n) {
    type_solution path;
    while (n->father_node != nullptr) {
        Node* father = n->father_node;
        delete n; // frees memory on the way
        path.push_back(father->action);
        n = father;
    }

    path.push_back(n->action); // Root node
    delete n;

    return path;
}

type_solution bfs(const puzzle_state& start_state, Evaluation& eval) {
    eval.start_time = std::chrono::high_resolution_clock::now();
    
    uint starting_node_h = manhattan_distance_8_puzzle(start_state, GOAL_STATE_8, eval);
    eval.initial_heuristic_value = starting_node_h;

    // Early goal test
    if (start_state == GOAL_STATE_8) {
        return type_solution{type_action::NONE};
    }

    // Initializing open and closed
    Node starting_node = Node{nullptr, start_state, type_action::NONE};
    std::queue<Node> open;
    open.push(starting_node);
    std::set<puzzle_state> closed;
    closed.insert(start_state);

    // Main search loop
    while (!open.empty()) {
        Node* n = new Node{open.front()};
        open.pop();
        puzzle_state father_state; // Getting father state as to not generate it when getting successors
        father_state = n->father_node == nullptr ? NONE_STATE : n->father_node->state;

        eval.expanded_nodes++;

        for (const auto &[action, new_state] : get_next_states_8_puzzle(n->state, father_state)) {
            Node* succ = new Node{n, new_state, action};
            if (succ->state == GOAL_STATE_8) {  // Early goal test
                return extract_path(succ);
            }
            if (closed.find(succ->state) == closed.end()) { // Insert into open if node not in closed
                open.push(*succ);
                closed.insert(succ->state);
            }
        }
    }

    return type_solution{type_action::UNSOLVABLE}; // Unsolvable
}

type_solution dfs_limited_depth(const puzzle_state& state, const puzzle_state& father_state, uint16_t depth_limit, Evaluation& eval) {
    // Early goal test
    if (state == GOAL_STATE_8) {
        return type_solution{type_action::NONE};
    }

    // Expands nodes if depth limit is not reached
    if (depth_limit != 0) {

        eval.expanded_nodes++;

        for (const auto &[action, new_state] : get_next_states_8_puzzle(state, father_state)) {
            type_solution path = dfs_limited_depth(new_state, state, depth_limit-1, eval); // Call dfs on child node

            // If a solution is found, return the path
            if (path.front() != type_action::UNSOLVABLE) {
                path.push_back(action);
                return path;
            }
        }
    }

    return type_solution{type_action::UNSOLVABLE}; // No solution in this branch with this depth limite
}

type_solution idfs(const puzzle_state& start_state, Evaluation& eval) {
    eval.start_time = std::chrono::high_resolution_clock::now();
    
    uint starting_node_h = manhattan_distance_8_puzzle(start_state, GOAL_STATE_8, eval);
    eval.initial_heuristic_value = starting_node_h;

    // Call dfs with limited depth for each depth limit
    for (uint16_t depth_limit = 0; depth_limit < MAX_DEPTH; depth_limit++) {
        type_solution path = dfs_limited_depth(start_state, NONE_STATE, depth_limit, eval);
        // If a solution is found, return the path
        if (path.front() != type_action::UNSOLVABLE) {
            return path;
        }
    }

    return type_solution{type_action::UNSOLVABLE}; // Unsolvable with this max depth
}

type_solution astar(const puzzle_state& start_state, Evaluation& eval) {
    eval.start_time = std::chrono::high_resolution_clock::now();

    uint starting_node_h = manhattan_distance_8_puzzle(start_state, GOAL_STATE_8, eval);
    eval.initial_heuristic_value = starting_node_h;

    // Initializing open
    std::priority_queue<Node, std::vector<Node>, AStarCompare> open;

    if (starting_node_h < H_INFINITY) {
        Node starting_node = {nullptr, start_state, type_action::NONE, 0, starting_node_h}; 
        open.push(starting_node);
    }

    // Initializing distances hashmap for reopening and duplicate detection
    std::unordered_map<puzzle_state, uint> distances;

    // Main search loop
    while (!open.empty()) {
        Node* n = new Node{open.top()};
        open.pop();
        // Reopening
        if (distances.find(n->state) == distances.end() || n->g < distances[n->state]) {
            distances[n->state] = n->g;
            // Early goal test
            if (n->state == GOAL_STATE_8) { // TODO: pass goal state as parameter to avoid hardcoding 15 puzzle
                return extract_path(n);
            }
            puzzle_state father_state; // Getting father state as to not generate it when getting successors
            father_state = n->father_node == nullptr ? NONE_STATE : n->father_node->state;
            
            eval.expanded_nodes++;

            for (const auto &[action, new_state] : get_next_states_8_puzzle(n->state, father_state)) {
                uint succ_h = manhattan_distance_8_puzzle(new_state, GOAL_STATE_8, eval);
                if (succ_h < H_INFINITY) {
                    Node* succ = new Node{n, new_state, action, n->g+1, succ_h}; // Considering every cost to be 1
                    open.push(*succ);
                }
            }
        }
    }

    return type_solution{type_action::UNSOLVABLE}; // Unsolvable
}

type_solution gbfs(const puzzle_state& start_state, Evaluation& eval) {
    eval.start_time = std::chrono::high_resolution_clock::now();

    uint starting_node_h = manhattan_distance_8_puzzle(start_state, GOAL_STATE_8, eval);
    eval.initial_heuristic_value = starting_node_h;

    // Initializing open
    std::priority_queue<Node, std::vector<Node>, GBFSCompare> open;

    if (starting_node_h < H_INFINITY) {
        Node starting_node = {nullptr, start_state, type_action::NONE, 0, starting_node_h}; 
        open.push(starting_node);
    }

    // Initializing distances hashmap for reopening and duplicate detection
    std::unordered_map<puzzle_state, uint> distances;

    // Main search loop
    while (!open.empty()) {
        
        Node* n = new Node{open.top()};
        open.pop();
        // Reopening
        if (distances.find(n->state) == distances.end() || n->g < distances[n->state]) {
            distances[n->state] = n->g;
            // Early goal test
            if (n->state == GOAL_STATE_8) { // TODO: pass goal state as parameter to avoid hardcoding 15 puzzle
                return extract_path(n);
            }
            puzzle_state father_state; // Getting father state as to not generate it when getting successors
            father_state = n->father_node == nullptr ? NONE_STATE : n->father_node->state;

            eval.expanded_nodes++;
            
            for (const auto &[action, new_state] : get_next_states_8_puzzle(n->state, father_state)) {
                uint succ_h = manhattan_distance_8_puzzle(new_state, GOAL_STATE_8, eval);
                if (succ_h < H_INFINITY) {
                    Node* succ = new Node{n, new_state, action, n->g+1, succ_h}; // Considering every cost to be 1
                    open.push(*succ);
                }
            }
        }
    }

    return type_solution{type_action::UNSOLVABLE}; // Unsolvable
}

type_solution idastar(const puzzle_state& start_state, Evaluation& eval) {
    eval.start_time = std::chrono::high_resolution_clock::now();

    uint starting_node_h = manhattan_distance_8_puzzle(start_state, GOAL_STATE_8, eval);
    eval.initial_heuristic_value = starting_node_h;

    Node starting_node = {nullptr, start_state, type_action::NONE, 0, starting_node_h};
    std::pair<uint, type_solution> f_limit_solution = {starting_node.h, type_solution{type_action::UNSOLVABLE}};
    while (f_limit_solution.first != F_INFINITY) {
        f_limit_solution = recursive_search(starting_node, f_limit_solution.first, eval);
        if (f_limit_solution.second.front() != type_action::UNSOLVABLE) {
            return f_limit_solution.second;
        }
    }
    
    return type_solution{type_action::UNSOLVABLE}; // Unsolvable
}

std::pair<uint, type_solution> recursive_search(Node& n, uint f_limit, Evaluation& eval) {
    uint fn = n.g + n.h;
    if (fn > f_limit) {
        return std::pair<uint, type_solution>{fn, type_solution{type_action::UNSOLVABLE}};
    }

    if (n.state == GOAL_STATE_8) {
        return std::pair<uint, type_solution>{F_LIMIT_NONE, extract_path(&n)};
    }

    uint next_limit = F_INFINITY;
    puzzle_state father_state; // Getting father state as to not generate it when getting successors
    father_state = n.father_node == nullptr ? NONE_STATE : n.father_node->state;

    eval.expanded_nodes++;

    for (const auto &[action, new_state] : get_next_states_8_puzzle(n.state, father_state)) {
        uint succ_h = manhattan_distance_8_puzzle(new_state, GOAL_STATE_8, eval);
        if (succ_h < H_INFINITY) {
            Node* succ = new Node{&n, new_state, action, n.g+1, succ_h};
            std::pair<uint, type_solution> rec_limit_solution = recursive_search(*succ, f_limit, eval);
            if (rec_limit_solution.second.front() != type_action::UNSOLVABLE) {
                return std::pair<uint, type_solution>{F_LIMIT_NONE, rec_limit_solution.second};
            }
            next_limit = std::min(next_limit, rec_limit_solution.first);
        }
    }

    return std::pair<uint, type_solution>{next_limit, type_solution{type_action::UNSOLVABLE}};
}

double calculate_elapsed_time(const std::chrono::system_clock::time_point& start_time) {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count() / 1000000.0;
}

void print_evaluation(const Evaluation& eval) {
    std::cout << "Expanded nodes: " << eval.expanded_nodes << std::endl;
    std::cout << "Optimal solution length: " << eval.optimal_solution_length << std::endl;
    std::cout << "Time to solution: " << eval.time_to_solution << std::endl;
    std::cout << "Average heuristic value: " << eval.average_heuristic_value << std::endl;
    std::cout << "Initial heuristic value: " << eval.initial_heuristic_value << std::endl;
}
