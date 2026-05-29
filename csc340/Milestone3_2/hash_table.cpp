/**
* @author - Ian Kligman
* @file hash_table.cpp -  This file contains the methods for hash table.
* 
* 
*/

#include "hash_table.h"
#include "hash_node.h"
#include <iostream>
#include <string>
#include <fstream>



extern std::ofstream _outFile;
std::ofstream& getOutFile();

//constructor
HashTable::HashTable(int size) {
    numberOfBuckets = size;
    numberOfItems = 0;
    table = new HashNode*[numberOfBuckets];

    for (int i = 0; i < numberOfBuckets; i++) {
        table[i] = nullptr;
    }
}
//destructor
HashTable::~HashTable() {
    clear();
    delete[] table;
}

HashNode** HashTable::getTable() {
    return table;
}

int HashTable::getSize() {
    return numberOfBuckets;
}

// Use modulo hashing
int HashTable::calculateHashCode(int currentKey) {
    return currentKey % numberOfBuckets;
}

// Checks if the hash table is empty
bool HashTable::isEmpty() {
    return numberOfItems == 0;
}

// Returns the number of items in the hash table
int HashTable::getNumberOfItems() {
    return numberOfItems;
}

// Adds a new node to the hash table
bool HashTable::add(int curKey, HashNode* myNode) {
    int index = calculateHashCode(curKey);
    HashNode* current = table[index];

    if (current == nullptr) {
        // No collision, directly add the node
        table[index] = myNode;
    } else {
        // Handle collision by chaining
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = myNode;
    }

    numberOfItems++;
    return true;
}

// Removes a node with the specified key
bool HashTable::remove(int curKey) {
    int index = calculateHashCode(curKey);
    HashNode* current = table[index];
    HashNode* prev = nullptr;

    while (current != nullptr) {
        if (current->key == curKey) {
            if (prev == nullptr) {
                // Removing the first node in the bucket
                table[index] = current->next;
            } else {
                // Removing a node in the middle or end
                prev->next = current->next;
            }
            numberOfItems--;
            return true;
        }

        prev = current;
        current = current->next;
    }

    return false; // Key not found
}

// Clears the hash table by deleting all nodes, iterating through all.
void HashTable::clear() {
    for (int i = 0; i < numberOfBuckets; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
        table[i] = nullptr;
    }
    numberOfItems = 0;
}

// Retrieves the node with the specified key
HashNode* HashTable::getItem(int curKey) {
    int index = calculateHashCode(curKey);
    HashNode* current = table[index];

    while (current != nullptr) {
        if (current->key == curKey) {
            return current;
        }
        current = current->next;
    }
    return nullptr; // Default: Key not found
}

// Checks if a node with the given key exists in the hash table
bool HashTable::contains(int curKey) {
    int index = calculateHashCode(curKey);
    HashNode* current = table[index];

    while (current != nullptr) {
        if (current->key == curKey) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Prints the contents of the hash table
void HashTable::printTable() {
    std::ofstream& outFile = getOutFile();

    for (int i = 0; i < numberOfBuckets; i++) {
        std::cout << "Bucket " << i << ": ";
        outFile << "Bucket " << i << ": ";

        HashNode* temp = table[i];
        if (temp == nullptr) {
            std::cout << "Empty" << std::endl;
            outFile << "Empty" << std::endl;
        } else {
            while (temp != nullptr) {
                std::cout << "FIFO info from cacheManager:  Node key: " << temp->key << std::endl;
                outFile << "FIFO info from cacheManager:  Node key: " << temp->key << std::endl;
                temp = temp->next;
            }
        }
    }
    std::cout << "End of table" << std::endl;
    outFile << "End of table" << std::endl;
}
