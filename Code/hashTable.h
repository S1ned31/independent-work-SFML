#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <fstream>
#include <cstring>

/////////////////////////////////
// Файл для роботи з геш-таблицею
/////////////////////////////////

const int TABLE_SIZE = 100;

struct HashEntry {
    char key[20];
    char ownerInfo[150];
    bool isEmpty;
    HashEntry();
};

class HashTable {
private:
    HashEntry table[TABLE_SIZE];

    int hash(const char* key);
    int linearProbe(int index);

public:
    void insert(const char* key, const char* ownerInfo);
    const char* search(const char* key);
    void display();
    void loadFromFile(const char* filename);
    HashEntry* getTable() {
        return table;
    }
};

#endif // HASHTABLE_H
