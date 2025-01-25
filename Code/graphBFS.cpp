#include "graphBFS.h"

Graph::Graph(bool isDirected) : directed(isDirected), edgeCount(0) {}

void Graph::addEdge(int u, int v, int weight) {
    addEdge(u, v);
    edgeWeights[{u, v}] = weight;
    if (!directed) {
        edgeWeights[{v, u}] = weight;
    }
}

void Graph::addEdge(int u, int v) {
    adjList[u].push_back(v);
    if (!directed) {
        adjList[v].push_back(u);
    }
    edgeCount++;
}

void Graph::breadthFirstSearch(int startVertex) {
    unordered_map<int, bool> visited;
    searchOrder.clear();
    vertexOrder.clear();
    currentOrder = 1;

    queue<int> q;
    q.push(startVertex);
    visited[startVertex] = true;

    while (!q.empty()) {
        int vertex = q.front();
        q.pop();
        searchOrder.push_back(vertex);
        vertexOrder[vertex] = currentOrder++;

        for (int neighbor : adjList[vertex]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    for (const auto& entry : adjList) {
        if (!visited[entry.first]) {
            q.push(entry.first);
            visited[entry.first] = true;

            while (!q.empty()) {
                int vertex = q.front();
                q.pop();
                searchOrder.push_back(vertex);
                vertexOrder[vertex] = currentOrder++;

                for (int neighbor : adjList[vertex]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
}

void Graph::calculateNodePositions(unordered_map<int, sf::Vector2f>& positions, float width, float height) {
    int radius = 20;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> disX(radius, width - radius);
    uniform_real_distribution<> disY(radius, height - radius);

    for (const auto& entry : adjList) {
        int vertex = entry.first;
        float x = disX(gen);
        float y = disY(gen);
        positions[vertex] = sf::Vector2f(x, y);
    }

    for (int i = 0; i < 1000; ++i) {
        for (const auto& entry : adjList) {
            int vertex = entry.first;
            for (int neighbor : entry.second) {
                if (vertex == neighbor) continue;

                sf::Vector2f direction = positions[neighbor] - positions[vertex];
                float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
                if (distance == 0) continue;

                float force = (distance - 100.0f) / distance;
                positions[vertex] += force * direction * 0.01f;
                positions[neighbor] -= force * direction * 0.01f;
            }
        }
    }
}

void Graph::visualizeDijkstra(int startVertex, int endVertex) {
    vector<int> path;
    dijkstra(startVertex, endVertex, path);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Візуалізація алгоритму Dijkstra");
    unordered_map<int, sf::Vector2f> positions;
    calculateNodePositions(positions, window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        drawGraph(window, positions);

        for (size_t i = 0; i < path.size() - 1; ++i) {
            sf::Vector2f from = positions[path[i]];
            sf::Vector2f to = positions[path[i + 1]];

            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(from.x + 20, from.y + 20), sf::Color::Blue),
                sf::Vertex(sf::Vector2f(to.x + 20, to.y + 20), sf::Color::Blue)
            };
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }
}

void Graph::visualizeBellmanFord(int startVertex, int endVertex) {
    vector<int> path;
    if (!bellmanFord(startVertex, endVertex, path)) return;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Візуалізація алгоритму Bellman-Ford");
    unordered_map<int, sf::Vector2f> positions;
    calculateNodePositions(positions, window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        drawGraph(window, positions);

        for (size_t i = 0; i < path.size() - 1; ++i) {
            sf::Vector2f from = positions[path[i]];
            sf::Vector2f to = positions[path[i + 1]];

            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(from.x + 20, from.y + 20), sf::Color::Red),
                sf::Vertex(sf::Vector2f(to.x + 20, to.y + 20), sf::Color::Red)
            };
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }
}

void Graph::drawGraph(sf::RenderWindow& window, const unordered_map<int, sf::Vector2f>& positions) {
    int radius = 20;
    for (const auto& entry : adjList) {
        int vertex = entry.first;
        sf::Vector2f position = positions.at(vertex);

        sf::CircleShape circle(radius);
        circle.setPosition(position);
        circle.setFillColor(sf::Color::Green);
        window.draw(circle);

        sf::Text text;
        sf::Font font;
        if (!font.loadFromFile("Fonts/static/Roboto-Bold.ttf")) {
            cerr << "Помилка завантаження шрифта!" << endl;
            return;
        }
        text.setFont(font);
        text.setString(to_string(vertex));
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setPosition(position.x + radius / 2, position.y + radius / 2);
        window.draw(text);

        for (int neighbor : entry.second) {
            sf::Vector2f neighborPosition = positions.at(neighbor);
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(position.x + radius, position.y + radius)),
                sf::Vertex(sf::Vector2f(neighborPosition.x + radius, neighborPosition.y + radius))
            };
            window.draw(line, 2, sf::Lines);
        }
    }
}

void Graph::dijkstra(int startVertex, int endVertex, vector<int>& path) {
    unordered_map<int, float> distances;
    unordered_map<int, int> previous;
    set<pair<float, int>> priorityQueue;

    for (const auto& entry : adjList) {
        distances[entry.first] = numeric_limits<float>::infinity();
        previous[entry.first] = -1;
    }
    distances[startVertex] = 0;
    priorityQueue.insert({ 0, startVertex });

    while (!priorityQueue.empty()) {
        int current = priorityQueue.begin()->second;
        priorityQueue.erase(priorityQueue.begin());

        if (current == endVertex) break;

        for (int neighbor : adjList[current]) {
            float weight = 1.0f; 
            float newDistance = distances[current] + weight;

            if (newDistance < distances[neighbor]) {
                priorityQueue.erase({ distances[neighbor], neighbor });
                distances[neighbor] = newDistance;
                previous[neighbor] = current;
                priorityQueue.insert({ newDistance, neighbor });
            }
        }
    }

    path.clear();
    for (int at = endVertex; at != -1; at = previous[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
}

bool Graph::bellmanFord(int startVertex, int endVertex, vector<int>& path) {
    unordered_map<int, int> distance;
    unordered_map<int, int> predecessor;

    for (const auto& entry : adjList) {
        distance[entry.first] = INT_MAX;
        predecessor[entry.first] = -1;
    }
    distance[startVertex] = 0;

    int vertexCount = adjList.size();
    for (int i = 0; i < vertexCount - 1; ++i) {
        for (const auto& entry : adjList) {
            int u = entry.first;
            for (int v : entry.second) {
                int weight = edgeWeights[{u, v}];
                if (distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                    predecessor[v] = u;
                }
            }
        }
    }

    for (const auto& entry : adjList) {
        int u = entry.first;
        for (int v : entry.second) {
            int weight = edgeWeights[{u, v}];
            if (distance[u] != INT_MAX && distance[u] + weight < distance[v]) {
                cerr << "Виявленно від'ємний цикл!" << endl;
                return false;
            }
        }
    }

    path.clear();
    for (int at = endVertex; at != -1; at = predecessor[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    return true;
}

void runVisualization() {
    Graph graphDijkstra(false);
    Graph graphBellmanFord(false);

    graphDijkstra.addEdge(0, 1);
    graphDijkstra.addEdge(1, 2);
    graphDijkstra.addEdge(2, 3);
    graphDijkstra.addEdge(3, 4);

    graphBellmanFord.addEdge(0, 1);
    graphBellmanFord.addEdge(1, 2);
    graphBellmanFord.addEdge(2, 3);
    graphBellmanFord.addEdge(3, 4);

    thread dijkstraThread([&]() { graphDijkstra.visualizeDijkstra(0, 4); });
    thread bellmanFordThread([&]() { graphBellmanFord.visualizeBellmanFord(0, 4); });

    dijkstraThread.join();
    bellmanFordThread.join();
}
