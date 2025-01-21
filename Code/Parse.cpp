#include "Parse.h"
#include <sstream>

// ������� ��� �������� ����� �� �����
std::vector<int> parseInput(const std::string& input) {
    std::vector<int> result;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        result.push_back(std::stoi(item));
    }
    return result;
}