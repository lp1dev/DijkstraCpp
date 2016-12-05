# DijkstraCpp

## Recursive Bidirectional Dijkstra C++ implementation

### Please note : The server can count the number of nodes and arcs of a given csv files but nothing much, I unfortunately didn't have time to implement the other steps and instead stayed focused on the bidirectional Dijkstra

## About the Dijkstra algorithm

I wanted to try to do a recursive Dijkstra implementation since I thought this problem would be interesting to solve using recursion instead of priority queues as it is more usually done and it seemed to me that a recursive method would be a good design for it.

So far it is not a perfect implementation since I couldn't manage to pass all the stress tests. I've been able to notice a performance difference when I rewrote the Dijkstra to be bi-directional, but I'm unfortunately not able to quantify it for the moment

I've designed my bidirectional implementation so I'd easily be able more directions from which to start seeking, in order to evolve to a multi-direction Dijkstra or even Concurrent Hierarchies in the future.

In a nutshell, the algorithm now looks like this :

```
while [there is a target to find in Targets]:
      - run the recursive Dijkstra searches from a chosen point to another one:
      	(source -> target and target-> source for instance)
      - store each search's data inside its own container with a unique ID
      - if a node is shared between two distances container, we found a path the target and store the given distance
```