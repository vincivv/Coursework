/**
* @author - Dias Almat
* @file cache_manager.cpp -  This is a cache manager files that combines doubly linked list and hash table.
* 05/7/2025 - D.Almat created file and added comments.
*/

#include "cache_manager.h"
#include <iostream>

extern std::ofstream& getOutFile();

// Get table
HashTable* CacheManager::getTable() {
    return hashTable;
}

// Get list
DoublyLinkedList* CacheManager::getList() {
    return doublyLinkedList;
}

// Get BST
BinarySearchTree* CacheManager::getBst() {
    return binarySearchTree;
}

// Get size
int CacheManager::getSize() {
    return hashTable->getNumberOfItems();
}

// Checks if the hash table is empty
bool CacheManager::isEmpty() {
    return hashTable->isEmpty();
}

// Add a node to the cache manager
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

    binarySearchTree->addToTree(curKey, myNode);

    return true;
}

// Remove a node with the specified key
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

    binarySearchTree->removeNode(curKey);

    return true;
}

// Clear the cache manager
void CacheManager::clear(){
    hashTable->clear();
    doublyLinkedList->clear();
    binarySearchTree->clear();
}

// Get item from the cache manager
DllNode* CacheManager::getItem(int curKey) {
    HashNode* temp = hashTable->getItem(curKey);
    if (temp == nullptr) {
        return nullptr;
    } else {
        doublyLinkedList->moveNodeToHead(curKey);
        return temp->getFifoNode();
    }
}

//Get max cache size
int CacheManager::getMaxCacheSize() {
    return maxCacheSize;
}

//Check if the cache manager contains a key
bool CacheManager::contains(int curKey) {
    return hashTable->contains(curKey);
}

// Print the cache manager
void CacheManager::printCache() {
    std::ofstream& outFile = getOutFile();
    
    std::cout << "Printing out the cache: " << std::endl;
    getOutFile() << "Printing out the cache: " << std::endl;
    
    // Print the DoublyLinkedList contents
    std::cout << "Here are the FIFO List contents: " << std::endl;
    outFile << "Here are the FIFO List contents: " << std::endl;
    DllNode* temp = doublyLinkedList->head;
    while (temp != nullptr) {
        std::cout << temp->key << " \n";
        outFile << temp->key << " \n";
        temp = temp->next;
    }
    std::cout << "\nEnd of FIFO List\n" << std::endl;
    outFile << "\nEnd of FIFO List\n" << std::endl;

    // Print the HashTable contents
    std::cout << "Here are the Hash Table contents (" << hashTable->getNumberOfItems() << " entries):" << std::endl;
    outFile << "Here are the Hash Table contents (" << hashTable->getNumberOfItems() << " entries):" << std::endl;
    hashTable->printTable();
}

//Print Range
void CacheManager::printRange(int low, int high) {
    std::cout << "Performing Range traversal" << std::endl;
    getOutFile() << "Performing Range traversal" << std::endl;
    binarySearchTree->printRange(low, high);
}

// Sort
void CacheManager::sort(bool ascending) {
    std::ofstream& outFile = getOutFile();

    if (ascending) {
        binarySearchTree->printInOrder();
    } else {
        binarySearchTree->printReverseOrder();
    }
}