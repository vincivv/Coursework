/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: Joshua Muhammad, John Rilles
* Student IDs:: 924074746, 922591914
* GitHub-Name:: JoshuaM928, blossomxi
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: volumeControlBlock.h
*
* Description:: The structure for the volume control block
*
**************************************************************/
#ifndef VOLUME_CONTROL_BLOCK_H
#define VOLUME_CONTROL_BLOCK_H

#include <stdint.h>
#include <sys/types.h>

// linking the free blocks together will help improve performance 
#define VCB_SIGNATURE 0x1234ABCD

typedef struct VCB{
	uint64_t signature; // signature for the volume
	uint64_t numBlock; // number of blocks in the volume
	uint64_t sizeOfBlock; // size of a block
	uint64_t freeSpacePos; // Block where free space begins
	uint64_t freeSpace; // number of free blocks in the volume
	uint64_t rootStart; // where the root directory starts
	uint64_t rootSize; // the number of blocks that the root directory takes up
} VCB;

extern VCB *vcb;
extern blksize_t st_blksize;

#endif // VOLUME_CONTROL_BLOCK_H