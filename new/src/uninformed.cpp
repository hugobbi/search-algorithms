/**
 * Implementations of uninformed search algorithms
 * Mostly taken from the slides
 * Standard stuff.
*/
#include "../headers/search.hpp"
#include "../headers/puzzle.hpp"
#include <deque>
#include <unordered_set>
#include <iostream>

void bfs_graph(state_t state, int& expanded_nodes, int& found_cost, int& start_heuristic){
    expanded_nodes = 0;
    start_heuristic = calculate_manhattan_distance(state, GOAL_STATE);
    if(state == GOAL_STATE){
        found_cost = 0;
        return;
    }

    std::deque<search_node_t> open;
    std::unordered_set<state_t> closed;

    search_node_t start_node = {state, NONE, 0, 0, 0, 0};
    open.push_back(start_node);
    closed.insert(state);

    while(!open.empty()){
        search_node_t current = open.front();
        open.pop_front();

        expanded_nodes++;
        
        for(action_t action : valid_actions(current.state)){
            if(action == get_opposite_action(current.action)){
                continue;
            }
            state_t new_state = apply_action(current.state, action);
            if(new_state == GOAL_STATE){
                found_cost = current.cost+1;
                return;
            }
            if(closed.find(new_state) == closed.end()){
                search_node_t new_node = {new_state, action, 0, current.cost + 1, 0, 0};
                open.push_back(new_node);
                closed.insert(new_state);
            }
        }
    }
}

bool dfs(search_node_t node, int limit, int& expanded_nodes, int& found_cost, int& start_heuristic){
    if(node.state == GOAL_STATE){
        found_cost = node.cost;
        reset_average_manhattan_distance();
        return true;
    }
    if (limit > 0){
        expanded_nodes++;
        for(action_t action : valid_actions(node.state)){
            if(action == get_opposite_action(node.action)){
                continue;
            }
            state_t new_state = apply_action(node.state, action);
            search_node_t new_node = {new_state, action, 0, node.cost + 1, 0, 0};
            if (dfs(new_node, limit - 1, expanded_nodes, found_cost, start_heuristic)) return true;
        }
    }
    return false;
}

void idfs(state_t state, int& expanded_nodes, int& found_cost, int& start_heuristic){
    expanded_nodes = 0;
    start_heuristic = calculate_manhattan_distance(state, GOAL_STATE);
    search_node_t start_node = {state, NONE, 0, 0, 0, 0};
    for(int limit = 0; ;limit++){
        if (dfs(start_node, limit, expanded_nodes, found_cost, start_heuristic)){
            return;
        }
    }
}