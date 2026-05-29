/**
* @author - Dias Almat
* @file generate_output.cpp -  This file contains the implementation of functions to generate output from a doubly linked list.
* 2/05/2025 - Dias Almat created file and added comments.
* 2/10/2025 - modified by Dias Almat; added comments
*/


#include "generate_output.h"
#include <iostream>

// Print the list from head to tail
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

// Print the list from tail to head
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
