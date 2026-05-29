/**
* @author - Dias Almat
* @file doubly_linked_list.cpp -  This file contains the implementation of a doubly linked list.
* 2/05/2025 - Dias Almat created file and added comments.
* 2/10/2025 - modified by Dias Almat; added comments
*/


#include "doubly_linked_list.h"
#include <iostream>

// Constructor: Initializes an empty list
DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr) {}

// Destructor: Clears the list
DoublyLinkedList::~DoublyLinkedList() {
    clear();
}

// Check if the list is empty
bool DoublyLinkedList::isEmpty() {
    return head == nullptr;
}

// Insert a new node at the beginning of the list
void DoublyLinkedList::insertAtHead(int key) {
    DllNode* newNode = new DllNode(key);
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
}

// Insert a new node at the end of the list
void DoublyLinkedList::insertAtTail(int key) {
    DllNode* newNode = new DllNode(key);
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

// Remove a node with a specific value
void DoublyLinkedList::remove(int key) {
    DllNode* temp = head;
    while (temp && temp->key != key) {
        temp = temp->next;
    }
    if (!temp) return;  // Key not found

    if (temp == head) removeHeaderNode();
    else if (temp == tail) removeTailNode();
    else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        delete temp;
    }
}

// Remove the head (first) node
void DoublyLinkedList::removeHeaderNode() {
    if (isEmpty()) return;

    DllNode* temp = head;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }
    delete temp;
}

// Remove the tail (last) node
void DoublyLinkedList::removeTailNode() {
    if (isEmpty()) return;

    DllNode* temp = tail;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        tail = tail->prev;
        tail->next = nullptr;
    }
    delete temp;
}

// Move a node with a specific value to the head
void DoublyLinkedList::moveNodeToHead(int key) {
    DllNode* temp = head;
    while (temp && temp->key != key) {
        temp = temp->next;
    }
    if (!temp || temp == head) return;  // Node not found or already at head

    if (temp == tail) removeTailNode();
    else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    temp->prev = nullptr;
    temp->next = head;
    head->prev = temp;
    head = temp;
}

// Move a node with a specific value to the tail
void DoublyLinkedList::moveNodeToTail(int key) {
    DllNode* temp = head;
    while (temp && temp->key != key) {
        temp = temp->next;
    }
    if (!temp || temp == tail) return;  // Node not found or already at tail

    if (temp == head) removeHeaderNode();
    else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    temp->prev = tail;
    temp->next = nullptr;
    tail->next = temp;
    tail = temp;
}

// Clear the list by deleting all nodes
void DoublyLinkedList::clear() {
    while (!isEmpty()) {
        removeHeaderNode();
    }
}
