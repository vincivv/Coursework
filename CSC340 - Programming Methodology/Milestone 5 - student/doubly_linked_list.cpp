/**
* @author - Dias Almat
* @file doubly_linked_list.cpp -  file with the method implementations for the Doubly Linked List class.
* 05/7/2025 - D.Almat created file and added comments.
*/

#include "doubly_linked_list.h"
#include <iostream>

extern std::ofstream& getOutFile();

DoublyLinkedList::DoublyLinkedList(){
    head = nullptr;
    tail = nullptr;
}

DoublyLinkedList::~DoublyLinkedList() {
    DllNode* curr = head;
    while (curr != nullptr) {
        DllNode* next = curr->next;
        delete curr;
        curr = next;
    }
}

 int DoublyLinkedList::getSize() {
    int size = 0;
    DllNode* curr = head;

    while (curr != nullptr) {
        // Exhaust the list and count each member in size.
        size++;
        curr = curr->next;
    }

    return size;
}

bool DoublyLinkedList::isEmpty(){
    if (head == nullptr){
        return true;
    }
    else {
        return false;
    }
}

void DoublyLinkedList::insertAtHead(int key, DllNode* newNode){
    
    // Add the given key to the given node.

    newNode->key = key;

    // If the list is empty, the given node will be head and tail.

    if (isEmpty()){
        head = tail = newNode;
    }

    // Point new node to head, then head to new node.
    // Then move head back one position, which will be new node.

    else{
        newNode->next = head;
        head->prev = newNode;
        head = head->prev;
    }
}

void DoublyLinkedList::insertAtTail(int key, DllNode* newNode){
    
    // Add the given key to the given node.

    newNode->key = key;

    // If the list is empty, the given node will be head and tail.

    if (isEmpty()){
        head = tail = newNode;
    }

    // Point new node to tail, then tail to new node.
    // Then move tail forward one position, which will be new node.

    else{
        newNode->prev = tail;
        tail->next = newNode;
        tail = tail->next;
    }
}

void DoublyLinkedList::remove(int key) {
    DllNode* curr = head;

    // Find the node with the given key
    while (curr != nullptr && curr->key != key) {
        curr = curr->next;
    }

    // If the node is not found, return
    if (curr == nullptr) {
        return;
    }

    // If the node is the head
    if (curr == head) {
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr; // List is now empty
        }
    }
    // If the node is the tail
    else if (curr == tail) {
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        }
    }
    // If the node is in the middle
    else {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
    }

    delete curr;
}

void DoublyLinkedList::removeHeaderNode(){
    if (isEmpty()){
        return;
    }

    // This method does not iterate and simply removes the head.

    if (head->next != nullptr){
        DllNode* temp = head;
        head = head->next;
        delete temp;
        head->prev = nullptr;
    }

    // If there is only one node, reset the list to an empty one.

    else {
        delete head;
        head = tail = nullptr;
    }
}

void DoublyLinkedList::removeTailNode(){
    if (isEmpty()){
        return;
    }

    // This method does not iterate and simply removes the tail.

    if (tail->prev != nullptr){
        DllNode* temp = tail;
        tail = tail->prev;
        delete temp;
        tail->next = nullptr;
    }

    // If there is only one node, reset the list to an empty one.

    else {
        delete head;
        head = tail = nullptr;
    }
}

void DoublyLinkedList::moveNodeToHead(int key){
    if (isEmpty()){
        return;
    }

    // Finding the key requires iterating through the list.

    DllNode* curr = head;
    while (curr != nullptr && curr->key != key) {
        curr = curr->next;
    }

    // Check that the node was found. If the node is already head, do nothing.

    if (curr == nullptr || curr == head) {
        return;
    }

    // Unlink the selected node from its neighbors' prev and next pointers.

    curr->prev->next = curr->next;
    if (curr->next != nullptr){
        curr->next->prev = curr->prev;
    }

    // If the selected node is tail, update tail.

    if (curr == tail){
        tail = curr->prev;
    }

    // Finally, position curr at the head.

    curr->next = head;
    curr->prev = nullptr;
    head->prev = curr;
    head = curr;
}

void DoublyLinkedList::moveNodeToTail(int key){
    if (isEmpty()){
        return;
    }

    // Finding the key requires iterating through the list.

    DllNode* curr = head;
    while (curr != nullptr && curr->key != key) {
        curr = curr->next;
    }

    // Check that the node was found. If the node is already tail, do nothing.

    if (curr == nullptr || curr == tail) {
        return;
    }

    // Unlink the selected node from its neighbors' prev and next pointers.
    curr->next->prev = curr->prev;
    if (curr->prev != nullptr){
        curr->prev->next = curr->next;
    }

    // If the selected node is head, update head.

    if (curr == head){
        head = curr->next;
    }

    // Finally, position curr at the tail.

    curr->next = nullptr;
    curr->prev = tail;
    tail->next = curr;
    tail = curr;
}

void DoublyLinkedList::clear(){
    while (!isEmpty()) {
        removeHeaderNode();
    }
    head = nullptr;
    tail = nullptr;
}

void printList(DoublyLinkedList& FIFOList){
    std::ofstream& outFile = getOutFile();

    // Iterate throught the list via node that starts at the DoublyLinkedList's head.

    DllNode* curr = FIFOList.head;

    while (curr != nullptr){
        std::cout << curr->key << " ";
        outFile << curr->key << " ";
        curr = curr->next;
    }

    std::cout << "\n";
    outFile << "\n";
}

void reversePrintList(DoublyLinkedList& FIFOList){
    std::ofstream& outFile = getOutFile();

    // Iterate throught the list via node that starts at the DoublyLinkedList's tail.

    DllNode* curr = FIFOList.tail;

    while (curr != nullptr){
        std::cout << curr->key << " ";
        outFile << curr->key << " ";
        curr = curr->prev;
    }

    std::cout << "\n";
    outFile << "\n";
}