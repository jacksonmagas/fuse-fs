// directory functions

#include "directory.h"
#include <stdio.h>

int directory_init(inum parent) {
  int inum = alloc_inode(40755);
  directory_link(inum, ".", inum);
  // for non-root directories
  if (parent >= 0) {
    directoy_link(inum, "..", parent);
  }
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

// directory inodes store the bits
int directory_link(inum di, const char* name, inum target) {
  void* bmap = get_inode_bitmap();
  if (bitmap_get(target)) {
    // get pointer to new directory entry
    dirent_t* entry = (dirent_t*) malloc(sizeof(dirent_t));
    entry->name = name;
    entry->inum = target;
    inode_write(di, (char*) entry, sizeof(dirent_t), get_inode(di)->size);
    free(entry);
  }
  return -1;
}

// Get the inum of the file or directory with the given name in the given inode
int directory_lookup(inode_t *di, const char *name) {
  dirent_t *dirs = blocks_get_block(di->block[0]);
  
  for (int i = 0; i < di->size / sizeof(dirent_t); i++) {
    if (strncmp(name, dirs[i].name) == 0) {
      return dirs[i].inum;
    }
  }

  return -1;
}

// Delete the directory in the given inode with the given name
int directory_delete(inode_t *di, const char *name) {
  dirent_t* dirs = (dirent_t*) malloc(sizeof(dirent_t));
  for (int i = 0; i < di->size; i+= sizeof(dirent_t)) {
    inode_read(di, (char*) dirs, sizeof(dirent_t), i);
    // 
    if (strncmp(name, dirs->name) == 0) {
      free_inode(dirs[i]->inum);
      // move the following entries forwards by one spoti
      int remaining_space = di->size - (i * sizeof(dirent_t));
      char* following_entries = malloc(remaining_space);
      inode_read(di, (char*) following_entries, remaining_space, remaining_space, i + sizeof(diernt_t));
      inode_write(di, (char*) following_entries, remaining_space, remaining_space, i);
      shrink_inode(di, sizeof(dirent_t);
      free(following_entries);
      free(dirs);
      return 0;
    }
  }
  free(dirs);
  return -1; 
}

// Get a linked list of the directories on the path
slist_t *directory_list(const char *path) {
  return s_explode(path, "/");
}

// print the directory element names with 2 spaces between them 
void print_directory(inode_t *dd) {
  int size = dd->size;
  char* entry = (char*) malloc(sizeof(dirent_t));
  for (int i = 0; i < size; i += sizeof(dirent_t)) {
    inode_read(dd, entry, sizeof(dirent_t), sizeof(dirent_t), i);
    printf("%s  ", ((dirent_t*)entry->name));
  }
  free(entry);
}
