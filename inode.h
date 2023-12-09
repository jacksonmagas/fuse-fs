// Inode manipulation routines.
//
// Feel free to use as inspiration. Provided as-is.

// based on cs3650 starter code
#ifndef INODE_H
#define INODE_H

#include "blocks.h"
#include <time.h>
#include <stdlib.h>

#define NUM_INODE_BLOCKS 3
#define NUM_DIRECT_BLOCKS 12

typedef struct inode {
  int refs;  // reference count
  int mode;  // permission & type
  int size;  // bytes
  int num_blocks; // number of blocks in use by this inode
  int block[NUM_DIRECT_BLOCKS]; // first 12 block numbers (if max file size <= 4K)
  int indirect_block;
  struct timespec access_time;
  struct timespec modification_time;  

} inode_t;

// print the information in the inode to stdout
// parameter node: pointer to the inode to print 
void print_inode(inode_t *node);

// get a pointer to the inode with the given number. Assumes that the inode is already allocated.
// returns: a pointer to the inode
inode_t *get_inode(int inum);

// allocate a new inode with the given mode
// param mode: the mode_t for file vs directory and perms
// returns: the inode number or -1 if allocation fails
int alloc_inode(int mode);

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
// returns: the block number or -1 if out of range
int inode_get_bnum(inode_t *node, int file_bnum);

// read up to n bytes into a buffer of the given size, starting from offset in the given inode
// param inum: the inode number to read from
// param buf: the char buffer to read into
// param n: the number of bytes to read
// param size: the size of the buffer to read into
// param offset: the offset of the buffer
// returns: 0 if successful or -1 if unsuccessful;
int inode_read(int inum, char* buf, int n, int size, int offset);

// write up to n bytes into a buffer of the given size, starting from offset in the given inode
// param inum: the inode number to write to
// param buf: the char buffer to write from
// param n: the number of bytes to write
// param offset: the offset of the buffer
// returns: 0 if successful or -1 if unsuccessful;
int inode_write(int inum, const char* buf, int n, int offset);

#endif
