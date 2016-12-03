#include "graph.h"

#include "base.h"

int Graph::AddArc(int from, int to) {
    CHECK_GE(from, 0);
    CHECK_GE(to, 0);
    AddNode(std::max(from, to));
    const int arc_index = NumArcs();
    outgoing_arcs_[from].push_back(arc_index);
    incoming_arcs_[to].push_back(arc_index);
    sources.push_back(from);//sources = tail_
    targets.push_back(to);//targets = head_
    return arc_index;
}

int Graph::GetArcId(int from, int to) const{
    for (int i = 0; i < sources.size(); i++) {
        if (from == sources[i] && to == targets[i])
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
