/**
*
* hash_node.h : This is the node struct header file for HashNode, used in hash_table.cpp
*
* 01/16/25 - Created by H. Hui
*/

#ifndef _HASH_NODE
#define _HASH_NODE

#include "dll_node.h"

// Define a structure for a node in the hash table
class HashNode {
public:	
	int key;                                                            // Value stored in the node
	int hashCode;													    // hash code based on array size
	HashNode* next;
	HashNode* prev;

	DllNode* fifoNode;													// pointer to the FIFO node

	HashNode(int myKey, DllNode* myFifoNode);

	DllNode* getFifoNode();
};


#endif 