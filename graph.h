#ifndef GRAPH_H_
#define GRAPH_H_

#include <string>
#include <vector>

using std::vector;
using std::string;

// In this graph, nodes are integers in 0..num_nodes-1, and arcs are also
// integers in 0..num_arcs-1.
// Thus, when you AddArc(4, 128), you implicitly declare the existence of
// at least 129 nodes (0..128).
class Graph {
public:
    // Return the arc index, which will be the number of times AddArc()
    // has been called before.
    int AddArc(int from, int to);

    // Optional: nodes are automatically added upon AddArc().
    void AddNode(int node);

    int NumNodes() const { return outgoing_arcs_.size(); }

    int NumArcs() const { return sources.size(); }

    // Gets the tail ("from") and head ("to") of an arc.
    int Tail(int arc) const { return sources[arc]; }

    int Head(int arc) const { return targets[arc]; }

    int GetArcId(int from, int to) const;


    // Returns a list of all the arc indices whose source is "from".
    const vector<int> &ArcsComingFrom(int from) const {
        return outgoing_arcs_[from];
    }

    // Returns a list of all the arc indices whose target is "to".
    const vector<int> &ArcsGoingTo(int to) const {
        return incoming_arcs_[to];
    }
    vector<int> sources;
    vector<int> targets;
private:
    vector<vector<int>> outgoing_arcs_;
    vector<vector<int>> incoming_arcs_;

};

#endif  // GRAPH_H_
