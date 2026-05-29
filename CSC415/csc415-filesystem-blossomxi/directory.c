

/**************************************************************
* Class::  CSC-415-0# Fall 2025
* Name:: Fidel Torres Ochoa
* Github:: Ochoa667
* Project:: Basic File System
*
* File:: directory.c
*
* Description::
*   Implementation of:
*     - fs_mkdir
*     - fs_opendir
*     - fs_readdir
*     - fs_closedir
*   
**************************************************************/

#include "mfs.h"                 // fdDir, fs_diriteminfo, FT_DIRECTORY, FT_REGFILE
#include "fsLow.h"               // LBAread, LBAwrite
#include "rootDir.h"             // DE struct
#include "freemap.h"             // free space allocator (implementation elsewhere)
#include "volumeControlBlock.h"  // VCB, extern VCB *vcb

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>

#ifndef MAXFILENAME
#define MAXFILENAME 64   // matches DE.name[64] from rootDir.h
#endif

/* allocFreeBlock is implemented elsewhere; declare it here
 * so the compiler knows about it without editing freemap.h.
 */
extern int allocFreeBlock(void);

/* -----------------------------------------------------------
   Local helpers & directory handle registry
   
----------------------------------------------------------- */

typedef struct {
    fdDir *handle;   // pointer given to user
    DE    *buffer;   // loaded directory block
    int    lba;      // starting block for this directory
} DirHandle;

#define MAX_OPEN_DIRS 64
static DirHandle g_dirTable[MAX_OPEN_DIRS] = {0};

/* how many DE entries fit in one block */
static inline int entries_per_block(void) {
    return (int)(vcb->sizeOfBlock / sizeof(DE));
}

/* load one directory block into memory */
static DE *load_directory_block(int startBlock) {
    DE *buf = (DE *)malloc(vcb->sizeOfBlock);
    if (!buf) return NULL;

    if (LBAread(buf, 1, startBlock) != 1) {
        free(buf);
        return NULL;
    }
    return buf;
}

/* find an entry by name inside a loaded directory block */
static int find_entry_in_dir(DE *dir, const char *name) {
    int n = entries_per_block();
    for (int i = 0; i < n; i++) {
        if (dir[i].inUse && strcmp(dir[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* find first free slot in a directory block */
static int find_free_slot(DE *dir) {
    int n = entries_per_block();
    for (int i = 0; i < n; i++) {
        if (!dir[i].inUse) {
            return i;
        }
    }
    return -1;
}

/* register a directory buffer for an fdDir handle */
static int dir_register(fdDir *h, DE *buf, int lba) {
    for (int i = 0; i < MAX_OPEN_DIRS; i++) {
        if (g_dirTable[i].handle == NULL) {
            g_dirTable[i].handle = h;
            g_dirTable[i].buffer = buf;
            g_dirTable[i].lba    = lba;
            return 0;
        }
    }
    errno = EMFILE; // too many open dirs
    return -1;
}

/* get directory buffer associated with handle */
static DE *dir_get_buffer(fdDir *h) {
    for (int i = 0; i < MAX_OPEN_DIRS; i++) {
        if (g_dirTable[i].handle == h) {
            return g_dirTable[i].buffer;
        }
    }
    return NULL;
}

/* get LBA for directory handle (currently unused but handy) */
static int dir_get_lba(fdDir *h) {
    for (int i = 0; i < MAX_OPEN_DIRS; i++) {
        if (g_dirTable[i].handle == h) {
            return g_dirTable[i].lba;
        }
    }
    return -1;
}

/* unregister & clear a handle (does not free buffer itself) */
static void dir_unregister(fdDir *h) {
    for (int i = 0; i < MAX_OPEN_DIRS; i++) {
        if (g_dirTable[i].handle == h) {
            g_dirTable[i].handle = NULL;
            g_dirTable[i].buffer = NULL;
            g_dirTable[i].lba    = 0;
            return;
        }
    }
}


static int load_dir_by_path(const char *path, DE **dirOut, int *lbaOut) {
    if (!path || path[0] != '/') {
        errno = EINVAL;
        return -1;
    }

    // Root directory
    if (strcmp(path, "/") == 0) {
        DE *root = load_directory_block(vcb->rootStart);
        if (!root) {
            errno = EIO;
            return -1;
        }
        *dirOut = root;
        *lbaOut = vcb->rootStart;
        return 0;
    }

    // Make a modifiable copy
    char *copy = strdup(path);
    if (!copy) {
        errno = ENOMEM;
        return -1;
    }

    DE *current = load_directory_block(vcb->rootStart);
    if (!current) {
        free(copy);
        errno = EIO;
        return -1;
    }
    int currentLBA = vcb->rootStart;

    char *saveptr = NULL;
    char *token   = strtok_r(copy, "/", &saveptr);

    while (token) {
        int idx = find_entry_in_dir(current, token);
        if (idx < 0 || !current[idx].isDirectory) {
            free(copy);
            free(current);
            errno = ENOENT;
            return -1;
        }

        int childLBA = current[idx].location;
        DE *child = load_directory_block(childLBA);
        if (!child) {
            free(copy);
            free(current);
            errno = EIO;
            return -1;
        }

        free(current);
        current   = child;
        currentLBA = childLBA;

        token = strtok_r(NULL, "/", &saveptr);
    }

    free(copy);
    *dirOut = current;
    *lbaOut = currentLBA;
    return 0;
}


static int split_path_parent_leaf(const char *pathname,
                                  char **parentPathOut,
                                  char *leaf, int leafCap)
{
    if (!pathname || pathname[0] != '/') {
        errno = EINVAL;
        return -1;
    }

    if (strcmp(pathname, "/") == 0) {
        errno = EINVAL;  // cannot mkdir on "/"
        return -1;
    }

    const char *slash = strrchr(pathname, '/');
    if (!slash) {
        errno = EINVAL;
        return -1;
    }

    const char *leafName = slash + 1;
    if (leafName[0] == '\0') {
        errno = EINVAL;  // trailing slash "/a/b/"
        return -1;
    }

    // Copy leaf
    strncpy(leaf, leafName, leafCap - 1);
    leaf[leafCap - 1] = '\0';

    // Parent path
    char *parentPath;
    if (slash == pathname) {
        // parent is root "/"
        parentPath = strdup("/");
    } else {
        size_t parentLen = (size_t)(slash - pathname);
        parentPath = (char *)malloc(parentLen + 1);
        if (!parentPath) {
            errno = ENOMEM;
            return -1;
        }
        memcpy(parentPath, pathname, parentLen);
        parentPath[parentLen] = '\0';
    }

    *parentPathOut = parentPath;
    return 0;
}

/* ============================================================
   fs_mkdir
============================================================ */
int fs_mkdir(const char *pathname, mode_t mode)
{
    (void)mode;  

    if (!pathname || pathname[0] == '\0') {
        errno = EINVAL;
        return -1;
    }

    char  leaf[MAXFILENAME];
    char *parentPath = NULL;

    if (split_path_parent_leaf(pathname, &parentPath, leaf, MAXFILENAME) < 0) {
        return -1; // errno set in helper
    }

    DE *parentDir = NULL;
    int parentLBA = 0;

    if (load_dir_by_path(parentPath, &parentDir, &parentLBA) < 0) {
        free(parentPath);
        return -1; // errno set
    }
    free(parentPath);

    // Check if leaf already exists in parent
    if (find_entry_in_dir(parentDir, leaf) >= 0) {
        free(parentDir);
        errno = EEXIST;
        return -1;
    }

    int slot = find_free_slot(parentDir);
    if (slot < 0) {
        free(parentDir);
        errno = ENOSPC;
        return -1;
    }

    // Allocate a new block for child directory
    int newDirLBA = allocFreeBlock();
    if (newDirLBA < 0) {
        free(parentDir);
        errno = ENOSPC;
        return -1;
    }

    DE *childDir = (DE *)calloc(1, vcb->sizeOfBlock);
    if (!childDir) {
        free(parentDir);
        errno = ENOMEM;
        return -1;
    }

    int    entries = entries_per_block();
    time_t now     = time(NULL);

    // "." entry
    strncpy(childDir[0].name, ".", MAXFILENAME);
    childDir[0].inUse        = 1;
    childDir[0].isDirectory  = 1;
    childDir[0].location     = newDirLBA;
    childDir[0].size         = entries;
    childDir[0].startingBlock= newDirLBA;
    childDir[0].numberOfBlocks = 1;
    childDir[0].sizeInBytes  = (int)vcb->sizeOfBlock;
    childDir[0].created      = now;
    childDir[0].lastModified = now;
    childDir[0].accessed     = now;

    // ".." entry
    strncpy(childDir[1].name, "..", MAXFILENAME);
    childDir[1].inUse        = 1;
    childDir[1].isDirectory  = 1;
    childDir[1].location     = parentLBA;
    childDir[1].size         = entries;
    childDir[1].startingBlock= parentLBA;
    childDir[1].numberOfBlocks = 1;
    childDir[1].sizeInBytes  = (int)vcb->sizeOfBlock;
    childDir[1].created      = now;
    childDir[1].lastModified = now;
    childDir[1].accessed     = now;

    if (LBAwrite(childDir, 1, newDirLBA) != 1) {
        free(childDir);
        free(parentDir);
        errno = EIO;
        return -1;
    }
    free(childDir);

    // Insert new directory entry into parent
    DE *e = &parentDir[slot];
    strncpy(e->name, leaf, MAXFILENAME);
    e->inUse         = 1;
    e->isDirectory   = 1;
    e->location      = newDirLBA;
    e->size          = entries;
    e->startingBlock = newDirLBA;
    e->numberOfBlocks = 1;
    e->sizeInBytes   = (int)vcb->sizeOfBlock;
    e->created       = now;
    e->lastModified  = now;
    e->accessed      = now;

    if (LBAwrite(parentDir, 1, parentLBA) != 1) {
        free(parentDir);
        errno = EIO;
        return -1;
    }

    free(parentDir);
    return 0;
}

/* ============================================================
   fs_opendir
============================================================ */
fdDir * fs_opendir(const char *pathname)
{
    if (!pathname || pathname[0] == '\0') {
        errno = EINVAL;
        return NULL;
    }

    DE *dirBuf = NULL;
    int dirLBA = 0;

    // Support opening root "/"
    if (strcmp(pathname, "/") == 0) {
        dirBuf = load_directory_block(vcb->rootStart);
        if (!dirBuf) {
            errno = EIO;
            return NULL;
        }
        dirLBA = vcb->rootStart;
    } else {
        if (load_dir_by_path(pathname, &dirBuf, &dirLBA) < 0) {
            // errno set by helper
            return NULL;
        }
    }

    fdDir *dd = (fdDir *)calloc(1, sizeof(fdDir));
    if (!dd) {
        free(dirBuf);
        errno = ENOMEM;
        return NULL;
    }

    dd->d_reclen         = 0;
    dd->dirEntryPosition = 0;
    dd->di = (struct fs_diriteminfo *)calloc(1, sizeof(struct fs_diriteminfo));
    if (!dd->di) {
        free(dd);
        free(dirBuf);
        errno = ENOMEM;
        return NULL;
    }

    if (dir_register(dd, dirBuf, dirLBA) < 0) {
        free(dd->di);
        free(dd);
        free(dirBuf);
        // errno already set in dir_register
        return NULL;
    }

    return dd;
}

/* ============================================================
   fs_readdir
============================================================ */
struct fs_diriteminfo *fs_readdir(fdDir *dirp)
{
    if (!dirp || !dirp->di) {
        errno = EINVAL;
        return NULL;
    }

    DE *table = dir_get_buffer(dirp);
    if (!table) {
        errno = EINVAL;
        return NULL;
    }

    int n = entries_per_block();
    while (dirp->dirEntryPosition < n) {
        DE *e = &table[dirp->dirEntryPosition++];

        if (!e->inUse)
            continue;

        struct fs_diriteminfo *di = dirp->di;
        di->fileType = e->isDirectory ? FT_DIRECTORY : FT_REGFILE;

        // d_name size is defined in mfs.h; use sizeof for safety
        size_t nameCap = sizeof(di->d_name);
        strncpy(di->d_name, e->name, nameCap - 1);
        di->d_name[nameCap - 1] = '\0';

        return di;
    }

    // no more entries
    return NULL;
}

/* ============================================================
   fs_closedir
============================================================ */
int fs_closedir(fdDir *dirp)
{
    if (!dirp) {
        errno = EINVAL;
        return -1;
    }

    DE *buf = dir_get_buffer(dirp);
    if (buf) {
        free(buf);
    }
    dir_unregister(dirp);

    if (dirp->di)
        free(dirp->di);
    free(dirp);

    return 0;
}