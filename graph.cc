#include "graph.h"

#include "base.h"

int Graph::AddArc(int from, int to) {
  CHECK_GE(from, 0);
  CHECK_GE(to, 0);
  AddNode(std::max(from, to));
  const int arc_index = NumArcs();
  outgoing_arcs_[from].push_back(arc_index);
  incoming_arcs_[to].push_back(arc_index);
  tail_.push_back(from);
  head_.push_back(to);
  return arc_index;
}

bool Graph::IsNode(std::pair<double, double> arc, int index, int size){
  if (index == 0)
    return true;
  if (outgoing_arcs_[arc.first].size() >= 1  &&
          incoming_arcs_[arc.second].size() >= 1)
    return true;
  return false;
}

void Graph::AddNode(int node) {
  if (NumNodes() <= node) {
    outgoing_arcs_.resize(node + 1);
    incoming_arcs_.resize(node + 1);
  }
}
