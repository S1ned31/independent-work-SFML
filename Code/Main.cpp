#include <SFML/Graphics.hpp>
#include "Sort.h"  
#include "hashTable.h"
#include "BTree.h"
#include <iostream>
#include <fstream>
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

// ������� ��� ����������� ���-������� � ����� ����
void displayHashTable(sf::RenderWindow& window, HashTable& hashTable, sf::Font& font) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);

    int yPos = 20;
    // �������� ������� ������
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

int main() {
    setlocale(LC_ALL, "ru");

    // Example array for sorting
    std::vector<int> arr = { 52, 45, 38, 29, 14, 98, 101, 33, 9, 6, 8, 5, 34, 4, 3, 2, 1 };

    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), L"���������");

    sf::Font font;
    if (!font.loadFromFile("Fonts/static/Roboto-Bold.ttf")) {
        std::cerr << L"������ �������� ������" << std::endl;
        return -1;
    }

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString(L"������������� ����������");
    titleText.setCharacterSize(16);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(10, 10);

    // Button for sorting
    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(L"����� ����");
    buttonText.setCharacterSize(16);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setStyle(sf::Text::Bold);
    sf::RectangleShape button(sf::Vector2f(200, 50));
    button.setPosition(10, 50);
    button.setFillColor(sf::Color::Blue);
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    sf::FloatRect textBounds = buttonText.getGlobalBounds();
    buttonText.setPosition(button.getPosition().x + (buttonBounds.width - textBounds.width) / 2,
        button.getPosition().y + (buttonBounds.height - textBounds.height) / 2 - 5);

    // Hash table title
    sf::Text hashTableTitle;
    hashTableTitle.setFont(font);
    hashTableTitle.setString(L"���-�������");
    hashTableTitle.setCharacterSize(16);
    hashTableTitle.setFillColor(sf::Color::White);
    hashTableTitle.setStyle(sf::Text::Bold);
    hashTableTitle.setPosition(10, 120);

    // Button for hash table display
    sf::Text hashButtonText;
    hashButtonText.setFont(font);
    hashButtonText.setString(L"�������� ���-�������");
    hashButtonText.setCharacterSize(16);
    hashButtonText.setFillColor(sf::Color::White);
    hashButtonText.setStyle(sf::Text::Bold);
    sf::RectangleShape hashButton(sf::Vector2f(200, 50));
    hashButton.setPosition(10, 160);
    hashButton.setFillColor(sf::Color::Green);
    sf::FloatRect hashButtonBounds = hashButton.getGlobalBounds();
    sf::FloatRect hashTextBounds = hashButtonText.getGlobalBounds();
    hashButtonText.setPosition(hashButton.getPosition().x + (hashButtonBounds.width - hashTextBounds.width) / 2,
        hashButton.getPosition().y + (hashButtonBounds.height - hashTextBounds.height) / 2 - 5);;

    // ��������� ��� B - ������
    sf::Text bTreeTitle;
    bTreeTitle.setFont(font);
    bTreeTitle.setString(L"B-������");
    bTreeTitle.setCharacterSize(16);
    bTreeTitle.setFillColor(sf::Color::White);
    bTreeTitle.setStyle(sf::Text::Bold);
    bTreeTitle.setPosition(600, 10);

    // ������ ��� ������ B-������
    sf::Text bTreeButtonText;
    bTreeButtonText.setFont(font);
    bTreeButtonText.setString(L"������� ������");
    bTreeButtonText.setCharacterSize(16);
    bTreeButtonText.setFillColor(sf::Color::White);
    bTreeButtonText.setStyle(sf::Text::Bold);
    sf::RectangleShape bTreeButton(sf::Vector2f(150, 50));
    bTreeButton.setPosition(600, 50);
    bTreeButton.setFillColor(sf::Color::Blue);
    sf::FloatRect bTreeButtonBounds = bTreeButton.getGlobalBounds();
    sf::FloatRect bTreeTextBounds = bTreeButtonText.getGlobalBounds();
    bTreeButtonText.setPosition(bTreeButton.getPosition().x + (bTreeButtonBounds.width - bTreeTextBounds.width) / 2,
        bTreeButton.getPosition().y + (bTreeButtonBounds.height - bTreeTextBounds.height) / 2 - 5);



    // Create hash table instance
    HashTable hashTable;
    loadFromFile("./Code/�����.txt", hashTable);


    // Insert some data into the hash table
    hashTable.insert("123456", "������ ���� ��������");
    hashTable.insert("654321", "������ ���� ��������");

    bool showHashTableWindow = false;

    // �������� B-������ � ���������� ������
    BTree t(3);
    Owner* owners[30] = {
        new Owner("����", "������", 101, 2),
        new Owner("����", "������", 102, 1),
        new Owner("������", "�������", 103, 3),
        new Owner("�����", "�������", 104, 2),
        new Owner("����", "��������", 105, 1),
        new Owner("�������", "��������", 106, 2),
        new Owner("�����", "�������", 107, 1),
        new Owner("�������", "��������", 108, 3),
        new Owner("������", "��������", 109, 2),
        new Owner("�����", "�������", 110, 1),
        new Owner("�����", "�������", 111, 1),
        new Owner("������", "�������", 112, 1),
        new Owner("��������", "����������", 113, 2),
        new Owner("����", "������", 114, 2),
        new Owner("�������", "�������", 115, 3),
        new Owner("������", "��������", 116, 1),
        new Owner("�����", "�������", 117, 1),
        new Owner("������", "������", 118, 2),
        new Owner("��������", "���������", 119, 2),
        new Owner("����������", "������������", 120, 1),
        new Owner("�������", "���������", 121, 1),
        new Owner("�������", "���������", 122, 2),
        new Owner("�������", "��������", 123, 3),
        new Owner("�����", "�������", 124, 1),
        new Owner("�������", "��������", 125, 2),
        new Owner("��������", "��������", 126, 2),
        new Owner("�����", "������", 127, 1),
        new Owner("��������", "���������", 128, 1),
        new Owner("��������", "�������", 142, 1),
        new Owner("�������", "�����������", 143, 2)
    };

    for (int i = 0; i < 30; i++) {
        t.insert(owners[i]);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
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
                    cout << "����� ����������� B-������:" << endl;
                    t.traverse();  // ����� ����������� ������
                }
            }
        }

        window.clear();
        window.draw(titleText);
        window.draw(button);
        window.draw(buttonText);
        window.draw(hashTableTitle);
        window.draw(hashButton);
        window.draw(hashButtonText);
        window.draw(bTreeTitle);
        window.draw(bTreeButton);
        window.draw(bTreeButtonText);
        window.display();
    

        if (showHashTableWindow) {
            // ������� ����� ���� ��� ����������� ���-�������
            sf::RenderWindow hashWindow(sf::VideoMode(600, 600), L"���-�������");

            while (hashWindow.isOpen()) {
                sf::Event hashEvent;
                while (hashWindow.pollEvent(hashEvent)) {
                    if (hashEvent.type == sf::Event::Closed)
                        hashWindow.close();
                }

                hashWindow.clear();
                displayHashTable(hashWindow, hashTable, font);
            }

            showHashTableWindow = false;
        }
    
}
    return 0;
}
