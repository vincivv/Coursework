/**
* @author - Dias Almat
* @file hash_table.cpp -  This file contains the implementation of a hash table.
* 3/16/2025 - Dias Almat created file and added comments.
* 3/16/2025 - modified by Dias Almat; added comments
*/

#include "hash_table.h"
#include <iostream>

//constructor
HashTable::HashTable(int size) {
    numberOfBuckets = size;
    table = new HashNode*[size]();  // Allocate array
    numberOfItems = 0;
}


/*getSize*/
int HashTable::getSize() {
    return numberOfItems;
}

/*calculateHashCode*/
int HashTable::calculateHashCode(int currentKey) {
    return currentKey%numberOfBuckets;
}

/*isEmpty*/
bool HashTable::isEmpty() {
    return (numberOfItems == 0);
}

/*getNumberOfItems*/
int HashTable::getNumberOfItems(){
    return numberOfItems;
}

/*add*/
bool HashTable::add(int curKey, HashNode* myNode) {
    int hashCode = calculateHashCode(curKey);
    if (table[hashCode] == nullptr) {
        table[hashCode] = myNode;
        numberOfItems++;
        return true;
    }
    else {
        HashNode* temp = table[hashCode];
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = myNode;
        myNode->prev = temp;
        numberOfItems++;
        return true;
    }
    return false;
}

/*remove*/
bool HashTable::remove(int curKey){
    int hashCode = calculateHashCode(curKey);
    if (table[hashCode] == nullptr) {
        return false;
    }
    else {
        HashNode* temp = table[hashCode];
        while (temp->key != curKey && temp->next != nullptr) {
            temp = temp->next;
        }
        if (temp->key == curKey) {
            if (temp->prev == nullptr && temp->next == nullptr) {
                table[hashCode] = nullptr;
                delete temp;
                numberOfItems--;
                return true;
            }
            else if (temp->prev == nullptr) {
                table[hashCode] = temp->next;
                temp->next->prev = nullptr;
                delete temp;
                numberOfItems--;
                return true;
            }
            else if (temp->next == nullptr) {
                temp->prev->next = nullptr;
                delete temp;
                numberOfItems--;
                return true;
            }
            else {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                delete temp;
                numberOfItems--;
                return true;
            }
        }
        else {
            return false;
        }
    }
}

/*clear*/
void HashTable::clear() {
    for (int i = 0; i < numberOfBuckets; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* next = current->next;
            delete current;
            current = next;
        }
        table[i] = nullptr;
    }
    numberOfItems = 0;
}

/*getItem*/
HashNode* HashTable::getItem(int curKey) {
    int hashCode = calculateHashCode(curKey);
    if (table[hashCode] == nullptr) {
        return nullptr;
    }
    else {
        HashNode* temp = table[hashCode];
        while (temp->key != curKey && temp->next != nullptr) {
            temp = temp->next;
        }
        if (temp->key == curKey) {
            return temp;
        }
        else {
            return nullptr;
        }
    }
}

/*contatins*/
bool HashTable::contains(int curKey) {
    int hashCode = calculateHashCode(curKey);
    if (table[hashCode] == nullptr) {
        return false;
    }
    else {
        HashNode* temp = table[hashCode];
        while (temp->key != curKey && temp->next != nullptr) {
            temp = temp->next;
        }
        if (temp->key == curKey) {
            return true;
        }
        else {
            return false;
        }
    }
}

/*printtable*/
void HashTable::printTable() {
    std::cout << "\nTable contents (" << numberOfItems << " entries):" << std::endl;

    bool isFirstEmpty = true; 
    bool inEmptySequence = false; 

    for (int i = 0; i < numberOfBuckets; i++) {
        if (table[i] == nullptr) { 
            if (isFirstEmpty) {
                std::cout << "\nEmpty: ";
                isFirstEmpty = false;
            }
            std::cout << i;
            if (i < numberOfBuckets - 1 && table[i + 1] == nullptr) {
                std::cout << ", ";
            } else {
                std::cout << "\n";
                isFirstEmpty = true;
            }
        } else {
            if (!isFirstEmpty) {
                std::cout << "\n"; 
                isFirstEmpty = true;
            }
            std::cout << "\nIndex: " << i << ": ";

            HashNode* temp = table[i];
            while (temp != nullptr) {
                std::cout << temp->key;
                if (temp->next != nullptr) {
                    std::cout << " -> ";
                }
                temp = temp->next;
            }
            std::cout << "\n";
        }
    }
    std::cout << "\nEnd of table\n\n";
}

/*getTable*/
HashNode** HashTable::getTable() {
    return table;
}