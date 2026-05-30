/**
*
* @author - Hugh Hui
* @file dll_node.h -  This header file defines the dll_node (Doubly Linked List Node) structure.

12/30/2024 - initial creation

*/
#ifndef DLL_NODE_H
#define DLL_NODE_H

#include <string>

// Node structure for Doubly Linked List
class DllNode {
public:
    int key;       // key of the node (integer)

    std::string fullName;
    std::string address;
    std::string city;
    std::string state;
    std::string zip;

    DllNode* prev;    // Pointer to the previous node in the list
    DllNode* next;    // Pointer to the next node in the list

    DllNode(int value);

    // Constructor to initialize a node with a specific value
    DllNode(int value, std::string fullName, std::string address, std::string city, std::string state, std::string zip);


    /**
    *
    * printNode
    *
    * Method to print the doubly linked list node data
    *
    * @param dll	none
    *
    * @return		nothing
    */
    void printNode();
};

#endif
