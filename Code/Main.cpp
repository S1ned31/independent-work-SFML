#include <SFML/Graphics.hpp>
#include "design.h"
#include "Sort.h"  
#include "hashTable.h"
#include "BTree.h"
#include "greedyAlgorithm.h"
#include "longestSequence.h"
#include "graphBFS.h"
//#include "graphVisualization.h"
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

// Функция для отображения хеш-таблицы в новом окне
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


int main() {
    setlocale(LC_ALL, "ru");

    // Example array for sorting
    std::vector<int> arr = { 52, 45, 38, 29, 14, 98, 101, 33, 9, 6, 8, 5, 34, 4, 3, 2, 1 };

    vector<Project> projects;
    vector<Project> MVP_Projects;

    sf::Color softGreenColor(199, 233, 192); // Фон
    sf::Color oliveGreenColor(154, 205, 50); // Кнопка
    sf::Color darkGreenColor(34, 139, 34);  // Текст

    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), L"Программа");

    sf::Font font;
    if (!font.loadFromFile("Fonts/static/Roboto-Bold.ttf")) {
        std::cerr << L"Ошибка загрузки шрифта" << std::endl;
        return -1;
    }



    sf::Text titleText;
    createTitleText(titleText, font, L"НЕпирамидальная сортировка", 16, 10, 10);

    // Кнопка сортировки
    sf::RectangleShape sortButton;
    sf::Text sortButtonText;
    createButton(sortButton, sortButtonText, font, L"Нажми меня", { 200, 50 }, { 10, 50 }, oliveGreenColor, 16);



    // Hash table title
    sf::Text hashTableTitle;
    createTitleText(hashTableTitle, font, L"Хеш-таблица", 16, 10, 120);

    // Кнопка для отображения хеш-таблицы
    sf::RectangleShape hashButton;
    sf::Text hashButtonText;
    createButton(hashButton, hashButtonText, font, L"Показать хеш-таблицу", { 200, 50 }, { 10, 160 }, oliveGreenColor, 16);


    // Заголовок для B - дерева
    sf::Text bTreeTitle;
    createTitleText(bTreeTitle, font, L"B-дерево", 16, 600, 10);

    // Кнопка для отображения B-дерева
    sf::RectangleShape bTreeButton;
    sf::Text bTreeButtonText;
    createButton(bTreeButton, bTreeButtonText, font, L"Вывести дерево", { 150, 50 }, { 575, 50 }, oliveGreenColor, 16);


    // Create hash table instance
    HashTable hashTable;
    loadFromFile("./Code/Текст.txt", hashTable);


    // Insert some data into the hash table
    hashTable.insert("123456", "Иванов Иван Иванович");
    hashTable.insert("654321", "Петров Петр Петрович");

    bool showHashTableWindow = false;
    bool showBTreeWindow = false;

    // Создание B-дерева и добавление данных
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

    sf::Text greedyTitle;
    createTitleText(greedyTitle, font, L"Жадібний алгоритм", 16, 570, 120);

    // Кнопка для открытия окна "Жадный алгоритм"
    sf::RectangleShape greedyButton;
    sf::Text greedyButtonText;
    createButton(greedyButton, greedyButtonText, font, L"Жадный алгоритм", { 200, 50 }, { 550, 160 }, oliveGreenColor, 16);

    bool showGreedyWindow = false;
    int startDate = 0, endDate = 0, profit = 0;

    sf::Text longestSequenceTitle;
    createTitleText(longestSequenceTitle, font, L"Найдовша спадна послідовність", 16, 500, 240);

    // Кнопка для пошуку найдовшої спадної послідовності
    sf::RectangleShape longestSequenceButton;
    sf::Text longestSequenceButtonText;
    createButton(longestSequenceButton, longestSequenceButtonText, font, L"Найдовша спадна послідовність", { 200, 50 }, { 550, 260 }, oliveGreenColor, 16);

    // Заголовок для графа BFS
    sf::Text graphTitle;
    createTitleText(graphTitle, font, L"Граф BFS", 16, 10, 320);

    // Кнопка для створення графу (BFS)
    sf::RectangleShape graphButton;
    sf::Text graphButtonText;
    createButton(graphButton, graphButtonText, font, L"Граф BFS", { 200, 50 }, { 10, 320 }, oliveGreenColor, 16);

    Graph g(false); // Пример для ненаправленного графа
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(4, 1);
    g.addEdge(4, 5);
    g.addEdge(4, 6);
    g.addEdge(6, 7);
    g.addEdge(6, 8);
    g.addEdge(6, 9);
    g.addEdge(8, 9);
    g.addEdge(9, 5);
    g.addEdge(8, 10);
    g.addEdge(10, 3);
    g.addEdge(18, 19);
    g.addEdge(19, 20);
    g.breadthFirstSearch(0);


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
                    t.traverse();  // Вывод содержимого дерева
                    showBTreeWindow = true;
                }

                if (greedyButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    showGreedyWindow = true;
                }

                if (longestSequenceButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    openLongestSequenceWindow(font);
                }

                if (graphButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    g.visualizeGraph();
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
        window.draw(graphButton);
        window.draw(graphButtonText);
        window.display();


        if (showHashTableWindow) {
            // Создаем новое окно для отображения хеш-таблицы
            sf::RenderWindow hashWindow(sf::VideoMode(600, 600), L"Хеш-таблица");

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

        if (showBTreeWindow) {
            // Создаем новое окно для отображения B-дерева
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

        // Открытие окна "Жадного алгоритма"
        if (showGreedyWindow) {
            sf::RenderWindow greedyWindow(sf::VideoMode(600, 400), L"Жадный алгоритм");
            sf::Text inputLabel(L"Введите проект (название, дата начала, дата конца, прибыль):", font, 16);
            inputLabel.setPosition(10, 10);

            sf::Text outputLabel(L"", font, 16);
            outputLabel.setPosition(380, 100);
            outputLabel.setFillColor(sf::Color::Cyan);

            sf::RectangleShape addButton(sf::Vector2f(150, 40));
            addButton.setPosition(10, 100);
            addButton.setFillColor(oliveGreenColor);

            sf::Text addButtonText(L"Добавить проект", font, 16);
            addButtonText.setPosition(15, 105);

            sf::RectangleShape calculateButton(sf::Vector2f(200, 40));
            calculateButton.setPosition(10, 200);
            calculateButton.setFillColor(oliveGreenColor);

            sf::Text calculateButtonText(L"Рассчитать прибыль", font, 16);
            calculateButtonText.setPosition(15, 205);

            std::wstring inputText; // Переменная для хранения текста

            while (greedyWindow.isOpen()) {
                sf::Event greedyEvent;
                while (greedyWindow.pollEvent(greedyEvent)) {
                    if (greedyEvent.type == sf::Event::Closed)
                        greedyWindow.close();

                    if (greedyEvent.type == sf::Event::MouseButtonPressed) {
                        if (addButton.getGlobalBounds().contains(greedyEvent.mouseButton.x, greedyEvent.mouseButton.y)) {
                            // Логика добавления проекта
                            std::wistringstream ss(inputText);
                            std::wstring projectName;
                            int startDate, endDate, profit;

                            ss >> projectName >> startDate >> endDate >> profit;
                            projects.emplace_back(projectName, profit, startDate, endDate);
                            inputText.clear(); // Очистка ввода
                        }

                        if (calculateButton.getGlobalBounds().contains(greedyEvent.mouseButton.x, greedyEvent.mouseButton.y)) {
                            // Логика вычисления максимальной прибыли
                            std::vector<Project> selectedProjects = findMaxProfitProjects(projects);
                            int totalProfit = calculateTotalProfit(selectedProjects);

                            std::wostringstream output;
                            output << L"Выбранные проекты: \n";
                            for (const auto& project : selectedProjects) {
                                output << project.NameOfProject << L" (прибыль: " << project.Profit << L")\n";
                            }
                            outputLabel.setString(sf::String(output.str()));
                        }
                    }

                    if (greedyEvent.type == sf::Event::TextEntered) {
                        if (greedyEvent.text.unicode == '\b' && !inputText.empty()) {
                            inputText.pop_back(); // Удалить символ
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

                // Отображение текста ввода
                sf::Text inputDisplay(sf::String(inputText), font, 16);
                inputDisplay.setPosition(60, 50);
                greedyWindow.draw(inputDisplay);

                // Отображаем результат
                greedyWindow.draw(outputLabel);

                greedyWindow.display();
            }

            showGreedyWindow = false;
        }
    }

    return 0;
}  




