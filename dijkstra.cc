#define DEBUG 1
#define OUTPUT std::cout << "\t\t\t"
#define GRN "\033[32;32m"
#define STD "\033[0;0m"

#include "dijkstra.h"

Dijkstra::Dijkstra(const Graph *graph, const vector<double> *arc_lengths) : graph(graph), arc_lengths(arc_lengths) {
    for (int i = 0; i <= arc_lengths->size(); i++) {
        parentarcs.push_back(-1);
        distances.push_back(infinity);
    }
}

const Graph &Dijkstra::GetGraph() const {
    return *graph;
}

double Dijkstra::GetDistance(int source, int target) const {
    int id = graph->GetArcId(source, target);
    return arc_lengths->at(id);
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
    for (const int arc: graph->ArcsComingFrom(source)) {
        int arc_target = graph->targets[arc];
        double arc_length = this->arc_lengths->at(arc);
        //We add the target to reachedNodes if it hasn't been reached already
        if (arc_length < lowest_distance) {
            lowest_distance = arc_length;
            closest_target = arc_target;
        }
        if (distances[arc_target] == infinity) {
            if (DEBUG) { OUTPUT << "(" << source << ") -> (" << arc_target << ") = " << arc_length << std::endl; }
            reachedNodes.push_back(arc_target);
            if (arc_length + totalDistance < distances[arc_target]) {
                distances[arc_target] = arc_length + totalDistance;
                if (DEBUG) OUTPUT << "assert distances[" << arc_target << "] = " << distances[arc_target] << std::endl;
                //We add the arc to parentarcs
                parentarcs[arc_target] = arc;
                //And keep seeking until we find the last node to the target
                if (seeked_target == arc_target) {
                    distances[seeked_target] = arc_length + totalDistance;
                    OUTPUT << GRN << "[Target " << seeked_target << " found]\n" << STD;
                    return;
                }
            }
            RunForTarget(seeked_target, closest_target, seeked_target, totalDistance + lowest_distance);
        }

    }
    OUTPUT << "lowest distance found " << lowest_distance << " closest : " << closest_target << std::endl;
}

void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int> &targets) {
    //Initialising the distance to the source to 0
    distances[source] = 0;
    reachedNodes.push_back(source);
    vector<int> targetsToFind = targets.empty() ? graph->targets : targets;
    //Running for each target
    for (int i = 0; i < targetsToFind.size(); i++) {
        if (DEBUG) OUTPUT << "\n    Seeking target " << targetsToFind[i] << " from " << source << std::endl;
        if (targetsToFind[i] == source)
            distances[source] = 0;
        else
            RunForTarget(targetsToFind[i], source, targetsToFind[i], 0);
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
}
