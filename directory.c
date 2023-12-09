// directory functions

#include "directory.h"

int directory_init(inode_t parent) {
  int inum = alloc_inode();
  inode_t* dir_node = get_inode(inum);
}
