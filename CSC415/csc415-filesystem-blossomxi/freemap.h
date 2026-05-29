/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: Fidel Torres Ochoa, Joshua Muhammad
* Student IDs:: 923279263, 924074746
* GitHub-Name:: Ochoa667, JoshuaM928
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: freemap.h
*
* Description::
*   Interface for initializing the free-space bitmap (Steps 3a–3e).
*   - Computes bitmap size/placement, allocates/initializes it,
*     writes it to disk, and returns start/count to the caller.
**************************************************************/

#ifndef FREEMAP_H
#define FREEMAP_H

// Indicates that something went wrong while initializing the free space with the return value -1.
#define FREESPACE_FAILURE -1
// Indicates that the free space was setup successfully.
#define FREESPACE_SUCCESS 0
// Starting block of the free space.
#define FREESPACE_START_BLOCK 2

#include "volumeControlBlock.h"
#include <stdint.h>

// free space bit map(FSBM) structure used to manipulate the free space in memory.
typedef struct FSBM {
    uint8_t * bm; // pointer to the memory block(s) that maps to the bits to the blocks.
    uint64_t blockSize; // size of a block.
    uint64_t bmStart; // first block that contains the free space.
    uint64_t bmBlocks; // number of blocks that make up the size of the free space.
    uint64_t bytesNeeded; // size of bitmap in bytes.
    uint64_t totalNumOfBlocks; // total number of block in the volume.
} FSBM;

extern FSBM fsbm;

/**
 * initFreeSpace
 * Steps 3a–3e:
 *  - 3a: compute bitmap size & placement (bitmap at blocks 1..N).
 *  - 3b: malloc N * blockSize bytes.
 *  - 3c: mark used bits (VCB + bitmap blocks).
 *  - 3d: LBAwrite to disk starting from block 1.
 *  - 3e: update the VCB global structure accordingly.
 *
 * Inputs:
 *  Total number of blocks in the volume, & the size of a block.
 * Return:
 *  0 on success, -1 on failure.
 */
int initFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize);

// Sets the target bit to 1 indicating that the block is in use.
int setBit(uint8_t *bm, uint64_t blockNumber, uint64_t totalBlocks);
// Sets the target bit to 0 indicating that the block is free.
void clearBit(uint8_t *bm, uint64_t blockNumber, uint64_t totalBlocks);
// Returns the target bit's value, on failure returns -1.
int readBit(uint8_t *bm, uint64_t blockNumber, uint64_t totalBlocks);
// Mask off the unused bits in the last byte when total number of blocks is not a multiple of 8.
void maskUnusedBits(uint8_t *bm, uint64_t blockNumber, uint64_t totalBlocks);

#endif // FREEMAP_H
