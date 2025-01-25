#ifndef GRAPHBFS_H
#define GRAPHBFS_H

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iomanip>
#include <random>
#include <limits>
#include <climits>
#include <set>
#include <thread>
#include <SFML/Graphics.hpp>

using namespace std;

//////////////////////////////////////////////////
// Файл для роботи з графами
// Має прототипи функцій для 
// створення графу за допомогою
// BFS, та алгоритми для пошуку найкоротших шляхів
// Алгоритми Дейікстри та Белмана-Форда
//////////////////////////////////////////////////

class Graph {
public:
    Graph(bool isDirected);
    void addEdge(int u, int v);
    void addEdge(int u, int v, int weight);
    void breadthFirstSearch(int startVertex);
    void calculateNodePositions(unordered_map<int, sf::Vector2f>& positions, float width, float height);
    void visualizeDijkstra(int startVertex, int endVertex);
    void visualizeBellmanFord(int startVertex, int endVertex);
    void dijkstra(int startVertex, int endVertex, vector<int>& path);
    bool bellmanFord(int startVertex, int endVertex, vector<int>& path);
    void runVisualization();

    struct hash_pair {
        template <class T1, class T2>
        size_t operator()(const pair<T1, T2>& pair) const {
            auto hash1 = hash<T1>{}(pair.first);
            auto hash2 = hash<T2>{}(pair.second);
            return hash1 ^ hash2;
        }
    };

private:
    bool directed;
    int edgeCount;
    unordered_map<int, vector<int>> adjList;
    unordered_map<pair<int, int>, int, hash_pair> edgeWeights;
    vector<int> searchOrder;
    unordered_map<int, int> vertexOrder;
    int currentOrder;
    void drawGraph(sf::RenderWindow& window, const unordered_map<int, sf::Vector2f>& positions);
};

#endif // GRAPHBFS_H
