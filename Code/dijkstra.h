#pragma once
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <utility>

class Dijkstra {
public:
    using GraphType = std::unordered_map<int, std::vector<std::pair<int, int>>>; // {узел: {{сосед, вес}, ...}}

    Dijkstra(const GraphType& graph);

    std::vector<int> calculateShortestPath(int start, int end);

private:
    const GraphType& graph;
};

#endif // DIJKSTRA_H
