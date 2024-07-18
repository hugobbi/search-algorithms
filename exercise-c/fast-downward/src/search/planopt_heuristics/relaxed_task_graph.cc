#include "relaxed_task_graph.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <deque>


using namespace std;

namespace planopt_heuristics {

  string name_formula(string prev_formula, PropositionID var){
    if(prev_formula == ""){
      return to_string(var);
    }
    return prev_formula + "&" + to_string(var);
  }
  
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

  // We will keep a map from set of variables to nodes
  unordered_map<string, NodeID> node_map;
  
  for(Proposition p : relaxed_task.propositions){
    variable_node_ids[p.id] = graph.add_node(NodeType::OR);
    node_map[name_formula("", p.id)] = variable_node_ids[p.id]; // Add it to our map
  } // variable_node_ids should contain the node id of the variable node for variable i -- OK

  initial_node_id = graph.add_node(NodeType::AND); // initial_node_id should contain the node id of the initial node -- OK


  string initial_formula = "";
  sort(relaxed_task.initial_state.begin(), relaxed_task.initial_state.end());
  for(PropositionID p_id: relaxed_task.initial_state){
    initial_formula = name_formula(initial_formula, p_id);
    NodeID var_node = variable_node_ids[p_id];
    graph.add_edge(var_node, initial_node_id);
  } // for all variables in the initial state, there is an arc from the variable to the initial node -- OK
  
  node_map[initial_formula] = initial_node_id; // Add initial node to the map

  string goal_formula = "";
  sort(relaxed_task.goal.begin(), relaxed_task.goal.end());
  for(PropositionID p_id : relaxed_task.goal){
    goal_formula = name_formula(goal_formula, p_id);
    if(node_map.find(goal_formula) == node_map.end()){
      node_map[goal_formula] = graph.add_node(NodeType::AND);
      graph.add_edge(node_map[goal_formula], goal_node_id);
      graph.add_edge(node_map[goal_formula], variable_node_ids[p_id]);
    }
    goal_node_id = node_map[goal_formula];
  }
  
  if(relaxed_task.goal.size() == 0){ // If the list was empty, the goal is trivially true
    goal_node_id = graph.add_node(NodeType::AND);
  }
  // Goal subgraph -- OK

  for(RelaxedOperator op : relaxed_task.operators){
    
    // We create an effect node for each operator
    NodeID effect = graph.add_node(NodeType::AND, op.cost);
    for(PropositionID var : op.effects){
      graph.add_edge(variable_node_ids[var], effect); // The effect sets the vars to true
    }

    // Now we create the preconditions
    string precondition_formula = "";
    NodeID precondition_node = -1;

    // Sort preconditions so the formulas are consistent
    sort(op.preconditions.begin(), op.preconditions.end());
    
    for(PropositionID p_id : op.preconditions){
      precondition_formula = name_formula(precondition_formula, p_id);
      if(node_map.find(precondition_formula) == node_map.end()){
	node_map[precondition_formula] = graph.add_node(NodeType::AND);
	graph.add_edge(node_map[precondition_formula], precondition_node);
	graph.add_edge(node_map[precondition_formula], variable_node_ids[p_id]);
      }
      precondition_node = node_map[precondition_formula];
    }
    if(op.preconditions.size() == 0){
      precondition_node = graph.add_node(NodeType::AND);
    }
    graph.add_edge(effect, precondition_node);
  } // Operators --  OK 

  /*/ Unga-Bunga solution
  for(Proposition p : relaxed_task.propositions){
    variable_node_ids[p.id] = graph.add_node(NodeType::OR);
  }
  initial_node_id = graph.add_node(NodeType::AND);
  for(PropositionID p_id : relaxed_task.initial_state){
    graph.add_edge(variable_node_ids[p_id], initial_node_id);
  }

  goal_node_id = graph.add_node(NodeType::AND);
  for(PropositionID p_id : relaxed_task.goal){
    graph.add_edge(goal_node_id, variable_node_ids[p_id]);
  }

  for(RelaxedOperator op : relaxed_task.operators){
    NodeID effects = graph.add_node(NodeType::AND, op.cost);
    for(PropositionID p_id : op.effects){
      graph.add_edge(variable_node_ids[p_id], effects);
    }
    NodeID preconditions = graph.add_node(NodeType::AND);
    for(PropositionID p_id : op.preconditions){
      graph.add_edge(preconditions, variable_node_ids[p_id]);
    }
    graph.add_edge(effects, preconditions);
    }*/
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
  graph.weighted_most_conservative_valuation();
  return graph.get_node(goal_node_id).additive_cost;
}


int RelaxedTaskGraph::ff_cost_of_goal() {
  graph.weighted_most_conservative_valuation();
  deque<NodeID> queue;
  unordered_set<NodeID> closed;
  queue.push_back(goal_node_id);
  int ff_cost = 0;
  while(!queue.empty()){
    AndOrGraphNode node = graph.get_node(queue.front());
    queue.pop_front();
    ff_cost += node.direct_cost;
    switch(node.type){
    case NodeType::OR:
      if(node.achiever == -1){
	cout << "something is fishy\n";
      }
      if(closed.find(node.achiever) == closed.end()){
	queue.push_back(node.achiever);
	closed.insert(node.achiever);
      }
      break;
    case NodeType::AND:
      for(NodeID succ : node.successor_ids){
	if(closed.find(succ) == closed.end()){
	  queue.push_back(succ);
	  closed.insert(succ);
	}
      }
    }
  }
  return  ff_cost;
}
}
