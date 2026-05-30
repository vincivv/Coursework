/**************************************************************
* Class::  CSC-415-03 Fall 2025
* Name:: Joshua Muhammad, John Rilles, Dias Almat
* Student IDs:: 924074746, 922591914, 923022045
* GitHub-Name:: JoshuaM928, blossomxi, vincivv
* Group-Name:: Four Blobs
* Project:: Basic File System
*
* File:: rootDir.h
*
* Description:: This file defines our directory entry structure, allowing us to
* create directories we can also use this structure to access files. It servers
* as our Inode structure in our file system.
*
**************************************************************/

#ifndef ROOTDIR_H
#define ROOTDIR_H

#define SIZE_OF_ROOT_IN_BLOCKS 11;

#include "freemap.h"
#include "mfs.h"
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

typedef struct dirEntry {
  char name[64];           // Name of the directory entry.
  uint64_t startingBlock;  // Index of the first block of the file.
  uint64_t numberOfBlocks; // Size of file in blocks.
  time_t created;          // Time the directory entry was created.
  time_t lastModified;     // Last time the directory entry was modified.
  time_t accessed;         // Last time the directory entry was accessed.
  int sizeInBytes;         // Number of bytes in the file.
  int size;                // Number of entires in the array.
  int isDirectory;         // Directory entry is NOT a file.
  int inUse;               // Current index in the directory.
  int location;            // Index of the first block of the directory.
} DE;

extern DE *currentDir;
extern DE *rootDir;
extern blksize_t st_blksize;

DE *createDir(int minDirEntries, DE *parent, int blockSize, int startBlock);

#endif
