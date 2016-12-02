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

int Graph::GetArcId(int from, int to) const{
    for (int i = 0; i < tail_.size(); i++) {
        if (from == tail_[i] && to == head_[i])
            return i;
    }
    return -1;
}

void Graph::AddNode(int node) {
    if (NumNodes() <= node) {
        outgoing_arcs_.resize(node + 1);
        incoming_arcs_.resize(node + 1);
    }
}
