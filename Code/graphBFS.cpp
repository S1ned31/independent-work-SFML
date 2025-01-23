#include "graphBFS.h"

Graph::Graph(bool isDirected) : directed(isDirected), edgeCount(0) {}

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

    // Инициализация случайного начального положения для узлов
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

    // Простейший алгоритм разметки узлов
    for (int i = 0; i < 1000; ++i) {
        for (const auto& entry : adjList) {
            int vertex = entry.first;
            for (int neighbor : entry.second) {
                if (vertex == neighbor) continue;

                sf::Vector2f direction = positions[neighbor] - positions[vertex];
                float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
                if (distance == 0) continue;

                // Притяжение соседних узлов
                float force = (distance - 100.0f) / distance;
                positions[vertex] += force * direction * 0.01f;
                positions[neighbor] -= force * direction * 0.01f;
            }
        }
    }
}

void Graph::visualizeGraph() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Graph Visualization");

    unordered_map<int, sf::Vector2f> positions;
    calculateNodePositions(positions, window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
        }

        window.clear();
        drawGraph(window, positions);
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
            cerr << "Ошибка загрузки шрифта!" << endl;
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
