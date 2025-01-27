#include <SFML/Graphics.hpp>
#include "design.h"
#include "Sort.h"  
#include "hashTable.h"
#include "BTree.h"
#include "greedyAlgorithm.h"
#include "longestSequence.h"
#include "graphBFS.h"
#include "dijkstra.h"
#include "huffmanCompression.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unordered_map>

using namespace std;

void loadFromFile(const char* filename, HashTable& hashTable) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    char vin[20];
    char ownerSurname[50];
    char ownerName[50];
    char ownerPatronymic[50];
    char ownerInfo[150];

    while (file >> vin >> ownerSurname >> ownerName >> ownerPatronymic) {
        strcpy_s(ownerInfo, ownerSurname);
        strcat_s(ownerInfo, " ");
        strcat_s(ownerInfo, ownerName);
        strcat_s(ownerInfo, " ");
        strcat_s(ownerInfo, ownerPatronymic);
        hashTable.insert(vin, ownerInfo);
    }

    file.close();
}

// Функція для відображення геш-таблиці у новому вікні
void displayHashTable(sf::RenderWindow& window, HashTable& hashTable, sf::Font& font) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);

    int yPos = 20;
    // Получаем таблицу данных
    HashEntry* table = hashTable.getTable();
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (!table[i].isEmpty) {
            text.setString("VIN: " + std::string(table[i].key) + " | Owner: " + std::string(table[i].ownerInfo));
            text.setPosition(10, yPos);
            window.draw(text);
            yPos += 20;
        }
    }

    window.display();
}

// Функція для відображення дерева у новому вікні
void displayBTreeUsers(sf::RenderWindow& window, BTree& t, sf::Font& font) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);

    int yPos = 20;

    // Создаем строку для отображения
    std::ostringstream output;
    // Перехватываем вывод из функции traverse и сохраняем в output
    std::streambuf* orig_buf = std::cout.rdbuf();
    std::cout.rdbuf(output.rdbuf());

    // Вызываем traverse для вывода информации
    t.traverse(); // Это выведет информацию в output

    // Восстанавливаем стандартный буфер для вывода в консоль
    std::cout.rdbuf(orig_buf);

    // Отображаем информацию в окне
    std::string result = output.str();
    text.setString(result);
    text.setPosition(10, yPos);
    window.draw(text);

    window.display();
}

// Функція для запуску вікна алгоритму Хаффмана
void runHuffmanWindow(sf::RenderWindow& mainWindow) {
    sf::RenderWindow huffmanWindow(sf::VideoMode(600, 400), L"Huffman Compression");
    sf::Font font;
    if (!font.loadFromFile("Fonts/static/Roboto-Bold.ttf")) {
        cerr << L"Помилка завантаження шрифта!\n";
        return;
    }

    sf::Text inputText(L"Введіть текст:", font, 20);
    inputText.setPosition(20, 20);

    sf::Text outputText(L"", font, 20);
    outputText.setPosition(20, 100);

    sf::RectangleShape button(sf::Vector2f(150, 50));
    button.setPosition(20, 300);
    button.setFillColor(sf::Color::Blue);

    sf::Text buttonText(L"Стиснути", font, 20);
    buttonText.setPosition(50, 310);

    string userInput;
    sf::Text userText(L"", font, 20);
    userText.setPosition(20, 60);

    while (huffmanWindow.isOpen()) {
        sf::Event event;
        while (huffmanWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                huffmanWindow.close();
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128 && event.text.unicode != 13) {
                    userInput += static_cast<char>(event.text.unicode);
                    userText.setString(userInput);
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (button.getGlobalBounds().contains(sf::Mouse::getPosition(huffmanWindow).x, sf::Mouse::getPosition(huffmanWindow).y)) {
                    auto frequencies = HuffmanCompression::calculateFrequencies(userInput);
                    auto root = HuffmanCompression::buildHuffmanTree(frequencies);
                    unordered_map<char, string> codes;
                    HuffmanCompression::generateHuffmanCodes(root, "", codes);
                    auto compressed = HuffmanCompression::compress(userInput, codes);
                    int compressedSize = HuffmanCompression::calculateCompressedSize(compressed);
                    int originalSize = userInput.size() * 8;
                    outputText.setString("Сompression: " + to_string(originalSize - compressedSize) + " bit");
                    HuffmanCompression::freeTree(root);
                }
            }
        }

        huffmanWindow.clear(sf::Color(199, 233, 192));
        huffmanWindow.draw(inputText);
        huffmanWindow.draw(userText);
        huffmanWindow.draw(button);
        huffmanWindow.draw(buttonText);
        huffmanWindow.draw(outputText);
        huffmanWindow.display();
    }
}

int main() {
    setlocale(LC_ALL, "ru");

    // Приклад для сортування 
    std::vector<int> arr = { 52, 45, 38, 29, 14, 98, 101, 33, 9, 6, 8, 5, 34, 4, 3, 2, 1 };

    vector<Project> projects;
    vector<Project> MVP_Projects;

    sf::Color softGreenColor(199, 233, 192); // Фон
    sf::Color oliveGreenColor(154, 205, 50); // Кнопка
    sf::Color darkGreenColor(34, 139, 34);  // Текст

    // Створення вікна
    sf::RenderWindow window(sf::VideoMode(800, 600), L"Программа");

    // Завантажуємо шрифт
    sf::Font font;
    if (!font.loadFromFile("Fonts/static/Roboto-Bold.ttf")) {
        std::cerr << L"Помилка завантаження шрифта!" << std::endl;
        return -1;
    }

    sf::Text titleText;
    createTitleText(titleText, font, L"Пірамидальне сортування", 16, 10, 10);

    // Кнопка сортування 
    sf::RectangleShape sortButton;
    sf::Text sortButtonText;
    createButton(sortButton, sortButtonText, font, L"Натисни мене", { 200, 50 }, { 10, 50 }, oliveGreenColor, 16);

    // Заголовок геш-таблиці 
    sf::Text hashTableTitle;
    createTitleText(hashTableTitle, font, L"Геш-таблиця", 16, 10, 120);

    // Кнопка для відображення геш-таблиці
    sf::RectangleShape hashButton;
    sf::Text hashButtonText;
    createButton(hashButton, hashButtonText, font, L"Показати геш-таблицу", { 200, 50 }, { 10, 160 }, oliveGreenColor, 16);

    // Файл з якого завантажуються користувачі до геш-таблиці
    HashTable hashTable;
    loadFromFile("./Code/Текст.txt", hashTable);

    // Додаемо нових користувачів до геш-таблиці
    hashTable.insert("123456", "Иванов Иван Иванович");
    hashTable.insert("654321", "Петров Петр Петрович");

    bool showHashTableWindow = false;

    // Заголовок для B - дерева
    sf::Text bTreeTitle;
    createTitleText(bTreeTitle, font, L"B-дерево", 16, 600, 10);

    // Кнопка для відображення B-дерева
    sf::RectangleShape bTreeButton;
    sf::Text bTreeButtonText;
    createButton(bTreeButton, bTreeButtonText, font, L"Вивести дерево", { 150, 50 }, { 575, 50 }, oliveGreenColor, 16);

    bool showBTreeWindow = false;

    // Створення В-дерева
    BTree t(3);
    Owner* owners[30] = {
        new Owner("Ivan", "Ivanov", 101, 2),
        new Owner("Pyotr", "Petrov", 102, 1),
        new Owner("Sergey", "Sergeyev", 103, 3),
        new Owner("Maria", "Markova", 104, 2),
        new Owner("Anna", "Antonova", 105, 1),
        new Owner("Dmitry", "Dmitriev", 106, 2),
        new Owner("Elena", "Yelenova", 107, 1),
        new Owner("Alexey", "Alexeev", 108, 3),
        new Owner("Maxim", "Maximov", 109, 2),
        new Owner("Irina", "Irinova", 110, 1),
        new Owner("Olga", "Ol'gova", 111, 1),
        new Owner("Nikita", "Nikitin", 112, 1),
        new Owner("Vladimir", "Vladimirov", 113, 2),
        new Owner("Yulia", "Yuleva", 114, 2),
        new Owner("Tatyana", "Tankova", 115, 3),
        new Owner("Viktor", "Viktorov", 116, 1),
        new Owner("Roman", "Romanov", 117, 1),
        new Owner("Kseniya", "Ksenia", 118, 2),
        new Owner("Grigory", "Grigoryev", 119, 2),
        new Owner("Konstantin", "Konstantinov", 120, 1),
        new Owner("Valeriya", "Valeryeva", 121, 1),
        new Owner("Zinaida", "Zinovyeva", 122, 2),
        new Owner("Arkady", "Arkadyev", 123, 3),
        new Owner("Denis", "Denisov", 124, 1),
        new Owner("Evgeny", "Evgenyev", 125, 2),
        new Owner("Svetlana", "Svetlova", 126, 2),
        new Owner("Pavel", "Pavlov", 127, 1),
        new Owner("Vasilisa", "Vasileva", 128, 1),
        new Owner("Viktoriya", "Kozlova", 142, 1),
        new Owner("Nikolay", "Alexandrov", 143, 2)

    };

    for (int i = 0; i < 30; i++) {
        t.insert(owners[i]);
    }

    // Заголовок до жадібного алгоритму
    sf::Text greedyTitle;
    createTitleText(greedyTitle, font, L"Жадібний алгоритм", 16, 570, 120);

    // Кнопка для відкритя вікно "Жадібний алгоритм"
    sf::RectangleShape greedyButton;
    sf::Text greedyButtonText;
    createButton(greedyButton, greedyButtonText, font, L"Жадібний алгоритм", { 200, 50 }, { 550, 160 }, oliveGreenColor, 16);

    bool showGreedyWindow = false;
    int startDate = 0, endDate = 0, profit = 0;

    // Заголовок для динамічного програмування 
    sf::Text longestSequenceTitle;
    createTitleText(longestSequenceTitle, font, L"Найдовша спадна послідовність", 16, 500, 240);

    // Кнопка для пошуку найдовшої спадної послідовності
    sf::RectangleShape longestSequenceButton;
    sf::Text longestSequenceButtonText;
    createButton(longestSequenceButton, longestSequenceButtonText, font, L"Найдовша спадна послідовність", { 200, 50 }, { 550, 260 }, oliveGreenColor, 16);

    // Заголовок для графа BFS з алгоритмом Дейкстри
    sf::Text dijkstaTitle;
    createTitleText(dijkstaTitle, font, L"Граф Дейкстри", 16, 10, 240);

    // Кнопка для створення графу (BFS)
    sf::RectangleShape dijkstaButton;
    sf::Text dijkstaButtonText;
    createButton(dijkstaButton, dijkstaButtonText, font, L"Граф Дейкстри", { 200, 50 }, { 10, 260 }, oliveGreenColor, 16);

    // Заголовок для графа BFS з алгоритмом Беллмана-Форда
    sf::Text bellmanFordTitle;
    createTitleText(bellmanFordTitle, font, L"Граф Беллмана-Форда", 16, 10, 340);

    // Кнопка для створення графу (BFS з алгоритмом Беллмана-Форда)
    sf::RectangleShape bellmanFordButton;
    sf::Text bellmanFordButtonText;
    createButton(bellmanFordButton, bellmanFordButtonText, font, L"Граф Беллмана-Форда", { 200, 50 }, { 10, 360 }, oliveGreenColor, 16);

    Graph dijkstra(false); // Приклад для ненаправленного графа
    dijkstra.addEdge(0, 1);
    dijkstra.addEdge(0, 2);
    dijkstra.addEdge(1, 2);
    dijkstra.addEdge(1, 3);
    dijkstra.addEdge(1, 3);
    dijkstra.addEdge(2, 3);
    dijkstra.addEdge(2, 4);
    dijkstra.addEdge(4, 1);
    dijkstra.addEdge(4, 5);
    dijkstra.addEdge(4, 6);
    dijkstra.addEdge(6, 7);
    dijkstra.addEdge(6, 8);
    dijkstra.addEdge(6, 9);
    dijkstra.addEdge(8, 9);
    dijkstra.addEdge(9, 5);
    dijkstra.addEdge(8, 10);
    dijkstra.addEdge(10, 3);
    dijkstra.addEdge(18, 19);
    dijkstra.addEdge(19, 20);
    dijkstra.breadthFirstSearch(0);
    
    Graph graphBellmanFord(false);
    graphBellmanFord.addEdge(0, 1);
    graphBellmanFord.addEdge(0, 2);
    graphBellmanFord.addEdge(1, 3);
    graphBellmanFord.addEdge(1, 4);
    graphBellmanFord.addEdge(2, 5);
    graphBellmanFord.addEdge(3, 6);
    graphBellmanFord.addEdge(4, 6);
    graphBellmanFord.addEdge(5, 7);
    graphBellmanFord.addEdge(6, 8);
    graphBellmanFord.addEdge(7, 9);
    graphBellmanFord.addEdge(8, 9);

    // Заголовок для алгоритму Хаффмана
    sf::Text huffmanCompressionTitle;
    createTitleText(huffmanCompressionTitle, font, L"Алгоритм Хаффмана", 16, 500, 340);

    // Кнопка для створення вікна з алгоримтмом Хаффмана
    sf::RectangleShape huffmanCompressionButton;
    sf::Text huffmanCompressionButtonText;
    createButton(huffmanCompressionButton, huffmanCompressionButtonText, font, L"Алгоритм Хаффмана", { 200, 50 }, { 500, 360 }, oliveGreenColor, 16);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (sortButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    heapSort(arr);
                    displaySortedArray(arr);
                }
                if (hashButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    hashTable.display();
                }

                if (hashButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    showHashTableWindow = true;
                }

                if (bTreeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    cout << "Вывод содержимого B-дерева:" << endl;
                    t.traverse(); 
                    showBTreeWindow = true;
                }

                if (greedyButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    showGreedyWindow = true;
                }

                if (longestSequenceButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    openLongestSequenceWindow(font);
                }

                if (dijkstaButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    dijkstra.visualizeDijkstra(0, 7);
                }

                if (bellmanFordButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    graphBellmanFord.visualizeBellmanFord(1, 9);
                }

                if (huffmanCompressionButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    runHuffmanWindow(window);
                }
            }
        }

        window.clear(softGreenColor);
        window.draw(titleText);
        window.draw(sortButton);
        window.draw(sortButtonText);
        window.draw(hashTableTitle);
        window.draw(hashButton);
        window.draw(hashButtonText);
        window.draw(bTreeTitle);
        window.draw(bTreeButton);
        window.draw(bTreeButtonText);
        window.draw(greedyTitle);
        window.draw(greedyButton);
        window.draw(greedyButtonText);
        window.draw(longestSequenceTitle);
        window.draw(longestSequenceButton);
        window.draw(longestSequenceButtonText);
        window.draw(dijkstaTitle);
        window.draw(dijkstaButton);
        window.draw(dijkstaButtonText);
        window.draw(bellmanFordTitle);
        window.draw(bellmanFordButton);
        window.draw(bellmanFordButtonText);
        window.draw(huffmanCompressionTitle);
        window.draw(huffmanCompressionButton);
        window.draw(huffmanCompressionButtonText);
        window.display();

        // Відкриття вікна "Геш-таблиця"
        if (showHashTableWindow) {
            // Створюємо нове вікно для відображення Геш-таблиці 
            sf::RenderWindow hashWindow(sf::VideoMode(600, 600), L"Геш-таблица");

            while (hashWindow.isOpen()) {
                sf::Event hashEvent;
                while (hashWindow.pollEvent(hashEvent)) {
                    if (hashEvent.type == sf::Event::Closed)
                        hashWindow.close();
                }

                hashWindow.clear(sf::Color(250, 220, 100, 0));
                displayHashTable(hashWindow, hashTable, font);
            }

            showHashTableWindow = false;
        }

        // Відкриття вікна "В-дерево
        if (showBTreeWindow) {
            // Створюемо нове вікно для відображення В-дерева
            sf::RenderWindow bTreeWindow(sf::VideoMode(600, 600), L"B-дерево");

            while (bTreeWindow.isOpen()) {
                sf::Event bTreeEvent;
                while (bTreeWindow.pollEvent(bTreeEvent)) {
                    if (bTreeEvent.type == sf::Event::Closed)
                        bTreeWindow.close();
                }

                bTreeWindow.clear(sf::Color(250, 220, 100, 0));
                displayBTreeUsers(bTreeWindow, t, font);
            }

            showBTreeWindow = false;
        }

        // Відкриття вікна "Жадібний алгоритм"
        if (showGreedyWindow) {
            sf::RenderWindow greedyWindow(sf::VideoMode(600, 400), L"Жадібний алгоритм");
            sf::Text inputLabel(L"Введіть проєкт (назва, дата початку, дата кінця, прибуток):", font, 16);
            inputLabel.setPosition(10, 10);

            sf::Text outputLabel(L"", font, 16);
            outputLabel.setPosition(380, 100);
            outputLabel.setFillColor(sf::Color::Cyan);

            sf::RectangleShape addButton(sf::Vector2f(150, 40));
            addButton.setPosition(10, 100);
            addButton.setFillColor(oliveGreenColor);

            sf::Text addButtonText(L"Додати проєкт", font, 16);
            addButtonText.setPosition(15, 105);

            sf::RectangleShape calculateButton(sf::Vector2f(200, 40));
            calculateButton.setPosition(10, 200);
            calculateButton.setFillColor(oliveGreenColor);

            sf::Text calculateButtonText(L"Вирахувати прибуток", font, 16);
            calculateButtonText.setPosition(15, 205);

            std::wstring inputText; // Змінна для зберігання тексту

            while (greedyWindow.isOpen()) {
                sf::Event greedyEvent;
                while (greedyWindow.pollEvent(greedyEvent)) {
                    if (greedyEvent.type == sf::Event::Closed)
                        greedyWindow.close();

                    if (greedyEvent.type == sf::Event::MouseButtonPressed) {
                        if (addButton.getGlobalBounds().contains(greedyEvent.mouseButton.x, greedyEvent.mouseButton.y)) {
                            std::wistringstream ss(inputText);
                            std::wstring projectName;
                            int startDate, endDate, profit;

                            ss >> projectName >> startDate >> endDate >> profit;
                            projects.emplace_back(projectName, profit, startDate, endDate);
                            inputText.clear(); 
                        }

                        if (calculateButton.getGlobalBounds().contains(greedyEvent.mouseButton.x, greedyEvent.mouseButton.y)) {
                            std::vector<Project> selectedProjects = findMaxProfitProjects(projects);
                            int totalProfit = calculateTotalProfit(selectedProjects);

                            std::wostringstream output;
                            output << L"Обранні проєкты: \n";
                            for (const auto& project : selectedProjects) {
                                output << project.NameOfProject << L" (прибуток: " << project.Profit << L")\n";
                            }
                            outputLabel.setString(sf::String(output.str()));
                        }
                    }

                    if (greedyEvent.type == sf::Event::TextEntered) {
                        if (greedyEvent.text.unicode == '\b' && !inputText.empty()) {
                            inputText.pop_back(); // Видалити символ
                        }
                        else if (greedyEvent.text.unicode > 31 && greedyEvent.text.unicode < 128) {
                            inputText += static_cast<wchar_t>(greedyEvent.text.unicode);
                        }
                    }
                }

                greedyWindow.clear(sf::Color(250, 220, 100, 0));
                greedyWindow.draw(inputLabel);
                greedyWindow.draw(addButton);
                greedyWindow.draw(addButtonText);
                greedyWindow.draw(calculateButton);
                greedyWindow.draw(calculateButtonText);

                // Відображення тексту вводу
                sf::Text inputDisplay(sf::String(inputText), font, 16);
                inputDisplay.setPosition(60, 50);
                greedyWindow.draw(inputDisplay);

                // Відображення результату
                greedyWindow.draw(outputLabel);

                greedyWindow.display();
            }

            showGreedyWindow = false;
        }
    }

    return 0;
}  




