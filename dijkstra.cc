#define DEBUG 1
#define OUTPUT std::cout
#include "dijkstra.h"

Dijkstra::Dijkstra(const Graph *graph, const vector<double> *arc_lengths) {
    this->graph = graph;
    this->arc_lengths = arc_lengths;
    for (int i = 0; i <= arc_lengths->size(); i++) {
        parentarcs.push_back(-1);
        distances.push_back(infinity);
    }
    if (DEBUG) {
        for (int i = 0; i < arc_lengths->size(); i++) {
            OUTPUT << "[" << arc_lengths->at(i) << "]";
        }
        OUTPUT << std::endl;
    }
}

const Graph &Dijkstra::GetGraph() const {
    return *graph;
}

double Dijkstra::GetDistance(int source, int target) const {
    int id = graph->GetArcId(source, target);
    if (DEBUG) {
        OUTPUT << "Arc id :" << id << std::endl;
        OUTPUT << "Arc length : " << arc_lengths->at(id) << std::endl;
    }
    return arc_lengths->at(id);
}

bool Dijkstra::IsNodeReached(int node) {
    for (int i = 0; i < reachedNodes.size(); i++) {
        if (reachedNodes[i] == node)
            return true;
    }
    return false;
}

void Dijkstra::RunForTarget(int seeked_source, int source, int target, double totalDistance, int targetId) {
    if (!resolvedTargets[targetId]) {
        if (DEBUG) {
            OUTPUT << "Source: " << source << std::endl;
            OUTPUT << "Target: " << target << std::endl;
        }

        //Retrieving outgoing arcs from source
        vector<int> outgoing_arcs = graph->OutgoingArcs(source);
        vector<int> incoming_arcs = graph->IncomingArcs(source);

        //For each arc we compute the distance to the source
        for (int i = 0; i < outgoing_arcs.size(); i++) {
            int arcFrom = graph->tail_[outgoing_arcs[i]];
            int arcTo = graph->head_[outgoing_arcs[i]];
            if (DEBUG) {
                OUTPUT << "Outgoing arc from source id: " << outgoing_arcs[i] << std::endl;
                OUTPUT << "Incoming arc from source id: " << incoming_arcs[i] << std::endl;
                OUTPUT << "Outgoing arc to:" << arcTo << std::endl;
            }
            double distance = GetDistance(arcFrom, arcTo);
            totalDistance += distance;
            distances[arcTo] = totalDistance; //We add totalDistance to distances
            if (DEBUG)
                OUTPUT << "assert distances[" << arcTo << "] = " << distances[arcTo] << std::endl;
            parentarcs[arcTo] = outgoing_arcs[i]; //We add the arc to parentarcs
            //We add the target to reachedNodes if it hasn't been reached already
            if (!IsNodeReached(outgoing_arcs[i]))
                reachedNodes.push_back(outgoing_arcs[i]);
            if (!IsNodeReached(incoming_arcs[i]))
                reachedNodes.push_back(incoming_arcs[i]);
            //And keep seeking until we find the last node to the target
            if (arcTo == target) {
                resolvedTargets[targetId] = 1;
            } else
                RunForTarget(seeked_source, arcTo, target, totalDistance, targetId);
        }
    }
}

void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int> &targets) {
    //Initialising the distance to the source to 0
    distances[source] = 0;
    if (DEBUG)
        OUTPUT << "Distance with " << source << " = 0\n";
    //Running for each target
    for (int i = 0; i < targets.size(); i++) {
        resolvedTargets.push_back(0);
        RunForTarget(source, source, targets[i], 0, i);
    }
    //Displaying the distances in DEBUG mode
    if (DEBUG) {
        OUTPUT << "Distances :" << std::endl;
        for (int i = 0; i < distances.size(); i++) {
            OUTPUT << "[" << distances[i] << "]";
        }
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
        OUTPUT << "ParentArcs :" << std::endl;
        for (int i = 0; i < parentarcs.size(); i++) {
            OUTPUT << "[" << parentarcs[i] << "]";
        }
        OUTPUT << std::endl;
    }
    return parentarcs;
}

vector<int> Dijkstra::ArcPathFromSourceTo(int node) const {

}
