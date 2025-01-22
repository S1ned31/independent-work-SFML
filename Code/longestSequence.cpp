#include "longestSequence.h"
#include "design.h"

sf::Color softGreenColor(199, 233, 192);

std::vector<int> findLongestDecreasingSubsequence(const std::vector<int>& sequence) {
    int n = sequence.size();
    std::vector<int> dp(n, 1), prev(n, -1);
    int maxLength = 0, maxIndex = -1;

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (sequence[i] < sequence[j] && dp[i] < dp[j] + 1) {
                dp[i] = dp[j] + 1;
                prev[i] = j;
            }
        }
        if (dp[i] > maxLength) {
            maxLength = dp[i];
            maxIndex = i;
        }
    }

    std::vector<int> longestSubsequence;
    for (int i = maxIndex; i != -1; i = prev[i]) {
        longestSubsequence.push_back(sequence[i]);
    }
    std::reverse(longestSubsequence.begin(), longestSubsequence.end());
    return longestSubsequence;
}

// Реализация логики окна SFML
void openLongestSequenceWindow(sf::Font& font) {
    sf::RenderWindow window(sf::VideoMode(800, 600), L"Найдите самую длинную убывающую последовательность");

    sf::Text inputLabel(L"Введите последовательность чисел через пробел:", font, 16);
    inputLabel.setPosition(10, 10);

    sf::RectangleShape calculateButton;
    sf::Text calculateButtonText;
    createButton(calculateButton, calculateButtonText, font, L"Найти последовательность", { 300, 50 }, { 10, 100 }, sf::Color::Green, 16);

    sf::Text inputDisplay("", font, 16);
    inputDisplay.setPosition(10, 50);

    sf::Text outputDisplay("", font, 16);
    outputDisplay.setPosition(10, 200);

    std::wstring inputText;
    bool isTyping = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !inputText.empty()) {
                    inputText.pop_back(); // Удалить последний символ
                }
                else if (event.text.unicode < 128) {
                    inputText += static_cast<wchar_t>(event.text.unicode);
                }
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (calculateButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    // Обработка последовательности
                    std::wistringstream iss(inputText);
                    std::vector<int> sequence;
                    int number;

                    while (iss >> number) {
                        sequence.push_back(number);
                    }

                    std::vector<int> longestSubsequence = findLongestDecreasingSubsequence(sequence);

                    // Формируем вывод
                    std::wostringstream output;
                    output << L"Длина последовательности: " << longestSubsequence.size() << L"\n";
                    output << L"Члены последовательности: ";
                    for (int num : longestSubsequence) {
                        output << num << L" ";
                    }

                    outputDisplay.setString(output.str());
                }
            }
        }

        window.clear(softGreenColor);
        window.draw(inputLabel);
        window.draw(calculateButton);
        window.draw(calculateButtonText);
        inputDisplay.setString(inputText);
        window.draw(inputDisplay);
        window.draw(outputDisplay);
        window.display();
    }
}