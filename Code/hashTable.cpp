#include "hashTable.h"
#include <fstream>

HashEntry::HashEntry() : isEmpty(true) {
    key[0] = '\0';
    ownerInfo[0] = '\0';
}

int HashTable::hash(const char* key) {
    int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum += key[i];
    }
    return sum % TABLE_SIZE;
}

int HashTable::linearProbe(int index) {
    return (index + 1) % TABLE_SIZE;
}

void HashTable::insert(const char* key, const char* ownerInfo) {
    int index = hash(key);
    int originalIndex = index;
    while (!table[index].isEmpty) {
        index = linearProbe(index);
        if (index == originalIndex) {
            std::cout << "Hash table is full!" << std::endl;
            return;
        }
    }
    strcpy_s(table[index].key, key);
    strcpy_s(table[index].ownerInfo, ownerInfo);
    table[index].isEmpty = false;
}

const char* HashTable::search(const char* key) {
    int index = hash(key);
    int originalIndex = index;
    while (!table[index].isEmpty) {
        if (strcmp(table[index].key, key) == 0) {
            return table[index].ownerInfo;
        }
        index = linearProbe(index);
        if (index == originalIndex) {
            break;
        }
    }
    return nullptr;
}

void HashTable::display() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (!table[i].isEmpty) {
            std::cout << "Індекс: " << i << " VIN: " << table[i].key << " Інформація про власника: " << table[i].ownerInfo << std::endl;
        }
    }
}

void HashTable::loadFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error opening file!" << std::endl;
        return;
    }

    char vin[20];
    char ownerSurname[50];
    char ownerName[50];
    char ownerPatronymic[50];
    char ownerInfo[150];

    while (file >> vin >> ownerSurname >> ownerName >> ownerPatronymic) {
        strcpy_s(ownerInfo, ownerSurname);
        strcat_s(ownerInfo, " ");
        strcat_s(ownerInfo, ownerName);
        strcat_s(ownerInfo, " ");
        strcat_s(ownerInfo, ownerPatronymic);
        insert(vin, ownerInfo);
    }

    file.close();
}

