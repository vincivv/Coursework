/**
* @author - Dias Almat
* @file doubly_linked_list.cpp -  This file contains the implementation of a doubly linked list.
* 3/16/2025 - Dias Almat created file and added comments.
* 3/16/2025 - modified by Dias Almat; added comments
*/


#include "doubly_linked_list.h"
#include "cache_manager.h"
#include <iostream>

// Constructor
DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr) {}

//Destructor
DoublyLinkedList::~DoublyLinkedList() {
    DllNode* current = head;
    while (current != nullptr) {
        DllNode* next = current->next;
        delete current;  // Free the node
        current = next;
    }
}

/*getSize*/
int DoublyLinkedList::getSize(){
    return size;
}

/*isEmpty*/
bool DoublyLinkedList::isEmpty() {
    return head == nullptr;
}

/*insertAtHead*/
void DoublyLinkedList::insertAtHead(int key, DllNode* newNode){
    if(isEmpty()){
        head = tail = newNode;
    }else{
        tail -> next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

/*insertAtTail*/
void DoublyLinkedList::insertAtTail(int key, DllNode* newNode){
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

/*remove*/
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

/*removeHeaderNode*/
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

/*removeTailNode*/
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

/*moveNodeToHead*/
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

/*moveNodeToTail*/
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

/*clear*/
void DoublyLinkedList::clear() {
    while (!isEmpty()) {
        removeHeaderNode();
    }
}

/*printList*/
void printList(DoublyLinkedList& dll) {
    std::ofstream& outFile = getOutFile();
    
    DllNode* temp = dll.head;
    while (temp) {
        std::cout << temp->key << " ";
        outFile << temp->key << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
    outFile << std::endl;
}

/*reversePrintList*/
void reversePrintList(DoublyLinkedList& dll) {
    std::ofstream& outFile = getOutFile();
    
    DllNode* temp = dll.tail;
    while (temp) {
        std::cout << temp->key << " ";
        outFile << temp->key << " ";
        temp = temp->prev;
    }
    std::cout << std::endl;
    outFile << std::endl;
}