// Inode manipulation routines.
//
// Feel free to use as inspiration. Provided as-is.

// based on cs3650 starter code
#ifndef INODE_H
#define INODE_H

#include "blocks.h"

typedef struct inode {
  int refs;  // reference count
  int mode;  // permission & type
  int size;  // bytes
  int block; // single block pointer (if max file size <= 4K)
} inode_t;

// print the information in the inode to stdout
// parameter node: pointer to the inode to print 
void print_inode(inode_t *node);

// get a pointer to the inode with the given number
// returns: a pointer to the inode
inode_t *get_inode(int inum);

// allocate a new inode
// returns: the inode number
int alloc_inode();

// free the inode with the given number
void free_inode(int inum);

// grow the given inode by the given number of bytes
// parameter node: pointer to the input inode
// parameter size: the number of bytes to increase the inode size by
// returns: 0 if successful, -1 if unsuccessful
int grow_inode(inode_t *node, int size);

// reduce the size of the given inode by the given number of bytes
// parameter node: pointer to the input inode
// parameter size: the number of bytes to increase the inode size by
// returns: 0 if successful, -1 if unsuccessful
int shrink_inode(inode_t *node, int size);

// get the on disc block number of the file block number in the given inode
// parameter node: a pointer to the input inode
// parameter file_bnum: the 0 indexed block number within the file to get, for example reading from the 6000th character with block size of 4K would have file_bnum be 1 
int inode_get_bnum(inode_t *node, int file_bnum);

#endif
