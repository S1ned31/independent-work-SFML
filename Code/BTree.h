#ifndef BTREE_H
#define BTREE_H

#include <iostream>
#include <string>
using namespace std;

struct Owner {
    string first_name;
    string last_name;
    int car_number;
    int cars_count;

    Owner(string fn, string ln, int cn, int count)
        : first_name(fn), last_name(ln), car_number(cn), cars_count(count) {}
};

class TreeNode {
    Owner** keys;   // Массив ключей
    int t;          // Минимальная степень
    TreeNode** C;   // Массив дочерних указателей
    int n;          // Текущее количество ключей
    bool leaf;      // Истина, если листовой узел

public:
    TreeNode(int temp, bool bool_leaf);
    void insertNonFull(Owner* owner);
    void splitChild(int i, TreeNode* y);
    void traverse();
    TreeNode* search(int k);

    friend class BTree;
};

class BTree {
    TreeNode* root; // Указатель на корневой узел
    int t;          // Минимальная степень

public:
    BTree(int temp);
    void traverse();
    TreeNode* search(int k);
    void insert(Owner* owner);
};

#endif
