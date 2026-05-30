/**
*
* hash_table.hpp : This is the header file for hash_table.cpp.
*
* 09/23/24 - Created by ChatGPT
* 10/17/24 - Modified by jhui
* 01/11/25 - Modified by hhui; 1) added calculateHashCode, 2) modified methods to include hash table size parameter
* 01/16/2025 - Modified by hhui; created separate node structure file
*/

#ifndef _HASH_TABLE
#define _HASH_TABLE

#include "hash_node.h"

// Define a class for the hash table 
class HashTable {
private:
	HashNode** table;
	int numberOfItems;
	int numberOfBuckets;

public:
	// Default constructor
	HashTable();
	
	// Constructor initializes an empty list
	HashTable(int hashTableSize) : numberOfBuckets(hashTableSize), numberOfItems(0) {
		table = new HashNode * [hashTableSize];
		std::cout << "hashTableSize: " << hashTableSize << std::endl;
		for (int i = 0; i < hashTableSize; i++) {
			table[i] = nullptr;
		}
	}

	/**
	*
	* getTable
	*
	* Method to return the hash table
	*
	* @param    none
	*
	* @return   pointer to the hash table array
	*/
	HashNode** getTable();

	/**
	*
	* getSize
	*
	* Method to return the size of the hash table
	*
	* @param    none
	*
	* @return	size of the hash table array
	*/
	int getSize();

	/**
	*
	* calculateHashCode
	*
	* Method to calculate the hashCode
	*
	* @param    currentKey     hash table size
	*
	* @return   hashcode for currentKey
	*/	int calculateHashCode(int currentKey);

	/**
	*
	* isEmpty
	*
	* Method to check if HashTable is empty
	*
	* @param    none
	*
	* @return   true if the table has zero entries, false otherwise
	*/
	bool isEmpty();

	/**
	*
	* getNumberOfItems
	*
	* Method to return number of items in the hash table
	*
	* @param    none
	*
	* @return   number of items in the table
	*/
	int getNumberOfItems();

	/**
	*
	* add
	*
	* Method to add a node to the hash table
	*
	* @param    curKey    key for this node
	* @param    myNode    new node to add to the table
	*
	* @return   true if success, false otherwise
	*/
	bool add(int curKey, HashNode* myNode);

	/**
	*
	* remove
	*
	* Method to remove node with curKey
	*
	* @param    key     key of node to remove
	*
	* @return   true if success, false otherwise
	*/
	bool remove(int curKey);

	/**
	*
	* clear
	*
	* Method to remove all entries from the table
	*
	* @param    none
	*
	* @return   nothing, but will delete all entries from the table
	*/
	void clear();

	/**
	*
	* getItem
	*
	* Method to retrieve item from the hash table
	*
	* @param    curKey  retrieve node from hash table with curKey value
	*
	* @return   pointer to the HashNode
	*/
	HashNode* getItem(int curKey);


	/**
	*
	* contains
	*
	* Method to verify if a key is in the hash table
	*
	* @param    curKey  check key for hash table with curKey value
	*
	* @return   true if found, false otherwise
	*/
	bool contains(int curKey);

	/**
	*
	* printTable
	*
	* Method to print out the contents of table
	*
	* @param    none
	*
	* @return   nothing, but output is sent to console and to file
	*/
	void printTable();


}; // end HashTable



#endif 