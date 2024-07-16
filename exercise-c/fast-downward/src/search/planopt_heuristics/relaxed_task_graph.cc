#include "relaxed_task_graph.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <set>

using namespace std;


struct SetHash {
    std::size_t operator()(const std::set<int>& s) const {
        std::size_t hash = 0;
        std::hash<int> hasher;
        for (const int& i : s) {
            hash ^= hasher(i) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

struct SetEqual {
    bool operator()(const std::set<int>& lhs, const std::set<int>& rhs) const {
        return lhs == rhs;
    }
};

namespace planopt_heuristics {
RelaxedTaskGraph::RelaxedTaskGraph(const TaskProxy &task_proxy)
    : relaxed_task(task_proxy),
      variable_node_ids(relaxed_task.propositions.size()) {
    /*
      TODO: add your code for exercise 2 (b) here. Afterwards
        - variable_node_ids[i] should contain the node id of the variable node for variable i
        - initial_node_id should contain the node id of the initial node
        - goal_node_id should contain the node id of the goal node
        - the graph should contain precondition and effect nodes for all operators
        - the graph should contain all necessary edges.
    */

  // Step 1: Generate an initial node and a goal node
  // Both the initial state and the goal are AND nodes
  initial_node_id = graph.add_node(NodeType::AND);
  goal_node_id = graph.add_node(NodeType::AND);

  // Step 2: Generate all variable nodes
  for(Proposition p : relaxed_task.propositions){
    // Generate a new node for each proposition
    // Variables are OR nodes.
    NodeID node_id = graph.add_node(NodeType::OR);


    // Add it to the list
    variable_node_ids[p.id] = node_id;
  }

  // Some variables have inbound arrows to the initial node
  for(PropositionID p_id : relaxed_task.initial_state){
    graph.add_edge(variable_node_ids[p_id], initial_node_id);
  }

  // The goal has dependencies in some variables
  for(PropositionID p_id : relaxed_task.goal){
    graph.add_edge(goal_node_id, variable_node_ids[p_id]);
  }

  // Now we have to create the operator sub-graphs.
  unordered_map<set<PropositionID>, NodeID, SetHash, SetEqual> formula_nodes;

  for(RelaxedOperator op : relaxed_task.operators){
    NodeID effect_node = graph.add_node(NodeType::AND, op.cost); // Added cost for ex. 2-c
    for(PropositionID p : op.effects){
      graph.add_edge(variable_node_ids[p], effect_node);
    }
    NodeID precondition_node;
    if(op.preconditions.size() == 1){
      // Don't create sub-formulas if they are equal to variables
      precondition_node = variable_node_ids[op.preconditions[0]];
    }else{
      set<PropositionID> preconditions;
      for(PropositionID p : op.preconditions){
	preconditions.insert(p);
      }
      if(formula_nodes.find(preconditions) == formula_nodes.end()){
	// We don't have a node for this formula
	formula_nodes[preconditions] = graph.add_node(NodeType::AND);
	for(PropositionID p : op.preconditions){
	  graph.add_edge(formula_nodes[preconditions], variable_node_ids[p]);
	}
      }
      precondition_node = formula_nodes[preconditions];
    }
    graph.add_edge(effect_node, precondition_node);
  }
}

void RelaxedTaskGraph::change_initial_state(const GlobalState &global_state) {
    // Remove all initial edges that where introduced for relaxed_task.initial_state.
    for (PropositionID id : relaxed_task.initial_state) {
        graph.remove_edge(variable_node_ids[id], initial_node_id);
    }

    // Switch initial state of relaxed_task
    relaxed_task.initial_state = relaxed_task.translate_state(global_state);

    // Add all initial edges for relaxed_task.initial_state.
    for (PropositionID id : relaxed_task.initial_state) {
        graph.add_edge(variable_node_ids[id], initial_node_id);
    }
}

bool RelaxedTaskGraph::is_goal_relaxed_reachable() {
    // Compute the most conservative valuation of the graph and use it to
    // return true iff the goal is reachable in the relaxed task.

    graph.most_conservative_valuation();
    return graph.get_node(goal_node_id).forced_true;
}

int RelaxedTaskGraph::additive_cost_of_goal() {
    // Compute the weighted most conservative valuation of the graph and use it
    // to return the h^add value of the goal node.

    // TODO: add your code for exercise 2 (c) here.
    return -1;
}

int RelaxedTaskGraph::ff_cost_of_goal() {
    // TODO: add your code for exercise 2 (e) here.
    return -1;
}

}
