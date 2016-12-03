#define DEBUG 1
#define OUTPUT std::cout << "\t"
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

void Dijkstra::RunForTarget(int seeked_source, int seeked_target, int source, int target, double totalDistance,
                            int targetId) {
    if (!resolvedTargets[targetId]) {
        if (DEBUG) {
            OUTPUT << "/////\n\tFrom: (" << source << ") to (" << target << ") totalDistance :" << totalDistance
                   << std::endl;
        }
        //Retrieving outgoing arcs from source
        vector<int> arcsComingFrom = graph->ArcsComingFrom(source);
        for (const int arc: arcsComingFrom) {
            target = graph->targets[arc];
            double distance = this->arc_lengths->at(arc);
            //If the distance we computed is longer, we stop here
            if (distance >= distances[target]) {
                if (DEBUG){
                    OUTPUT << "Distance " << distance << " found is bigger than " << distances[source];
                }
                return;
            }
            if (DEBUG) {
                OUTPUT << "[" << arc << "]\n";
                OUTPUT << "(" << source << ") -> (" << target << ") = " << distance << std::endl;
            }
            totalDistance += distance;
            //We add the target to reachedNodes if it hasn't been reached already
            if (distances[target] == infinity)
                reachedNodes.push_back(target);
            if (DEBUG && distances[target] != infinity) {
                OUTPUT << "! Resetting same distance distance[" << target << "]\n";
            }
            if (source == seeked_source)
                distances[target] = distance;
            else
                distances[target] = totalDistance;

            //
            if (DEBUG)
                OUTPUT << "assert distances[" << source << "] = " << distances[target] << std::endl;
            //We add the arc to parentarcs
            parentarcs[target] = arc;
            //And keep seeking until we find the last node to the target
            if (target == seeked_target) {
                if (DEBUG) OUTPUT << GRN << "[Target " << target << " found]\n" << STD;
                resolvedTargets[targetId] = 1;
            } else
                RunForTarget(seeked_source, seeked_target, source, target, totalDistance, targetId);
        }
    }
}

void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int> &targets) {
    //Initialising the distance to the source to 0
    distances[source] = 0;
    reachedNodes.push_back(source);
    //
    if (DEBUG) {
        OUTPUT << "--------------------------\n";
        OUTPUT << "Distance with " << source << " = 0\n";
    }
    //Running for each target
    for (int i = 0; i < targets.size(); i++) {
        resolvedTargets.push_back(0);
        RunForTarget(source, targets[i], source, targets[i], 0, i);
    }
    //Displaying the distances in DEBUG mode
    if (DEBUG) {
        OUTPUT << "Distances : \n\t";
        for (int i = 0; i < distances.size(); i++) std::cout << "[" << distances[i] << "]";
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
