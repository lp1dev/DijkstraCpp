#define DEBUG 1
#define OUTPUT std::cout << "\t"
#define GRN "\033[32;32m"
#define STD "\033[0;0m"

#include "dijkstra.h"

Dijkstra::Dijkstra(const Graph *graph, const vector<double> *arc_lengths) : graph(graph), arc_lengths(arc_lengths) {
    for (int i = 0; i <= arc_lengths->size(); i++) {
        parentarcs.push_back(-1);
        distances.push_back(infinity);
        resolvedTargets.push_back(0);
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

void Dijkstra::RunForTarget(int seeked_target, int source, int target, double totalDistance,
                            int targetId) {
    if (!resolvedTargets[targetId] && source != target) {
        if (DEBUG) {
            OUTPUT << "/////\n\tFrom: (" << source << ") to (" << target << ") totalDistance :" << totalDistance
                   << std::endl;
            OUTPUT << "(seeked_target : " << seeked_target << ")\n";
        }
        //Retrieving all arcs coming from the source
        vector<int> arcsComingFrom = graph->ArcsComingFrom(source);
        for (const int arc: arcsComingFrom) {
            if (resolvedTargets[targetId])
                return;
            target = graph->targets[arc];
            if (DEBUG){
                OUTPUT << "passing by target " << target << std::endl;
            }
            double distance = this->arc_lengths->at(arc);
            //If the distance of this arc is longer than the distance with the target we have, we stop here
            if (distance >= distances[target]) {
                if (DEBUG) {
                    OUTPUT << "Distance " << distance <<
                           " found is bigger than " << distances[target] <<
                           " to go to target " << target << std::endl;
                }
                continue;
            }
            if (DEBUG) {
                OUTPUT << "[" << arc << "]\n";
                OUTPUT << "(" << source << ") -> (" << target << ") = " << distance << std::endl;
            }
            //We add the target to reachedNodes if it hasn't been reached already
            if (distances[target] == infinity)
                reachedNodes.push_back(target);
            //If the total distance + the distance of this arc is >= the stored distance, we stop
            if ((totalDistance + distance) >= distances[target])
                continue;
            totalDistance += distance;
            if (DEBUG)
                OUTPUT << "total distance is now : " << totalDistance << std::endl;
            distances[target] = totalDistance;
            if (DEBUG)
                OUTPUT << "assert distances[" << target << "] = " << distances[target] << std::endl;
            //We add the arc to parentarcs
            parentarcs[target] = arc;
            //And keep seeking until we find the last node to the target
            if (target == seeked_target) {
                if (DEBUG) OUTPUT << GRN << "[Target " << target << " found]\n" << STD;
                resolvedTargets[targetId] = 1;
                return;
            } else
                RunForTarget(seeked_target, target, seeked_target, totalDistance, targetId);
        }
    }
}

void Dijkstra::RunUntilAllTargetsAreReached(int source, const vector<int> &targets) {
    //Initialising the distance to the source to 0
    distances[source] = 0;
    reachedNodes.push_back(source);
    vector<int> targetsToFind = targets.empty() ? graph->targets : targets;
    //
    if (DEBUG) {
        OUTPUT << "--------------------------\n";
        OUTPUT << "Distance with " << source << " = 0\n";
    }
    //Running for each target

    for (int i = 0; i < targetsToFind.size(); i++) {
        if (DEBUG) OUTPUT << "Seeking target " << targetsToFind[i] << std::endl;
        RunForTarget(targetsToFind[i], source, targetsToFind[i], 0, i);
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
