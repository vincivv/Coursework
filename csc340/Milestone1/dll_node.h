/**
*
* @author - Hugh Hui
* @file node.h -  This header file defines the node structure.

12/30/2024 - initial creation

*/


#ifndef NODE_H
#define NODE_H

// Node structure for Doubly Linked List
struct DllNode {
    int key;          // Key of the node (integer)
    DllNode* prev;    // Pointer to the previous node in the list
    DllNode* next;    // Pointer to the next node in the list

    // Constructor to initialize a node with a specific key
    DllNode(int key);
};

#endif
