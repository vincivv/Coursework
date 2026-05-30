/**
*
* hash_node.h : This is the node struct header file for HashTable, used in hash_table.cpp
*
* 01/16/25 - Created by H. Hui
*/

#ifndef _HASH_NODE
#define _HASH_NODE

// Define a structure for a node in the hash table
struct HashNode {
	
	int key;                                                           // Value stored in the node
	int hashCode;													   // hash code based on array size

	std::string fullName;
	std::string address;
	std::string city;
	std::string state;
	std::string zip;

	HashNode* prev;                                                     // Pointer to the previous node
	HashNode* next;                                                     // Pointer to the next node

	// Constructor to initialize a new node with values
	HashNode(int val, std::string myFullName, std::string myAddress, std::string myCity, std::string myState, std::string myZip) : 
		key(val), fullName(myFullName), address(myAddress), city(myCity), state(myState), zip(myZip), prev(nullptr), next(nullptr) {}
};

#endif 