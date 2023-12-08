// Inode manipulation functions

#include "inode.h"
#include <stdio.h>

void print_inode(inode_t *node) {
  printf("Inode %p: number of references = %d, mode = %d, size = %d, block numbes = %d",
         node, node->refs, node->mode, node->size, node->block);
}

inode_t *get_inode(int inum) {
  // TODO, requires inode table
}
