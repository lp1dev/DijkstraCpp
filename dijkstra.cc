#define DEBUG 1
#define OUTPUT std::cout << "\t\t\t"
#define GRN "\033[32;32m"
#define STD "\033[0;0m"

#include "dijkstra.h"

Dijkstra::Dijkstra(const Graph *graph, const vector<double> *arc_lengths) : graph(graph), arc_lengths(arc_lengths) {
    for (int i = 0; i <= graph->NumNodes(); i++) {
        parentarcs.push_back(-1);
        distancesFromSource.push_back(infinity);
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

void Dijkstra::RunForSourceFromTarget(int seeked_source, int source, int target, double totalDistance) {

}

void Dijkstra::RunForTargetFromSource(int seeked_target, int source, int target, double totalDistance) {
    if (seeked_target == target)
        for (int found_target : foundTargets)
            if (found_target == seeked_target && totalDistance > distancesFromSource[seeked_target]) {
                return;
            }
    for (const int arc: graph->OutgoingArcs(source)) {
        int arc_target = graph->Head(arc);
        if (DEBUG) OUTPUT << "Arc(" << source << ")->(" << arc_target << ")\n";
        double arc_length = this->arc_lengths->at(arc);

        //If this path is longer or equal than the one we stored, we skip
        if (arc_length + totalDistance >= distancesFromSource[arc_target])
            continue;
        //If we did not reach the arc_target node already
        if (distancesFromSource[arc_target] == infinity) {
            reachedNodes.push_back(arc_target);
            if (DEBUG) {
                OUTPUT << "discovered (" << source << ") -> (" << arc_target << ") = " << arc_length << std::endl;
            }
        }
        distancesFromSource[arc_target] = arc_length + totalDistance;
        if (DEBUG) OUTPUT << "assert distancesFromSource[" << arc_target << "] = " << distancesFromSource[arc_target] << std::endl;
        //We add the arc to parentarcs
        parentarcs[arc_target] = arc;
        //And keep seeking until we find the last node to the target
        if (seeked_target == arc_target) {
            if (DEBUG) std::cout << GRN << "[Found target " << seeked_target << "]\n" << STD;
            foundTargets.push_back(seeked_target);
        }
        RunForTargetFromSource(seeked_target, arc_target, seeked_target, totalDistance + arc_length);
    }
}

void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int> &targets) {
    // Clean up the last Dijkstra run, sparsely.
    for (const int node : reachedNodes) {
        distancesFromSource[node] = infinity;
        parentarcs[node] = -1;
    }
    reachedNodes.clear();
    foundTargets.clear();

    //Initialising the distance to the source to 0
    distancesFromSource[source] = 0;
    reachedNodes.push_back(source);
    vector<int> targetsToFind = targets.empty() ? graph->head_ : targets;
    remainingTargets = targetsToFind.size();

    //Running for each target
    for (int i = 0; i < targetsToFind.size(); i++) {
        if (targetsToFind[i] == source)
            distancesFromSource[source] = 0;
        else {
            if (i % 2)
                RunForTargetFromSource(targetsToFind[i], source, targetsToFind[i], 0);
            else
                RunForTargetFromSource(targetsToFind[i], source, targetsToFind[i], 0);
        }
    }
    //Displaying the distancesFromSource if in DEBUG
    if (DEBUG) {
        OUTPUT << "Distances : \n\t";
        for (int i = 0; i < distancesFromSource.size(); i++) std::cout << "[" << distancesFromSource[i] << "]";
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
    return distancesFromSource;
}

const vector<int> &Dijkstra::ParentArcs() const {
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
