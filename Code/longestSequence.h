#pragma once
#ifndef LONGESTSEQ
#define LONGESTSEQ

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <algorithm>

//////////////////////////////////////////////
// Файл для роботи з динамічним програмуванням
//////////////////////////////////////////////

void openLongestSequenceWindow(sf::Font& font);

std::vector<int> findLongestDecreasingSubsequence(const std::vector<int>& sequence);

#endif // !LONGESTSEQ
