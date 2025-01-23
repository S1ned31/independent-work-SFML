#include "graphVisualization.h"

void displayGraphWindow(sf::Font& font) {
    sf::RenderWindow graphWindow(sf::VideoMode(800, 600), L"Граф BFS");
    sf::Text inputLabel(L"Введите количество узлов и рёбер (Пример: 5 7):", font, 16);
    inputLabel.setPosition(10, 10);

    sf::Text edgeInputLabel(L"Введите рёбра (Пример: 1 2):", font, 16);
    edgeInputLabel.setPosition(10, 55);

    sf::RectangleShape createNodesButton(sf::Vector2f(150, 40));
    createNodesButton.setPosition(10, 100);
    createNodesButton.setFillColor(sf::Color(154, 205, 50));

    sf::Text createNodesButtonText(L"Подтвердить узлы", font, 16);
    createNodesButtonText.setPosition(15, 110);

    sf::RectangleShape createEdgesButton(sf::Vector2f(150, 40));
    createEdgesButton.setPosition(10, 150);
    createEdgesButton.setFillColor(sf::Color(154, 205, 50));

    sf::Text createEdgesButtonText(L"Подтвердить рёбра", font, 16);
    createEdgesButtonText.setPosition(15, 160);

    sf::RectangleShape visualizeButton(sf::Vector2f(150, 40));
    visualizeButton.setPosition(10, 200);
    visualizeButton.setFillColor(sf::Color(154, 205, 50));

    sf::Text visualizeButtonText(L"Визуализировать", font, 16);
    visualizeButtonText.setPosition(15, 210);

    sf::Text resultLabel(L"", font, 16);
    resultLabel.setPosition(10, 250);

    std::wstring inputText;
    std::vector<Edge> edges;
    int nodeCount = 0;
    bool nodesConfirmed = false;

    while (graphWindow.isOpen()) {
        sf::Event event;
        while (graphWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                graphWindow.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (createNodesButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    std::wistringstream ss(inputText);
                    ss >> nodeCount;

                    if (nodeCount > 0) {
                        nodesConfirmed = true;
                        resultLabel.setString(L"Количество узлов подтверждено!");
                    }
                    inputText.clear();
                }
                else if (createEdgesButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (nodesConfirmed) {
                        edges.clear();
                        std::wistringstream ss(inputText);
                        int from, to;
                        while (ss >> from >> to) {
                            edges.push_back({ from, to });
                        }
                        resultLabel.setString(L"Рёбра подтверждены!");
                    }
                    else {
                        resultLabel.setString(L"Сначала подтвердите количество узлов.");
                    }
                    inputText.clear();
                }
                else if (visualizeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (nodesConfirmed && !edges.empty()) {
                        // Логика визуализации графа
                        resultLabel.setString(L"Граф визуализирован!");
                    }
                    else {
                        resultLabel.setString(L"Подтвердите узлы и рёбра перед визуализацией.");
                    }
                }
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !inputText.empty()) {
                    inputText.pop_back();
                }
                else if (event.text.unicode > 31 && event.text.unicode < 128) {
                    inputText += static_cast<wchar_t>(event.text.unicode);
                }
            }
        }

        graphWindow.clear(sf::Color(250, 220, 100));
        graphWindow.draw(inputLabel);
        graphWindow.draw(createNodesButton);
        graphWindow.draw(createNodesButtonText);
        graphWindow.draw(edgeInputLabel);
        graphWindow.draw(createEdgesButton);
        graphWindow.draw(createEdgesButtonText);
        graphWindow.draw(visualizeButton);
        graphWindow.draw(visualizeButtonText);

        sf::Text inputDisplay(sf::String(inputText), font, 16);
        inputDisplay.setPosition(10, 85);
        graphWindow.draw(inputDisplay);

        graphWindow.draw(resultLabel);
        graphWindow.display();
    }
}
