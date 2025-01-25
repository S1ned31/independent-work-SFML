#include "Dijkstra.h"

Dijkstra::Dijkstra(const GraphType& graph) : graph(graph) {}

std::vector<int> Dijkstra::calculateShortestPath(int start, int end) {
    std::unordered_map<int, int> dist; // Расстояния до узлов
    std::unordered_map<int, int> prev; // Предыдущий узел
    for (const auto& entry : graph) {
        dist[entry.first] = std::numeric_limits<int>::max();
        prev[entry.first] = -1;
    }
    dist[start] = 0;

    using pii = std::pair<int, int>; // {расстояние, вершина}
    std::priority_queue<pii, std::vector<pii>, std::greater<>> pq;
    pq.emplace(0, start);

    while (!pq.empty()) {
        auto [currentDist, currentVertex] = pq.top(); // Структурированные привязки
        pq.pop();

        if (currentDist > dist[currentVertex]) continue;

        for (const auto& [neighbor, weight] : graph.at(currentVertex)) { // Структурированные привязки
            int newDist = dist[currentVertex] + weight;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                prev[neighbor] = currentVertex;
                pq.emplace(newDist, neighbor);
            }
        }
    }

    // Восстановление пути
    std::vector<int> path;
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    if (path.size() == 1 && path[0] != start) {
        return {}; // Путь не найден
    }
    return path;
}
