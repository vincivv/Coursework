/**
* @author - Dias Almat
* @author - Ian Kligman
* @file cache_manager.cpp -  This is a cache manager files that combines doubly linked list and hash table.
* 3/16/2025 - Dias Almat created file and added comments.
* 3/16/2025 - modified by Dias Almat; added comments
* 3/19/2025 - modified by I. Kligman; added add and remove methods + comments
*/


#include "cache_manager.h"
#include <iostream>

/*getTable*/
HashTable* CacheManager::getTable(){
    return hashTable;
}

/*getList*/
DoublyLinkedList* CacheManager::getList(){
    return doublyLinkedList;
}

/*getSize*/
int CacheManager::getSize() {
    return doublyLinkedList->getSize();
}

/*isEmpty*/
bool CacheManager::isEmpty() {
    return (doublyLinkedList->isEmpty());
}

/*add*/
bool CacheManager::add(int curKey, DllNode* myNode) {
    // Check if the cache is full
    if (getSize() >= maxCacheSize) {
        if (doublyLinkedList->head != nullptr) {
            remove(doublyLinkedList->head->key);
        } else {
            std::cerr << "Error: Attempted to remove from an empty list." << std::endl;
            return false;
        }
    }

    // Add the new node to the hash table
    HashNode* hashNode = new HashNode(curKey, myNode);
    if (!hashTable->add(curKey, hashNode)) {
        delete hashNode;
        return false;
    }

    // Add the new node to the end of the DoublyLinkedList
    doublyLinkedList->insertAtTail(curKey, myNode);

    return true;
}

/*remove*/
bool CacheManager::remove(int curKey) {
    // First, confirm that the key exists in the hash table.
    HashNode* hashNode = hashTable->getItem(curKey);
    if (hashNode == nullptr) {
    // If getItem failed, return false indicating remove failure.
        return false; 
    }

    // Remove the node from the hash table, return false if remove fails.
    if (!hashTable->remove(curKey)) {
        return false;
    }

    // Since it exists in hash table, get the DllNode and remove it from the DLL.
    DllNode* dllNode = hashNode->getFifoNode();
    if (dllNode == nullptr) {
        std::cerr << "Error: DllNode is null for key " << curKey << std::endl;
        delete hashNode;
        return false;
    }

    // Remove the node from the DoublyLinkedList.
    doublyLinkedList->remove(dllNode->key);

    // Free the dynamic memory and return true.
    delete hashNode;

    return true;
}

/*clear*/
void CacheManager::clear(){
    hashTable->clear();
    doublyLinkedList->clear();
}

/*getItem*/
DllNode* CacheManager::getItem(int curKey) {
    HashNode* temp = hashTable->getItem(curKey);
    if (temp == nullptr) {
        return nullptr;
    } else {
        doublyLinkedList->moveNodeToHead(curKey);
        return temp->getFifoNode();
    }
}

/*getMaxCacheSize*/
int CacheManager::getMaxCacheSize() {
    return maxCacheSize;
}

/*contains*/
bool CacheManager::contains(int curKey) {
    return hashTable->contains(curKey);
}

/*printCache*/
void CacheManager::printCache() {
    std::ofstream& outFile = getOutFile();

    // Print the DoublyLinkedList contents
    std::cout << "Here are the FIFO List contents: " << std::endl;
    outFile << "Here are the FIFO List contents: " << std::endl;
    DllNode* temp = doublyLinkedList->head;
    while (temp != nullptr) {
        std::cout << "FIFO Node key: " << temp->key << std::endl;
        outFile << "FIFO Node key: " << temp->key << std::endl;
        temp = temp->next;
    }
    std::cout << "End of FIFO List" << std::endl;
    outFile << "End of FIFO List" << std::endl;

    // Print the HashTable contents
    std::cout << "Here are the Hash Table contents (" << hashTable->getNumberOfItems() << " entries):" << std::endl;
    outFile << "Here are the Hash Table contents (" << hashTable->getNumberOfItems() << " entries):" << std::endl;
    hashTable->printTable();
}