#include <queue>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "../headers/search.hpp"
#include "../headers/puzzle.hpp"
#include <climits>

struct CompareNode {
    bool operator()(search_node_t const& a, search_node_t const& b) {
        bool smaller_cost = a.total_cost > b.total_cost;
        bool equal_cost = a.total_cost == b.total_cost;
        bool smaller_heuristic = a.heuristic > b.heuristic;
        bool equal_heuristic = a.heuristic == b.heuristic;
        bool lifo_seq = a.seq < b.seq;
        return smaller_cost || (equal_cost && (smaller_heuristic || (equal_heuristic && lifo_seq)));
    }
};

void a_star(state_t start_state, int& expanded_nodes, int& found_cost, int& start_heuristic){
    int seq = 0;
    // Make node
    start_heuristic = calculate_manhattan_distance(start_state, GOAL_STATE);
    search_node_t start_node = {start_state, NONE, start_heuristic, 0, start_heuristic, seq++};
    std::priority_queue<search_node_t, std::vector<search_node_t>, CompareNode> open;
    open.push(start_node);

    std::unordered_map<state_t, int> distances;

    while(!open.empty()){
        search_node_t current = open.top();
        open.pop();

        if(distances.find(current.state) == distances.end() || current.total_cost < distances[current.state]){
            distances[current.state] = current.total_cost;

            if(current.state == GOAL_STATE){
                found_cost = current.cost;
                return;
            }
            expanded_nodes++;
            std::vector<action_t> actions = valid_actions(current.state);
            for(action_t action : actions){
                if (action == get_opposite_action(current.action)){
                    continue;
                }
                state_t new_state = apply_action(current.state, action);
                int new_cost = current.cost + 1;
                int new_heuristic = calculate_manhattan_distance(new_state, GOAL_STATE);
                search_node_t new_node = {new_state, action, new_heuristic, new_cost, new_cost + new_heuristic, seq++};
                open.push(new_node);
            }
        }
    }
}


void a_star_15(state_t start_state, int& expanded_nodes, int& found_cost, int& start_heuristic){
    int seq = 0;
    // Make node
    start_heuristic = calculate_manhattan_distance_15(start_state, GOAL_STATE_15);
    search_node_t start_node = {start_state, NONE, start_heuristic, 0, start_heuristic, seq++};
    std::priority_queue<search_node_t, std::vector<search_node_t>, CompareNode> open;
    open.push(start_node);

    std::unordered_map<state_t, int> distances;

    while(!open.empty()){
        search_node_t current = open.top();
        open.pop();

        if(distances.find(current.state) == distances.end() || current.total_cost < distances[current.state]){
            distances[current.state] = current.total_cost;

            if(current.state == GOAL_STATE_15){
                found_cost = current.cost;
                return;
            }
            expanded_nodes++;
            std::vector<action_t> actions = valid_actions_15(current.state);
            for(action_t action : actions){
                if (action == get_opposite_action(current.action)){
                    continue;
                }
                state_t new_state = apply_action_15(current.state, action);
                int new_cost = current.cost + 1;
                int new_heuristic = calculate_manhattan_distance_15(new_state, GOAL_STATE_15);
                search_node_t new_node = {new_state, action, new_heuristic, new_cost, new_cost + new_heuristic, seq++};
                open.push(new_node);
            }
        }
    }
}

struct CompareNodeGBFS {
    bool operator()(search_node_t const& a, search_node_t const& b) {
        bool smaller_heuristic = a.heuristic > b.heuristic;
        bool equal_heuristic = a.heuristic == b.heuristic;
        bool greater_cost = a.cost < b.cost;
        bool equal_cost = a.cost == b.cost;
        bool lifo_seq = a.seq < b.seq;
        return smaller_heuristic || (equal_heuristic && (greater_cost || (equal_cost && lifo_seq)));
    }
};

void gbfs(state_t start_state, int& expanded_nodes, int& found_cost, int& start_heuristic){ // Without re-opening
    int seq = 0;
    // Make node
    start_heuristic = calculate_manhattan_distance(start_state, GOAL_STATE);
    search_node_t start_node = {start_state, NONE, start_heuristic, 0, start_heuristic, seq++};
    std::priority_queue<search_node_t, std::vector<search_node_t>, CompareNodeGBFS> open;
    open.push(start_node);

    std::unordered_set<state_t> closed;

    while(!open.empty()){
        search_node_t current = open.top();
        open.pop();

        if(closed.find(current.state) == closed.end()){
            closed.insert(current.state);

            if(current.state == GOAL_STATE){
                found_cost = current.cost;
                return;
            }
            expanded_nodes++;
            std::vector<action_t> actions = valid_actions(current.state);
            for(action_t action : actions){
                if (action == get_opposite_action(current.action)){
                    continue;
                }
                state_t new_state = apply_action(current.state, action);
                int new_cost = current.cost + 1;
                int new_heuristic = calculate_manhattan_distance(new_state, GOAL_STATE);
                search_node_t new_node = {new_state, action, new_heuristic, new_cost, new_heuristic, seq++};
                open.push(new_node);
            }
        }
    }
}

std::pair<bool, int> ida_search(search_node_t &node, int& threshold, int& expanded_nodes, int& found_cost){

    if(node.total_cost > threshold){
        return {false, node.total_cost};
    }
    
    if(node.state == GOAL_STATE){
        found_cost = node.cost;
        return {true, 0};
    }

    expanded_nodes++;

    int next_threshold = INT_MAX;

    for(action_t action : valid_actions(node.state)){
        if(action == get_opposite_action(node.action)){
            continue;
        }
        state_t new_state = apply_action(node.state, action);
        int new_cost = node.cost + 1;
        int new_heuristic = calculate_manhattan_distance(new_state, GOAL_STATE);
        search_node_t new_node = {new_state, action, new_heuristic, new_cost, new_cost + new_heuristic, 0};

        std::pair<bool, int> res = ida_search(new_node, threshold, expanded_nodes, found_cost);
        if(res.first) return {true, 0};
        next_threshold = std::min(next_threshold, res.second);
    }
    return {false, next_threshold};
}

void ida_star(state_t start_state, int& expanded_nodes, int& found_cost, int& start_heuristic){
    int seq = 0;
    // Make node
    start_heuristic = calculate_manhattan_distance(start_state, GOAL_STATE);
    search_node_t start_node = {start_state, NONE, start_heuristic, 0, start_heuristic, seq++};
    int threshold = start_heuristic;
    while(true){
        //start_heuristic = calculate_manhattan_distance(start_state, GOAL_STATE); // This is not needed
        // But if we add this line the average heuristic matches the reference. 
        std::pair<bool, int> res = ida_search(start_node, threshold, expanded_nodes, found_cost);
        if(res.first) return;
        threshold = res.second;
    }
}