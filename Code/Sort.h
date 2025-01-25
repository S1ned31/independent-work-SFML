#pragma once

#ifndef SORT_H
#define SORT_H

#include <vector>

//////////////////////////////////////////////
// Файл для роботи з пірамідальним сортуванням
//////////////////////////////////////////////

void heapify(std::vector<int>& arr, int n, int i);
void heapSort(std::vector<int>& arr);
void displaySortedArray(const std::vector<int>& arr);

#endif // SORT_H
