#pragma once
#ifndef GREEDY
#define GREEDY

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

////////////////////////////////////////
// Файл для роботи з жадібним алгоритмом
////////////////////////////////////////

#define MAX_LOADSTRING 100

// Структура для проектів
struct Project {
    std::wstring NameOfProject; 
    int Profit;                 
    int DateOfBegining;         
    int DateOfEnd;              

    Project(const std::wstring& name, int profit, int start, int end)
        : NameOfProject(name), Profit(profit), DateOfBegining(start), DateOfEnd(end) {}
};

bool compareProjects(const Project& a, const Project& b);

// Функция для знаходження проєктів з максимальним прибутком
vector<Project> findMaxProfitProjects(const vector<Project>& projects);

// Функция для підрахування спільного прибутку
int calculateTotalProfit(const vector<Project>& projects);

#endif // !GREEDY
