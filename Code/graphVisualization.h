#pragma once
#ifndef GRAPH_VS
#define GRAPH_VS

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>

struct Edge {
    int from;
    int to;
};

void displayGraphWindow(sf::Font& font);

#endif // !GRAPH_VS
