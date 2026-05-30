/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: Joshua Muhammad, Dias Almat
* Student IDs:: 924074746, 923022045
* GitHub-Name:: JoshuaM928, vincivv
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: fsInit.c
*
* Description:: Main driver for file system assignment.
*
* This file is where you will start and initialize your system
*
**************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "freemap.h"
#include "fsLow.h"
#include "mfs.h"
#include "rootDir.h"
#include "volumeControlBlock.h"

VCB *vcb = NULL;
blksize_t st_blksize = 0;


/**
 * @brief initFileSystem is responsible for making sure the various parts of the
 * file system are initialized properly when starting the program. It is
 * responsible for making sure the volume control block is on disk, that the
 * root directory is setup, and the free space is tracking all the various
 * things on disk.
 *
 * @param numberOfBlocks
 * @param blockSize
 * @return int
 */
int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize) {
  printf("Initializing File System with %ld blocks with a block size of %ld\n",
         numberOfBlocks, blockSize);

  st_blksize = (blksize_t)blockSize;

  // By reading block 0 from disk we can determine if we need to format the
  // volume or not.
  char *buffer = malloc(blockSize);
  if (!buffer) {
    printf("Failed to allocate space in memory for block 0\n");
    return EXIT_FAILURE;
  }

  int blocksRead = 1;
  if (LBAread(buffer, blocksRead, 0) != blocksRead) {
    printf("Failed to read in correct number of blocks when initializing the "
           "file system\n");
    return EXIT_FAILURE;
  }

  vcb = (VCB *)buffer;

  if (vcb->signature != VCB_SIGNATURE) {
    // Initializing VCB Fields
    vcb->signature = VCB_SIGNATURE;
    vcb->numBlock = numberOfBlocks; // # number of blocks
    vcb->sizeOfBlock = blockSize;   // size of blocks

    // Initialize free space. Initializing free space initializes parts of the
    // VCB.
    if (initFreeSpace(numberOfBlocks, blockSize) == FREESPACE_FAILURE) {
      printf("failed to initialize freespace\n");
      return EXIT_FAILURE;
    }

    // Initialize root directory
    int minEntries = 50;
    DE *root = createDir(minEntries, NULL, blockSize, vcb->rootStart);

    // Write VCB to disk
    memcpy(buffer, vcb, sizeof(VCB));

    int blocksWritten = 1;
    if (LBAwrite(buffer, blocksWritten, 0) != blocksWritten) {
      printf("LBAwrite didn't write the correct number of blocks: %d",
             blocksWritten);
      return EXIT_FAILURE;
    }

    printf("successfully initialize volume.\n");
  }

  return EXIT_SUCCESS;
}

void exitFileSystem() { printf("System exiting\n"); }