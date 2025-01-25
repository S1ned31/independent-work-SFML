#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <string>
using namespace std;

////////////////////////////////////////
// Файл, який реалізує роботу з деревом
////////////////////////////////////////

struct Owner {
    string first_name;
    string last_name;
    int car_number;
    int cars_count;

    Owner(string fn, string ln, int cn, int count)
        : first_name(fn), last_name(ln), car_number(cn), cars_count(count) {}
};

class TreeNode {
    Owner** keys;   // Массив ключів
    int t;          // Минимальная ступінь
    TreeNode** C;   // Массив дочірніх показчиків
    int n;          // Поточна кількість ключів
    bool leaf;      // Правда, якщо листовий вузол

public:
    TreeNode(int temp, bool bool_leaf);
    void insertNonFull(Owner* owner);
    void splitChild(int i, TreeNode* y);
    void traverse();
    TreeNode* search(int k);

    friend class BTree;
};

class BTree {
    TreeNode* root; // Показчик на корневий вузол
    int t;          // Мінімальна  ступінь

public:
    BTree(int temp);
    void traverse();
    TreeNode* search(int k);
    void insert(Owner* owner);
};

#endif
