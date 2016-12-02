#include "dijkstra.h"

int debug = 1;

Dijkstra::Dijkstra(const Graph *graph, const vector<double> *arc_lengths) {
    this->graph = graph;
    this->arc_lengths = arc_lengths;
    for (int i = 0; i <= arc_lengths->size(); i++) {
        parentarcs.push_back(-1);
    }
    if (debug) {
        for (int i = 0; i < arc_lengths->size(); i++) {
            std::cout << "[" << arc_lengths->at(i) << "]";
        }
    }
    std::cout << std::endl;
}

const Graph &Dijkstra::GetGraph() const {
    return *graph;
}

double Dijkstra::GetDistance(int source, int target) const {
    int id = graph->GetArcId(source, target);
    if (debug) {
        std::cout << "Arc id :" << id << std::endl;
        std::cout << "Arc length : " << arc_lengths->at(id) << std::endl;
    }
    return arc_lengths->at(id);
}

void Dijkstra::RunForTarget(int seeked_source, int source, int target, int totalDistance, int targetId) {
    if (!resolvedTargets[targetId]) {
        if (debug) {
            std::cout << "Source: " << source << std::endl;
            std::cout << "Target: " << target << std::endl;
        }
        //Retrieving outgoing arcs from source
        vector<int> outgoing_arcs = graph->OutgoingArcs(source);

        //For each arc we compute the distance to the source
        for (int i = 0; i < outgoing_arcs.size(); i++) {
            int arcFrom = graph->tail_[outgoing_arcs[i]];
            int arcTo = graph->head_[outgoing_arcs[i]];
            if (debug) {
                std::cout << "Outgoing arc from source id: " << outgoing_arcs[i] << std::endl;
                std::cout << "Outgoing arc from:" << arcFrom << std::endl;
                std::cout << "Outgoing arc to:" << arcTo << std::endl;
            }
            double distance = GetDistance(arcFrom, arcTo);
            totalDistance += distance;
            //We add totalDistance to distances
            while (distances.size() < target) distances.push_back(0);
            distances[arcTo] = totalDistance;
            //We add the arc to parentarcs
            parentarcs[arcTo] = outgoing_arcs[i];
            //And keep seeking until we find the last node to the target

            if (arcTo == target)
                resolvedTargets[targetId] = 1;
            else
                RunForTarget(seeked_source, arcTo, target, totalDistance, targetId);
        }
    }
}

void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int> &targets) {
    for (int i = 0; i < targets.size(); i++) {
        resolvedTargets.push_back(0);
        RunForTarget(source, source, targets[i], 0, i);
    }
    if (debug) {
        std::cout << "Distances :" << std::endl;
        for (int i = 0; i < distances.size(); i++) {
            std::cout << "[" << distances[i] << "]";
        }
        std::cout << std::endl;
    }
}

const vector<int> &Dijkstra::ReachedNodes() const {

}

const vector<double> &Dijkstra::Distances() const {
    return distances;
}

const vector<int> &Dijkstra::ParentArcs() const {
    if (debug) {
        std::cout << "ParentArcs :" << std::endl;
        for (int i = 0; i < parentarcs.size(); i++) {
            std::cout << "[" << parentarcs[i] << "]";
        }
        std::cout << std::endl;
    }
    return parentarcs;
}

vector<int> Dijkstra::ArcPathFromSourceTo(int node) const {

}
