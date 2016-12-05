#define DEBUG 0
#define OUTPUT std::cout << "\t\t\t"
#define GRN "\033[32;32m"
#define STD "\033[0;0m"
#define SOURCE 0
#define TARGET 1

#include "dijkstra.h"

Dijkstra::Dijkstra(const Graph *graph, const vector<double> *arc_lengths) : graph(graph), arc_lengths(arc_lengths) {
    distancesContainer.push_back(std::vector<double>());
    distancesContainer.push_back(std::vector<double>());
    reachedNodes.push_back(std::vector<int>());
    reachedNodes.push_back(std::vector<int>());
    parentarcs.push_back(std::vector<int>());
    parentarcs.push_back(std::vector<int>());
    for (int i = 0; i <= graph->NumNodes(); i++) {
        parentarcs[SOURCE].push_back(-1);
        parentarcs[TARGET].push_back(-1);
        distancesContainer[SOURCE].push_back(infinity);
        distancesContainer[TARGET].push_back(infinity);
    }
}

const Graph &Dijkstra::GetGraph() const {
    return *graph;
}

void Dijkstra::Run(int seeked_node, int source, int target, double totalDistance, int containerId) {
    if (seeked_node == target)
        for (int found_target : foundTargets)
            if (found_target == seeked_node && totalDistance > distancesContainer[containerId][seeked_node]) {
                return;
            }
    for (const int arc: graph->OutgoingArcs(source)) {
        int arc_target = graph->Head(arc);
        double arc_length = this->arc_lengths->at(arc);

        //If this path is longer or equal than the one we stored, we skip
        if (arc_length + totalDistance >= distancesContainer[containerId][arc_target])
            continue;

        //If the arc_target node is known by another container then have the distance to the seeked_node
        if (distancesContainer[containerId][arc_target] != infinity &&
            distancesContainer[!containerId][arc_target] != infinity) {
            double distancesFromTwoSides = totalDistance + distancesContainer[!containerId][arc_target];
            if (distancesFromTwoSides < distancesContainer[containerId][seeked_node]) {
                distancesContainer[containerId][seeked_node] = distancesFromTwoSides;
                foundTargets.push_back(seeked_node);
            }
        }

        //If we did not reach the arc_target node already
        if (distancesContainer[containerId][arc_target] == infinity) {
            reachedNodes[containerId].push_back(arc_target);
        }
        distancesContainer[containerId][arc_target] = arc_length + totalDistance;
        //We add the arc to parentarcs
        parentarcs[containerId][arc_target] = arc;
        //And keep seeking until we find the last node to the target
        if (seeked_node == arc_target) {
            foundTargets.push_back(seeked_node);
        }
        Run(seeked_node, arc_target, seeked_node, totalDistance + arc_length, containerId);
    }
}

void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int> &targets) {
    // Clean up the last Dijkstra run, sparsely.
    for (const int node : reachedNodes[0]) {
        distancesContainer[0][node] = infinity;
        distancesContainer[1][node] = infinity;
        parentarcs[SOURCE][node] = -1;
        parentarcs[TARGET][node] = -1;
    }
    reachedNodes[0].clear();
    reachedNodes[1].clear();
    foundTargets.clear();

    //Initialising the distances and the targets
    reachedNodes[0].push_back(source);
    vector<int> targetsToFind = targets.empty() ? graph->head_ : targets;
    distancesContainer[0][source] = 0;

    //Running for each target
    for (int i = 0; i < targetsToFind.size(); i++) {
        reachedNodes[1].push_back(targetsToFind[i]);
        if (targetsToFind[i] == source) {
            distancesContainer[0][source] = 0;
            distancesContainer[1][targetsToFind[i]] = 0;
        } else {
            Run(targetsToFind[i], source, targetsToFind[i], 0, 0);
            Run(source, targetsToFind[i], source, 0, 1);
        }
    }
    //Displaying the computed distances if in DEBUG
    if (DEBUG) {
        OUTPUT << "Distances : \n\t";
        for (int i = 0; i < distancesContainer[0].size(); i++) std::cout << "[" << distancesContainer[0][i] << "]";
        OUTPUT << std::endl;
    }
    if (DEBUG) {
        OUTPUT << "Reached nodes : \n\t";
        for (int i = 0; i < reachedNodes[0].size(); i++) std::cout << "[" << reachedNodes[0][i] << "]";
        OUTPUT << std::endl;
    }
}

const vector<int> &Dijkstra::ReachedNodes() const {
    return reachedNodes[0];
}

const vector<double> &Dijkstra::Distances() const {
    return distancesContainer[0];
}

const vector<int> &Dijkstra::ParentArcs() const {
    return parentarcs[SOURCE];
}

vector<int> Dijkstra::ArcPathFromSourceTo(int node) const {
    vector<int> arcs;
    for (;;) {
        const int a = parentarcs[SOURCE][node];
        if (a < 0) break;
        arcs.push_back(a);
        node = graph->Tail(a);
    }
    std::reverse(arcs.begin(), arcs.end());
    return arcs;
}
