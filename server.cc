#include "graph.h"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdlib.h>

int server_debug = 1;

bool isNode(std::pair<double, double> arc, int index, int size){
    if (index == 0 || index == size)
        return true;

    return false;
}

void initNodes(){

}

void initGraph(Graph *graph, std::vector<double> posBuffer){
    for (int i = 0; (i + 1) < posBuffer.size(); i+=2){
        double from = posBuffer[i];
        double to =  posBuffer[i + 1];
//        std::cout << posBuffer[i + 1];
        graph->AddArc(2, 1);
    }
}

std::vector<double> parseFile(char *filename) {
    std::ifstream classFile(filename);
    std::string lineBuffer, buffer;
    std::vector<double> paramsBuffer;

    while (getline(classFile, lineBuffer)) {
        std::stringstream stream;
        stream.clear();
        stream.str(lineBuffer);
        //
        while (getline(stream, buffer, ',')) {
            paramsBuffer.push_back(atof(buffer.c_str()));
        }
    }
    if (server_debug)
        std::cout << paramsBuffer.size() / 2 << " points found\n";
    return paramsBuffer;
}

int main(int argc, char **argv){
    Graph graph;
    std::vector<double> posBuffer;
    if (argc < 2)
        return -1;
    posBuffer = parseFile(argv[1]);
//    initGraph(&graph, posBuffer);
    std::cout << graph.NumNodes() << " nodes found\n";
}
