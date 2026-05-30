/**
* @author - Hugh Hui
* @file generate_output.h -  This header file declares the methods in the generate_output.cpp file.
* 1/16/2025 - H. Hui created file and added comments.
* 1/15/2025 - modified by H. Hui; modified remove methods; added comments
*/
#ifndef GENERATEOUTPUT_H
#define GENERATEOUTPUT_H

#include <fstream>
#include "dll_node.h"
#include "doubly_linked_list.h"

extern std::ofstream& getOutFile();

/**
*
* printList
*
* Method to print the doubly linked list from head to tail to console and output file
*
* @param dll		doubly linked list to output
*
* @return			nothing
*/
void printList(DoublyLinkedList& FIFOList);


/**
*
* reversePrintList
*
* Method to print the doubly linked list from tail to head to console and output file
*
* @param dll		doubly linked list to output
*
* @return			nothing
*/
void reversePrintList(DoublyLinkedList& FIFOList);

#endif // GENERATEOUTPUT_H
