/**
* @author - Dias Almat
* @author - Ian Kligman
* @file cache_manager.cpp -  This is a cache manager files that combines doubly linked list and hash table.
* 3/16/2025 - Dias Almat created file and added comments.
* 3/16/2025 - modified by Dias Almat; added comments
* 3/19/2025 - modified by I. Kligman; added add and remove methods + comments
*/

#include "doubly_linked_list.cpp"
#include "hash_table.cpp"
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
bool CacheManager::add(int curKey, DllNode* myNode){
    
    // First, check if the cache is full. If so, remove the DLL head (least recently used item)

    if(getSize() >= maxCacheSize) {
        doublyLinkedList->removeHeaderNode();
    }
    
    // Attempt to add the node to the hash table.

    HashNode* hashNode = new HashNode(curKey, myNode);
    if (!hashTable->add(curKey, hashNode)) {
        // If insertion fails, free up dynamic memory and return false.
        delete hashNode; 
        return false; 
    }

    // Here, adding to hash was successful, so add to end of DLL.

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
    doublyLinkedList->remove(dllNode->key);

    // Free the dynamic memory and return true.
    delete hashNode;

    return true;
}

/*clear*/
void CacheManager::clear(){
    hashTable->clear();
    doublyLinkedList->clear();
    maxCacheSize = 0;
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
    std::cout << "Cache: ";
    getOutFile() << "Cache: ";
    DllNode* temp = doublyLinkedList->head;
    while (temp != nullptr) {
        std::cout << temp->key << " ";
        getOutFile() << temp->key << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
    getOutFile() << std::endl;
}