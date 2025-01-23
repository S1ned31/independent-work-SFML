#ifndef GRAPHBFS_H
#define GRAPHBFS_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iomanip>
#include <random>
#include <SFML/Graphics.hpp>

using namespace std;

class Graph {
public:
    Graph(bool isDirected);
    void addEdge(int u, int v);
    void breadthFirstSearch(int startVertex);
    void calculateNodePositions(unordered_map<int, sf::Vector2f>& positions, float width, float height);
    void visualizeGraph();

private:
    bool directed;
    int edgeCount;
    unordered_map<int, vector<int>> adjList;
    vector<int> searchOrder;
    unordered_map<int, int> vertexOrder;
    int currentOrder;
    void drawGraph(sf::RenderWindow& window, const unordered_map<int, sf::Vector2f>& positions);
};

#endif // GRAPHBFS_H
