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

// Get the inum of the file or directory with the given name in the given inode
int directory_lookup(inode_t *di, const char *name) {
  dirent *dirs = blocks_get_block(di->block[0]);
  
  for (int i = 0; i < di->size / sizeof(dirent); i++) {
    if (strncmp(name, dirs[i].name) == 0) {
      return dirs[i].inum;
    }
  }

  return -1;
}

// Delete the directory in the given inode with the given name
int directory_delete(inode_t *di, const char *name) {
  dirent *dirs = blocks_get_block(di->block[0]);

  for (int i = 0; i < di->size / sizeof(dirent); i++) {
    if (strncmp(name, dirs[i].name) == 0) {
      free_inode(dirs[i].inum);
      memcpy(&dirs[i], &dirs[i + 1], 4096 - (i + 1) * sizeof(dirent));
      return 0;
    }
  }

  return -1; 
}

// Get a linked list of the directories on the path
slist_t *directory_list(const char *path) {
  return s_explode(path, "/");
}
