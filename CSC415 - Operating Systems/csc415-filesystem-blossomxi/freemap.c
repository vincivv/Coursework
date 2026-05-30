/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: Joshua Muhammad, John Rilles, Fidel Torres Ochoa, Dias Almat
* Student IDs:: 924074746, 922591914, 923279263, 923022045
* GitHub-Name:: JoshuaM928, blossomxi, Ochoa667, vincivv
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: freemap.c
*
* Description::
*   Implements free-space bitmap initialization (Steps 3a–3e):
*     - 3a: compute bitmap size & placement (blocks 1..N)
*     - 3b: malloc N * blockSize bytes
*     - 3c: set bits (0 = free, 1 = used) for VCB and bitmap blocks
*     - 3d: write the bitmap to disk via LBAwrite
*     - 3e: return (start, count) to the caller
*
**************************************************************/

#include "freemap.h"
#include "fsLow.h"
#include "rootDir.h"
#include "volumeControlBlock.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FSBM fsbm = {0};

/**
 * @brief Helper function used to set a bit as being used in the free space.
 *
 * @param bm
 * @param blockNumber
 * @param totalBlocks
 * @return int
 */
int setBit(uint8_t *bm, uint64_t blockNumber, uint64_t totalBlocks) {
  if (blockNumber >= totalBlocks) {
    return FREESPACE_FAILURE;
  }

  uint64_t byteIndex = blockNumber / 8;
  uint8_t bitIndex = (uint8_t)(blockNumber % 8);

  bm[byteIndex] |= (uint8_t)(1u << bitIndex);
  return FREESPACE_SUCCESS;
}

/**
 * @brief Helper function used to clear the bit as being free in the free space.
 *
 * @param bm
 * @param blockNumber
 * @param totalBlocks
 */
void clearBit(uint8_t *bm, uint64_t blockNumber, uint64_t totalBlocks) {
  if (blockNumber >= totalBlocks) {
    return;
  }

  uint64_t byteIndex = blockNumber / 8;
  uint8_t bitIndex = (uint8_t)(blockNumber % 8);
  uint8_t mask = (uint8_t) ~(1u << bitIndex);

  bm[byteIndex] &= mask;
}

/**
 * @brief Helper function used to tell the caller the status of a specified
 * block in free space.
 *
 * @param bm
 * @param blockNumber
 * @param totalBlocks
 * @return int
 */
int readBit(uint8_t *bm, uint64_t blockNumber, uint64_t totalBlocks) {
  if (blockNumber >= totalBlocks) {
    return FREESPACE_FAILURE;
  }

  return (bm[blockNumber / 8] >> (blockNumber % 8)) & 1u;
}

/**
 * @brief Helper function used when initializing the free space to mark the
 * unused bits in the last byte so the system doesn't accidentally use them.
 *
 * @param bm
 * @param blockNumber
 * @param totalBlocks
 */
void maskUnusedBits(uint8_t *bm, uint64_t blockNumber, uint64_t totalBlocks) {
  (void)blockNumber;

  uint64_t remainder = totalBlocks % 8;

  if (remainder == 0) {
    return;
  }

  uint64_t bytes = (totalBlocks + 7) / 8;

  uint8_t mask = (uint8_t) ~((1u << remainder) - 1u);

  bm[bytes - 1] |= mask;
}

/**
 * @brief This function initializes the free space, it utilizes the helper
 * functions to make sure the correct bits are marked as used when the file
 * system is formatting the drive when the volume control block signature
 * doesn't match the expected value.
 *
 * @param numberOfBlocks
 * @param blockSize
 * @return int
 */
int initFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize) {
  // Knowing the number of blocks and the size of those blocks in the volume we
  // can determine the number of bits need in our free space which gives us the
  // total number of bytes thus giving us the number blocks needed for the
  // freespace.
  uint64_t bytesNeeded = (numberOfBlocks + 7) / 8;
  uint64_t blocksNeeded = (bytesNeeded + (blockSize - 1)) / blockSize;

  size_t safeBytes = (size_t)bytesNeeded;
  if (safeBytes != bytesNeeded) {
    printf(
        "the amount of bytes needed is to large for calloc on this system.\n");

    free(fsbm.bm);
    fsbm.bm = NULL;
    return FREESPACE_FAILURE;
  }

  fsbm.bm = calloc((size_t)bytesNeeded, sizeof(uint8_t));
  if (!fsbm.bm) {
    printf("calloc failed when allocating space for the bit map.\n");

    free(fsbm.bm);
    fsbm.bm = NULL;
    return FREESPACE_FAILURE;
  }

  fsbm.blockSize = blockSize;
  fsbm.bmStart = FREESPACE_START_BLOCK;
  fsbm.bmBlocks = blocksNeeded;
  fsbm.bytesNeeded = bytesNeeded;
  fsbm.totalNumOfBlocks = numberOfBlocks;
  if ((fsbm.bmStart + fsbm.bmBlocks) > numberOfBlocks) {
    printf("size of free space is larger than the number of blocks on disk.\n");

    free(fsbm.bm);
    fsbm.bm = NULL;
    return FREESPACE_FAILURE;
  }

  printf("the value of blockSize is: %lu\n", blockSize);
  printf("the value of fsbm.blockSize is: %lu\n", fsbm.blockSize);
  printf("the value of fsbm.bmStart is: %lu\n", fsbm.bmStart);
  printf("the value of fsbm.bmBlocks is: %lu\n", fsbm.bmBlocks);
  printf("the value of fsbm.bytesNeeded is: %lu\n", fsbm.bytesNeeded);
  printf("the value of fsbm.totalNumOfBlocks is: %lu\n", fsbm.totalNumOfBlocks);
  printf("+++++++++++++++++++++++++++++++++++\n");

  setBit(fsbm.bm, 1, numberOfBlocks);
  for (uint64_t index = fsbm.bmStart; index < (fsbm.bmStart + fsbm.bmBlocks);
       index++) {
    if (setBit(fsbm.bm, index, numberOfBlocks) == FREESPACE_FAILURE) {
      printf("failed to set a bit during free space initialization.\n");

      free(fsbm.bm);
      fsbm.bm = NULL;
      return FREESPACE_FAILURE;
    }
  }

  if (vcb->numBlock != numberOfBlocks) {
    vcb->numBlock = numberOfBlocks;
  }

  vcb->rootStart = fsbm.bmStart + fsbm.bmBlocks;
  vcb->rootSize = SIZE_OF_ROOT_IN_BLOCKS;
  vcb->freeSpace = vcb->numBlock - (1 + fsbm.bmBlocks + vcb->rootSize);
  vcb->freeSpacePos = fsbm.bmStart;

  for (uint64_t index = vcb->rootStart; index < vcb->rootStart + vcb->rootSize;
       ++index) {
    if (setBit(fsbm.bm, index, numberOfBlocks) == FREESPACE_FAILURE) {
      printf("failed to set a bit during free space initialization.\n");

      free(fsbm.bm);
      fsbm.bm = NULL;
      return FREESPACE_FAILURE;
    }
  }

  maskUnusedBits(fsbm.bm, 0, numberOfBlocks);

  printf("the value of vcb->signature is : %lu\n", vcb->signature);
  printf("the value of vcb->numBlock is: %lu\n", vcb->numBlock);
  printf("the value of vcb->sizeOfBlock is: %lu\n", vcb->sizeOfBlock);
  printf("the value of vcb->freeSpacePos is: %lu\n", vcb->freeSpacePos);
  printf("the value of vcb->freeSpace is: %lu\n", vcb->freeSpace);
  printf("the value of vcb->rootStart is: %lu\n", vcb->rootStart);
  printf("the value of vcb->rootSize is: %lu\n", vcb->rootSize);

  uint8_t *buffer = calloc((size_t)blockSize * (size_t)fsbm.bmBlocks, 1);
  if (!buffer) {
    printf("failed to allocate space for the buffer when trying to write free "
           "space to disk\n");

    free(fsbm.bm);
    fsbm.bm = NULL;
    free(buffer);
    buffer = NULL;
    return FREESPACE_FAILURE;
  }
  memcpy(buffer, fsbm.bm, (size_t)fsbm.bytesNeeded);

  if (LBAwrite(buffer, fsbm.bmBlocks, fsbm.bmStart) != fsbm.bmBlocks) {
    printf("failed to write the correct number of blocks when initializing "
           "free space.\n");

    free(fsbm.bm);
    fsbm.bm = NULL;
    free(buffer);
    buffer = NULL;
    return FREESPACE_FAILURE;
  }

  free(buffer);
  buffer = NULL;
  return FREESPACE_SUCCESS;
}