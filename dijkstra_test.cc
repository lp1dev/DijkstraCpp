#include <iostream>
using namespace std;

#include "base.h"
#include "graph.h"
#include "dijkstra.h"

int main() {
  int num_tests = 0;

  // Ladder graph. 0->1->2->3->4.... Arcs all have cost 1.
  Graph ladder_graph;
  const int kLadderNumNodes = 10;
  vector<double> ladder_arc_lengths;
  for (int i = 1; i < kLadderNumNodes; ++i) {
    ladder_graph.AddArc(i - 1, i);
    ladder_arc_lengths.push_back(1);
  }
  // Test case: Only one Dijkstra exploration on the ladder graph.
  // Sparsity is NOT checked.
  {
    Dijkstra dijkstra(&ladder_graph, &ladder_arc_lengths);

    vector<int> targets;
    targets.push_back(7);
    dijkstra.RunUntilAllTargetsAreReached(3, targets);

    CHECK_EQ(dijkstra.GetGraph().NumArcs(), kLadderNumNodes - 1);

    CHECK_EQ(dijkstra.Distances()[7], 4);
    cout << "Test #" << num_tests++ << " PASSED" << endl;

    CHECK_EQ(dijkstra.Distances()[3], 0);
    CHECK_EQ(dijkstra.Distances()[4], 1);
    CHECK_EQ(dijkstra.Distances()[5], 2);
    CHECK_EQ(dijkstra.Distances()[6], 3);
    cout << "Test #" << num_tests++ << " PASSED" << endl;

    CHECK_EQ(dijkstra.ParentArcs()[7], 6);
    CHECK_EQ(dijkstra.ParentArcs()[6], 5);
    CHECK_EQ(dijkstra.ParentArcs()[5], 4);
    CHECK_EQ(dijkstra.ParentArcs()[4], 3);
    CHECK_EQ(dijkstra.ParentArcs()[3], -1);
    cout << "Test #" << num_tests++ << " PASSED" << endl;
  }

  // Bidirectional cycle:  ...<->N-1<->0<->1<->2<->3<->4<->...,
  // with arc costs: 0.1 from i to i+1 and 0.2 from i+1 to i.
  Graph cycle_graph;
  vector<double> cycle_arc_lengths;
  const int kCycleNumNodes = 10;
  for (int i = 0; i < kCycleNumNodes; ++i) {
    const int next = i == kCycleNumNodes - 1 ? 0 : i + 1;
    cycle_graph.AddArc(i, next);
    cycle_arc_lengths.push_back(0.1);
    cycle_graph.AddArc(next, i);
    cycle_arc_lengths.push_back(0.2);
  }

  // Test case: only one Dijkstra exploration. Sparsity not checked.
  {
    CHECK_EQ(kCycleNumNodes, 10);  // The numbers below only work for 10.

    Dijkstra dijkstra(&cycle_graph, &cycle_arc_lengths);

    vector<int> targets;
    targets.push_back(5);
    targets.push_back(6);

    dijkstra.RunUntilAllTargetsAreReached(9, targets);

    CHECK_EQ(dijkstra.GetGraph().NumArcs(), 20);
    CHECK_EQ(dijkstra.ReachedNodes().size(), 10);
    CHECK_EQ(dijkstra.Distances()[9], 0);
//    CHECK_NEAR(dijkstra.Distances()[0], 0.1, 1e-9);
//    CHECK_NEAR(dijkstra.Distances()[1], 0.2, 1e-9);
//    CHECK_NEAR(dijkstra.Distances()[2], 0.3, 1e-9);
//    CHECK_NEAR(dijkstra.Distances()[3], 0.4, 1e-9);
//    CHECK_NEAR(dijkstra.Distances()[4], 0.5, 1e-9);
//    CHECK_NEAR(dijkstra.Distances()[5], 0.6, 1e-9);
//    CHECK_NEAR(dijkstra.Distances()[6], 0.6, 1e-9);
//    CHECK_NEAR(dijkstra.Distances()[7], 0.4, 1e-9);
//    CHECK_NEAR(dijkstra.Distances()[8], 0.2, 1e-9);
    CHECK_EQ(dijkstra.ParentArcs()[9], -1);
    CHECK_EQ(dijkstra.ParentArcs()[0], 18);
    CHECK_EQ(dijkstra.ParentArcs()[1], 0);
    CHECK_EQ(dijkstra.ParentArcs()[2], 2);
    CHECK_EQ(dijkstra.ParentArcs()[3], 4);
    CHECK_EQ(dijkstra.ParentArcs()[4], 6);
    CHECK_EQ(dijkstra.ParentArcs()[5], 8);
    CHECK_EQ(dijkstra.ParentArcs()[6], 13);
    CHECK_EQ(dijkstra.ParentArcs()[7], 15);
    CHECK_EQ(dijkstra.ParentArcs()[8], 17);
    cout << "Test #" << num_tests++ << " PASSED" << endl;
  }

  // Messy graph.
  cout << "DONE for now -- BUT MORE TESTS ARE COMING SOON, TESTING MORE THINGS"
       << endl;
}
