#include <SFML/Graphics.hpp>
#include "Sort.h";
#include <algorithm>;
#include <iostream>
#include <sstream>

// Функція для пірамідального сортування
void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void displaySortedArray(const std::vector<int>& arr) {
    sf::RenderWindow resultWindow(sf::VideoMode(400, 300), L"Отсортированный массив");

    sf::Font font;
    if (!font.loadFromFile("Fonts/static/Roboto-Bold.ttf")) {
        std::cerr << L"Ошибка загрузки шрифта" << std::endl;
        return;
    }

    std::ostringstream oss;
    for (int num : arr) {
        oss << num << " ";
    }

    sf::Text resultText;
    resultText.setFont(font);
    resultText.setString(oss.str());
    resultText.setCharacterSize(12);
    resultText.setFillColor(sf::Color::White);
    resultText.setStyle(sf::Text::Bold);
    resultText.setPosition(50, 120);

    while (resultWindow.isOpen()) {
        sf::Event event;
        while (resultWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                resultWindow.close();
        }

        resultWindow.clear();
        resultWindow.draw(resultText);
        resultWindow.display();
    }
}
