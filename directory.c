// directory functions

#include "directory.h"

int directory_init(inum parent) {
  int inum = alloc_inode(40755);
  directory_link(inum, ".", inum);
  directoy_link(inum, "..", parent);
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
  return directory_link(di, name, inum);
}

int directory_link(inum di, const char* name, inum target) {
  void* bmap = get_inode_bitmap();
  if (bitmap_get(target)) {
    dirent_t *entry = malloc(sizeof(dirent_t));
    entry->name = name;
    entry->inum = target;
    inode_write(di, (char*) entry, sizeof(dirent_t), get_inode(di)->size);
    free(entry);
  }
  return -1;
}
