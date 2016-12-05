
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdlib.h>
#include "graph.h"

class Road {
public:
    std::string name;
    int priority;
    int max_speed;
    int num_lanes;
    int oneway;
    int numpoints;
    int numnodes;
    int recurringNodes;
    std::map<std::pair<float, float>, int> points;

    Road(std::string name, int priority, int max_speed, int num_lanes, int oneway, int numpoints,
         std::map<std::pair<float, float>, int> points, int numnodes, int recurringNodes) :
            name(name), priority(priority), max_speed(max_speed), num_lanes(num_lanes), oneway(oneway),
            numpoints(numpoints), points(points), numnodes(numnodes), recurringNodes(recurringNodes) {
    }
};

int getArcsDurationSum(std::vector<Road> roads) {
    return 0;
}

Graph parseFile(char *filename) {
    Graph graph;
    std::vector<Road> roads;
    std::ifstream classFile(filename);
    std::string lineBuffer, buffer;
    std::map<std::pair<float, float>, int> points;
    std::map<std::pair<float, float>, int> nodes;

    while (getline(classFile, lineBuffer)) {
        int i = 0;
        int originalNodeSize = (int) nodes.size();
        int recurringNodes = 0;
        std::string name;
        std::stringstream stream;
        std::vector<float> params;
        std::map<std::pair<float, float>, int> roadPoints;
        //
        params.clear();
        roadPoints.clear();
        stream.clear();
        stream.str(lineBuffer);
        //
        while (getline(stream, buffer, ',')) {
            if (0 == i++)
                name = buffer;
            else {
                params.push_back(atof(buffer.c_str()));
            }
        }
        for (int j = 5; j < params.size(); j += 2) {
            bool exists = false;
            std::pair<float, float> pair = std::make_pair(params[j], params[j + 1]);
            points[pair]++;
            roadPoints[pair]++;
            if (j == 5 || j == (params.size() - 2) || points[pair] > 1) {
                graph.AddArc(graph.NumNodes(), graph.NumNodes() + 1);
                nodes[std::make_pair(params[j], params[j + 1])]++;
            }
        }
        Road road = Road(name, (int) params[0], (int) params[1], (int) params[2], (int) params[3], (int) params[4],
                         roadPoints, (int) nodes.size() - originalNodeSize, recurringNodes);
        roads.push_back(road);
    }
    std::cout << nodes.size() << std::endl;
    std::cout << graph.NumArcs() << std::endl;
    std::cout << getArcsDurationSum(roads) << std::endl;
    return graph;
}

int main(int argc, char **argv) {
    std::map<std::pair<float, float>, int> points;
    if (argc < 2)
        return -1;
    parseFile(argv[1]);
    return 0;
}