#include "pdb.h"

#include "../utils/logging.h"

#include <queue>
#include <limits> 

using namespace std;

namespace planopt_heuristics {

/*
  An entry in the queue is a tuple (h, i) where h is the goal distance of state i.
  See comments below for details.
*/
using QueueEntry = pair<int, int>;


  bool op_reaches_state(const TNFOperator &op, const TNFState &state){
    for(TNFOperatorEntry entry : op.entries){
      if (state[entry.variable_id] != entry.effect_value){
	return false;
      }
    }
    return true;
  }

  TNFState get_precondition_state(const TNFOperator &op, const TNFState &state){
    TNFState prev_state = state;
    for(TNFOperatorEntry entry : op.entries){
      prev_state[entry.variable_id] = entry.precondition_value;
    }
    return prev_state;
  }
  
  
  
PatternDatabase::PatternDatabase(const TNFTask &task, const Pattern &pattern)
    : projection(task, pattern) {
    /*
      We want to compute goal distances for all abstract states in the
      projected task. To do so, we start by assuming every abstract state has
      an infinite goal distance and then do a backwards uniform cost search
      updating the goal distances of all encountered states.

      Instead of searching on the actual states, we use perfect hashing to
      run the search on the hash indices of states. To go from a state s to its
      index use rank(s) and to go from an index i to its state use unrank(i).
    */
    const TNFTask &projected_task = projection.get_projected_task();
    distances.resize(projected_task.get_num_states(), numeric_limits<int>::max());

    /*
      Priority queues usually order entries so the largest entry is the first.
      By using the comparator greater<T> instead of the default less<T>, we
      change the ordering to sort the smallest element first.
    */
    priority_queue<QueueEntry, vector<QueueEntry>, greater<QueueEntry>> queue;
    /*
      Note that we start with the goal state to turn the search into a regression.
      We also have to switch the role of precondition and effect in operators
      later on. This is sufficient to turn the search into a regression since
      the task is in TNF.
    */

    queue.push({0, projection.rank_state(projected_task.goal_state)});
    
    distances[projection.rank_state(projected_task.goal_state)] = 0;
    // TODO: add your code for exercise (b) here.
    while(!queue.empty()){
      int distance = queue.top().first;
      int index = queue.top().second;
      //cout << "Running " << index << " with cost: " << distance << endl;
      TNFState state = projection.unrank_state(index);
      queue.pop();
      for(TNFOperator op : projected_task.operators){
	if(op_reaches_state(op, state)){
	  int new_index = projection.rank_state(get_precondition_state(op, state));
	  if(distances[new_index] > distance+1){
	    distances[new_index] = distance+1;
	    queue.push({distance+1, new_index});
	  }
	}
      }
      }
}

int PatternDatabase::lookup_distance(const TNFState &original_state) const {
    TNFState abstract_state = projection.project_state(original_state);
    int index = projection.rank_state(abstract_state);
    return distances[index];

}

}
