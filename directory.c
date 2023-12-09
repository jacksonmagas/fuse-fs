// directory functions

#include "directory.h"

int directory_init(inum parent) {
  int inum = alloc_inode(40755);
  inode_t* dir_node = get_inode(inum);
  dirent_t self;
  dirent_t parent_dir;
  self.name = ".";
  parent_dir.name = "..";
  self.inum = inum;
  parent_dir.inum = parent;
  inode_write(inum, (char*) &self, sizeof(dirent_t), dir_node->size);
  inode_write(inum, (char*) &parent, sizeof(dirent_t), dir_node->size);
  return inum;
}

int directory_put(inum di, const char *name, int mode) {
  int inum;
  if (mode / 40000 == 1) {
    // if it is a directory
    inum = directory_init(di);
    get_inode(inum)->mode = mode;
  } else {
    inum = alloc_inode(mode);
  }
  dirent_t entry;
  entry.name = name;
  entry.inum = inum;
  inode_write(entry, (char*) &self, sizeof(dirent_t), get_inode(di)->size);
}
