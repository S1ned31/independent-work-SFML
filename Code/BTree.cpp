#include "BTree.h"

// Конструктор узла
TreeNode::TreeNode(int t1, bool leaf1) {
    t = t1;
    leaf = leaf1;
    keys = new Owner * [2 * t - 1];
    C = new TreeNode * [2 * t];
    n = 0;
}

// Проход по дереву
void TreeNode::traverse() {
    int i;
    for (i = 0; i < n; i++) {
        if (!leaf) C[i]->traverse();
        cout << "Car number: " << keys[i]->car_number
            << ", Owner: " << keys[i]->first_name << " " << keys[i]->last_name
            << ", Cars: " << keys[i]->cars_count << endl;
    }
    if (!leaf) C[i]->traverse();
}

// Поиск узла
TreeNode* TreeNode::search(int k) {
    int i = 0;
    while (i < n && k > keys[i]->car_number) i++;

    if (keys[i]->car_number == k) return this;
    if (leaf) return nullptr;

    return C[i]->search(k);
}

// Конструктор BTree
BTree::BTree(int temp) {
    root = nullptr;
    t = temp;
}

// Обход дерева
void BTree::traverse() {
    if (root != nullptr) root->traverse();
}

// Поиск в дереве
TreeNode* BTree::search(int k) {
    return (root == nullptr) ? nullptr : root->search(k);
}

// Вставка в BTree
void BTree::insert(Owner* owner) {
    if (root == nullptr) {
        root = new TreeNode(t, true);
        root->keys[0] = owner;
        root->n = 1;
    }
    else {
        if (root->n == 2 * t - 1) {
            TreeNode* s = new TreeNode(t, false);
            s->C[0] = root;
            s->splitChild(0, root);
            int i = 0;
            if (s->keys[0]->car_number < owner->car_number) i++;
            s->C[i]->insertNonFull(owner);
            root = s;
        }
        else {
            root->insertNonFull(owner);
        }
    }
}

// Вставка в неполный узел
void TreeNode::insertNonFull(Owner* owner) {
    int i = n - 1;
    if (leaf) {
        while (i >= 0 && keys[i]->car_number > owner->car_number) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = owner;
        n++;
    }
    else {
        while (i >= 0 && keys[i]->car_number > owner->car_number) i--;
        if (C[i + 1]->n == 2 * t - 1) {
            splitChild(i + 1, C[i + 1]);
            if (keys[i + 1]->car_number < owner->car_number) i++;
        }
        C[i + 1]->insertNonFull(owner);
    }
}

// Разбиение узла
void TreeNode::splitChild(int i, TreeNode* y) {
    TreeNode* z = new TreeNode(y->t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++) z->keys[j] = y->keys[j + t];
    if (!y->leaf) {
        for (int j = 0; j < t; j++) z->C[j] = y->C[j + t];
    }

    y->n = t - 1;
    for (int j = n; j >= i + 1; j--) C[j + 1] = C[j];
    C[i + 1] = z;

    for (int j = n - 1; j >= i; j--) keys[j + 1] = keys[j];
    keys[i] = y->keys[t - 1];

    n++;
}
