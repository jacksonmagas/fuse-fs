// directory functions

#include "directory.h"

int directory_init(inode_t parent) {
  int inum = alloc_inode();
  inode_t* dir_node = get_inode(inum);
}

// Get the inum of the file or directory with the given name in the given inode
int directory_lookup(inode_t *di, const char *name) {
  dirent *dirs = blocks_get_block(di->block[0]);
  
  for (int i = 0; i < di->size / sizeof(dirent); i++) {
    if (strncmp(name, dirs->name)) {
      return dirs->inum;
    }
    dirs += sizeof(dirent)
  }

  return -1;
}

// Delete the directory in the given inode with the given name
int directory_delete(inode_t *di, const char *name) {
  
}

// Get a linked list of the directories on the path
slist_t *directory_list(const char *path) {
  return s_explode(path, "/");
}
