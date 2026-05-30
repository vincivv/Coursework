/**
* @author - Hugh Hui
* @file doubly_linked_list.h -  This header file declares the methods which are coded in the doubly_linked_list.cpp file.
* 12/30/2024 - H. Hui created file and added comments.
*
*/


#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "dll_node.h"
#include <iostream>
#include <fstream>
#include <string>

class DoublyLinkedList {
public:
    DllNode* head;   // Pointer to the first node (head) in the list
    DllNode* tail;   // Pointer to the last node (tail) in the list

public:
    DoublyLinkedList();  // Constructor: Initializes the list to be empty
    ~DoublyLinkedList(); // Destructor: Clears the list

    /**
    *
    * isEmpty
    *
    * Method to check if the list is empty
    *
    * @param    nothing
    *
    * @return   true if head pointer is nullptr, otherwise return false
    */
    bool isEmpty();


    /**
    *
    * insertAtHead
    *
    * Method to insert a new node at the head of the list
    *
    * @param key    key for node to insert
    *
    * @return       nothing; updates doubly linked list and size
    */
    void insertAtHead(int key);


    /**
    *
    * insertAtTail
    *
    * Method to insert a new node at the tail of the list
    *
    * @param key    key for node to insert
    *
    * @return       nothing; updates doubly linked list and size
    */
    void insertAtTail(int key);


    /**
    *
    * remove
    *
    * Method to remove a node with a specific value from the list
    *
    * @param key    key for node to insert
    *
    * @return       nothing; updates doubly linked list
    */
    void remove(int key);


    /**
    *
    * removeHeaderNode
    *
    * Method to remove the header node (first node)
    *
    * @param        none
    *
    * @return       nothing; updates doubly linked list
    */
    void removeHeaderNode();


    /**
    *
    * removeTailNode
    *
    * Method to remove the tail node (last node)
    *
    * @param        none
    *
    * @return       nothing; updates doubly linked list
    */
    void removeTailNode();


    /**
    *
    * moveNodeToHead
    *
    * Method to move a specific node to the head of the list
    *
    * @param key    key for node to move
    *
    * @return       nothing; updates doubly linked list
    */
    void moveNodeToHead(int key);


    /**
    *
    * moveNodeToTail
    *
    * Method to move a specific node to the tail of the list
    *
    * @param key    key for node to move
    *
    * @return       nothing; updates doubly linked list
    */
    void moveNodeToTail(int key);


    /**
    *
    * clear
    *
    * Method to clear the list by deleting all nodes
    *
    * @param        none
    *
    * @return       nothing; reset size to 0
    */
    void clear();
};

#endif
