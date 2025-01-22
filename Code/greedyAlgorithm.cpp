#include "greedyAlgorithm.h"

using namespace std;

bool compareProjects(const Project& a, const Project& b) {
    return a.DateOfEnd < b.DateOfEnd;
}

// Функция для нахождения проектов с максимальной прибылью
std::vector<Project> findMaxProfitProjects(const std::vector<Project>& projects) {
    // Создаём копию, чтобы отсортировать проекты
    std::vector<Project> sortedProjects = projects;
    sort(sortedProjects.begin(), sortedProjects.end(), compareProjects);

    std::vector<Project> bestProjects;
    int bestProfit = 0;

    for (int i = 0; i < sortedProjects.size(); i++) {
        int currentProfit = 0;
        int currentDate = 0;
        std::vector<Project> currentProjects;

        for (int j = i; j < sortedProjects.size(); j++) {
            if (sortedProjects[j].DateOfBegining >= currentDate) {
                currentProfit += sortedProjects[j].Profit;
                currentDate = sortedProjects[j].DateOfEnd;
                currentProjects.push_back(sortedProjects[j]);
            }
        }

        if (currentProfit > bestProfit) {
            bestProfit = currentProfit;
            bestProjects = currentProjects;
        }
    }

    return bestProjects;
}

// Функция для подсчёта общей прибыли
int calculateTotalProfit(const std::vector<Project>& projects) {
    int totalProfit = 0;
    for (const auto& project : projects) {
        totalProfit += project.Profit;
    }
    return totalProfit;
}

