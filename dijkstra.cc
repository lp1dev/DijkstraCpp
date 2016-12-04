#define DEBUG 1
#define OUTPUT std::cout << "\t\t\t"
#define GRN "\033[32;32m"
#define STD "\033[0;0m"

#include "dijkstra.h"

Dijkstra::Dijkstra(const Graph *graph, const vector<double> *arc_lengths) : graph(graph), arc_lengths(arc_lengths) {
    for (int i = 0; i <= graph->NumNodes(); i++) {
        parentarcs.push_back(-1);
        distances.push_back(infinity);
    }
}

const Graph &Dijkstra::GetGraph() const {
    return *graph;
}

bool Dijkstra::IsNodeReached(int node) {
    for (int i = 0; i < reachedNodes.size(); i++) {
        if (reachedNodes[i] == node)
            return true;
    }
    return false;
}

void Dijkstra::RunForTarget(int seeked_target, int source, int target, double totalDistance) {
    if (DEBUG) {
        std::cout << "\n\tFrom: (" << source << ") to (" << target << ") totalDistance : " << totalDistance
                  << std::endl;
        OUTPUT << "(seeked_target : " << seeked_target << ")\n\n";
    }
    int closest_target = target;
    double lowest_distance = infinity;
    if (totalDistance > distances[source])
        return;
    for (const int arc: graph->OutgoingArcs(source)) {
        int arc_target = graph->Head(arc);
        if (DEBUG) OUTPUT << "Arc(" << source << ")->(" << arc_target << ")\n";
        double arc_length = this->arc_lengths->at(arc);
        //We add the target to reachedNodes if it hasn't been reached already
        if (arc_length < lowest_distance) {
            if (DEBUG)
                OUTPUT << "New closest arc target is " << source << "->" << arc_target << "=" << arc_length
                       << std::endl;
            lowest_distance = arc_length;
            closest_target = arc_target;
        }

        //If this path is longer or equal than the one we stored, we skip
        if (arc_length + totalDistance >= distances[arc_target])
            continue;
        //If we did not reach the arc_target node already
        if (distances[arc_target] == infinity) {
            reachedNodes.push_back(arc_target);
            if (DEBUG) {
                OUTPUT << "discovered (" << source << ") -> (" << arc_target << ") = " << arc_length << std::endl;
            }
        }
        distances[arc_target] = arc_length + totalDistance;
        if (DEBUG) OUTPUT << "assert distances[" << arc_target << "] = " << distances[arc_target] << std::endl;
        //We add the arc to parentarcs
        parentarcs[arc_target] = arc;
        //And keep seeking until we find the last node to the target
        RunForTarget(seeked_target, arc_target, seeked_target, totalDistance + arc_length);
    }
}

void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int> &targets) {
    // Clean up the last Dijkstra run, sparsely.
    for (const int node : reachedNodes) {
        distances[node] = infinity;
        parentarcs[node] = -1;
    }
    reachedNodes.clear();

    //Initialising the distance to the source to 0
    distances[source] = 0;
    reachedNodes.push_back(source);
    vector<int> targetsToFind = targets.empty() ? graph->head_ : targets;
    remainingTargets = targetsToFind.size();

    //Running for each target
    for (int i = 0; i < targetsToFind.size(); i++) {
        if (targetsToFind[i] == source)
            distances[source] = 0;
        else {
            RunForTarget(targetsToFind[i], source, targetsToFind[i], 0);
        }
    }
    //Displaying the distances if in DEBUG
    if (DEBUG) {
        OUTPUT << "Distances : \n\t";
        for (int i = 0; i < distances.size(); i++) std::cout << "[" << distances[i] << "]";
        OUTPUT << std::endl;
    }
    if (DEBUG) {
        OUTPUT << "Reached nodes : \n\t";
        for (int i = 0; i < reachedNodes.size(); i++) std::cout << "[" << reachedNodes[i] << "]";
        OUTPUT << std::endl;
    }
}

const vector<int> &Dijkstra::ReachedNodes() const {
    return reachedNodes;
}

const vector<double> &Dijkstra::Distances() const {
    return distances;
}

const vector<int> &Dijkstra::ParentArcs() const {
    if (DEBUG) {
        OUTPUT << "ParentArcs :\n\t";
        for (int i = 0; i < parentarcs.size(); i++) {
            std::cout << "[" << parentarcs[i] << "]";
        }
        OUTPUT << std::endl;
    }
    return parentarcs;
}

vector<int> Dijkstra::ArcPathFromSourceTo(int node) const {
    vector<int> arcs;
    for (;;) {
        const int a = parentarcs[node];
        if (a < 0) break;
        arcs.push_back(a);
        node = graph->Tail(a);
    }
    std::reverse(arcs.begin(), arcs.end());
    return arcs;
}
