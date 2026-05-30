/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name::  Dias Almat
* Student ID::  923022045
* GitHub-Name::
* Project:: Assignment 4 – Processing FLR Data with Threads
*
* File:: Almat_Dias_HW4_hashMap.h
*
* Description::  Header file for a hash map to store and compute statistics for police call data.
*
**************************************************************/

#ifndef ALMAT_DIAS_HW4_HASHMAP_H
#define ALMAT_DIAS_HW4_HASHMAP_H

#include <pthread.h>

#define TABLE_SIZE 1031
#define MAX_KEY_SIZE 64


typedef struct Stats{
    int *values;
    int size;
    int capacity;
    long count;
    long min, max;
    long sum;
    long sumSq;

    long q1, median, q3;
    long iqr, lb, ub;
    double mean, stddev;

} Stats;

typedef struct HashNode{
    char key[MAX_KEY_SIZE];
    Stats dr;
    Stats se;
    Stats sr;
    pthread_mutex_t lock;
    struct HashNode *next;
} HashNode;

typedef struct HashMap{
    HashNode *slots[TABLE_SIZE];
    pthread_mutex_t slotLocks[TABLE_SIZE];
} HashMap;


void initHashMap(HashMap *map);
void freeHashMap(HashMap *map);
void addToHashMap(HashMap *map, const char *key, int drValue, int haveDR,
     int seValue, int haveSE, 
     int srValue, int haveSR);
void finalizeHashMap(HashMap *map);
#endif