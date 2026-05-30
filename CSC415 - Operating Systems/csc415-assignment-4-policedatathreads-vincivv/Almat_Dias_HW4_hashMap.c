/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name::  Dias Almat
* Student ID::  923022045
* GitHub-Name::
* Project:: Assignment 4 – Processing FLR Data with Threads
*
* File:: Almat_Dias_HW4_hashMap.c
*
* Description:: Implementation of a hash map to store and compute statistics for police call data.
                Contains functions for initializing, adding data, finalizing (computing stats).
*
**************************************************************/

#include "Almat_Dias_HW4_hashMap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>


//Insertion sort to sort the data
static void sortData(int *data, int size) {
    for (int i = 1; i < size; i++) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

//Helper func to compute the median, is used in computeStats
static int getMedian(int *data, int size) {
    if (size == 0) return 0;
    if (size % 2 == 1) {
        return data[size / 2];
    } else {
        return (data[(size / 2) - 1] + data[size / 2]) / 2;
    }
}

//Compute all the stats
static void computeStats(Stats *stats) {
    if (stats->count == 0) return;

    sortData(stats->values, stats->size);

    //compute median, q1 and q3
    if (stats->size == 1) {
        stats->median = stats->values[0];
        stats->q1 = stats->values[0];
        stats->q3 = stats->values[0];
    } else if (stats->size == 2) {
        stats->median = (stats->values[0] + stats->values[1]) / 2;
        stats->q1 = stats->values[0];
        stats->q3 = stats->values[1];
    } else if (stats->size == 3) {
        stats->median = stats->values[1];
        stats->q1 = stats->values[0];
        stats->q3 = stats->values[2];
    } else {
        int mid = stats->size / 2;
        if (stats->size % 2 == 1) { 
            stats->median = stats->values[mid];
            stats->q1 = getMedian(stats->values, mid); 
            stats->q3 = getMedian(stats->values + mid + 1, mid);    
        } else { 
            stats->median = (stats->values[mid - 1] + stats->values[mid]) / 2;
            stats->q1 = getMedian(stats->values, mid); 
            stats->q3 = getMedian(stats->values + mid, mid);
        }
    }

    //compute IQR, lower and upper bounds
    stats->iqr = stats->q3 - stats->q1;
    stats->lb = stats->q1 - 1.5 * stats->iqr;
    stats->ub = stats->q3 + 1.5 * stats->iqr;

    
    //compute mean and standard deviation
    stats->mean = (double) stats->sum / (double)stats->count;
    double variance = ((double)stats->sumSq / stats->count) - ((double)stats->mean * stats->mean);
    stats->stddev = (long)(sqrt(variance) + 0.5); 
}

//DJB2 hash function
static unsigned long hashFunction(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; 
    }

    return hash % TABLE_SIZE;
}

//intalize stats
static void statsInit(Stats *stats) {
    stats ->values = malloc(4096 * sizeof(int));
    stats->size = 0;
    stats->capacity = 4096;
    stats->count = 0;
    stats->min = 999999999;
    stats->max = -999999999;
    stats->sum = 0;
    stats->sumSq = 0;
}

//add a value to stats
static void statsAddValue(Stats *stats, int value) {
    if (stats->size == stats->capacity) {
        int newcap = stats->capacity * 2;
        int *nv = realloc(stats->values, newcap * sizeof(int));
        if (!nv) return;
        stats->values = nv;
        stats->capacity = newcap;
    }
    stats->values[stats->size++] = value;
    stats->count++;
    stats->sum += value;
    stats->sumSq += value * value;
    if (value < stats->min) stats->min = value;
    if (value > stats->max) stats->max = value;
}

//free stats memory
static void statsFree(Stats *stats) {
    free(stats->values);
    stats->values = NULL;
    stats->size = 0;
    stats->capacity = 0;
    stats->count = 0;
    stats->min = 0;
    stats->max = 0;
    stats->sum = 0;
    stats->sumSq = 0;
}

//create a new hash node
static HashNode* createNode(const char *key) {
    HashNode *node = malloc(sizeof(HashNode));
    if(!node) return NULL;
    strncpy(node->key, key, MAX_KEY_SIZE-1);
    node->key[MAX_KEY_SIZE-1] = '\0';
    statsInit(&node->dr);
    statsInit(&node->se);
    statsInit(&node->sr);
    pthread_mutex_init(&node->lock, NULL);
    node->next = NULL;
    return node;
}

//initialize the hash map
void initHashMap(HashMap *map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        map->slots[i] = NULL;
        pthread_mutex_init(&map->slotLocks[i], NULL);
    }
}

//free the hash map
void freeHashMap(HashMap *map) {
    for (int i = 0; i < TABLE_SIZE; i++) {

        HashNode *node = map->slots[i];
        while (node) {
            HashNode *next = node->next;
            pthread_mutex_destroy(&node->lock);
            statsFree(&node->dr);
            statsFree(&node->se);
            statsFree(&node->sr);
            free(node);
            node = next;
        }
        pthread_mutex_destroy(&map->slotLocks[i]);
        map->slots[i] = NULL;

    }
}

//Add data to the hash map
void addToHashMap(HashMap *map, const char *key, int drValue, int haveDR,
                  int seValue, int haveSE, 
                  int srValue, int haveSR) {
    unsigned long index = hashFunction(key);
    pthread_mutex_lock(&map->slotLocks[index]);

    HashNode *node = map->slots[index];
    while(node && strcmp(node->key, key) != 0) {
        node = node->next;
    }
    if(!node) {
        node = createNode(key);
        node->next = map->slots[index];
        map->slots[index] = node;
    }
    pthread_mutex_unlock(&map->slotLocks[index]);

    pthread_mutex_lock(&node->lock);
    if(haveDR) statsAddValue(&node->dr, drValue);
    if(haveSE) statsAddValue(&node->se, seValue);
    if(haveSR) statsAddValue(&node->sr, srValue);
    pthread_mutex_unlock(&node->lock);

}

//helper func to finalize the hash map by computing stats for each node
void finalizeHashMap(HashMap *map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        pthread_mutex_lock(&map->slotLocks[i]);
        HashNode *node = map->slots[i];
        while (node) {
            computeStats(&node->dr);
            computeStats(&node->se);
            computeStats(&node->sr);
            node = node->next;
        }
        pthread_mutex_unlock(&map->slotLocks[i]);
    }
}
