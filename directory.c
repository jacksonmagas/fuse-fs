// directory functions

#include <errno.h>
#include "directory.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int directory_init(int parent) {
  printf("allocate inode for new directory with parent %d\n", -1);
  int inum = alloc_inode(040755);
  directory_link(inum, ".", inum);
  // decrement the reference counter to compensate for the extra reference of .
  get_inode(inum)->refs--;
  // for non-root directories
  if (parent >= 0) {
    directory_link(inum, "..", parent);
  }
  return inum;
}

int directory_put(int di, const char *name, int mode) {
  int inum;
  if (mode & 040000) {
    // if it is a directory
    inum = directory_init(di);
    get_inode(inum)->mode = mode;
  } else {
    inum = alloc_inode(mode);
  }
  return directory_link(di, name, inum);
}

// directory inodes store the bits
int directory_link(int di, const char* name, int target) {
  // if there is a directory of the same name return error directory exists
  if (directory_lookup(di, name) >= 0) {
    return -EEXIST;
  }
  printf("link name %s to inode %d in directory %d\n", name, target, di);
  void* bmap = get_inode_bitmap();
  if (bitmap_get(bmap, target)) {
    // get pointer to new directory entry
    dirent_t* entry = (dirent_t*) malloc(sizeof(dirent_t));
    strncpy(entry->name, name, 128);
    entry->inum = target;
    inode_write(di, (char*) entry, sizeof(dirent_t), get_inode(di)->size);
    get_inode(target)->refs++;
    free(entry);
    return target;
  }
  return -ENOENT;
}

// Get the inum of the file or directory with the given name in the given inode
// empty string returns parent inum
int directory_lookup(int dir_inum, const char *name) {
  printf("directory_lookup of %s\n", name);
  inode_t* di = get_inode(dir_inum);
  dirent_t *dirs = blocks_get_block(di->block[0]);
  
  if (strnlen(name, 4) == 0) {
    return dir_inum;
  }

  for (int i = 0; i < di->size / sizeof(dirent_t); i++) {
    if (strncmp(name, dirs[i].name, 128) == 0) {
      return dirs[i].inum;
    }
  }

  return -ENOENT;
}

// Delete the directory in the given inode with the given name
int directory_delete(int di, const char *name) {
  inode_t *dinode = get_inode(di);
  dirent_t* dirs = (dirent_t*) malloc(sizeof(dirent_t));
  for (int i = 0; i < dinode->size; i+= sizeof(dirent_t)) {
    inode_read(di, (char*) dirs, sizeof(dirent_t), sizeof(dirent_t), i);
    // if the directory entry is the one to delete
    if (strncmp(name, dirs->name, 128) == 0) {
      free_inode(dirs->inum);
      // move the following entries forwards by one spoti
      int remaining_space = dinode->size - i;
      char* following_entries = malloc(remaining_space);
      // read from the next dirent into buffer
      inode_read(di, (char*) following_entries, remaining_space, remaining_space, i + sizeof(dirent_t));
      // write the buffer back in with the offset one less
      inode_write(di, (char*) following_entries, remaining_space, i);
      shrink_inode(dinode, sizeof(dirent_t));
      free(following_entries);
      free(dirs);
      return 0;
    }
  }
  free(dirs);
  return -ENOENT; 
}

// Get a linked list of the directories on the path
slist_t *directory_list(const char *path) {
  return s_explode(path, '/');
}

// print the directory element names with 2 spaces between them 
void print_directory(int dd) {
  int size = get_inode(dd)->size;
  char* entry = (char*) malloc(sizeof(dirent_t));
  for (int i = 0; i < size; i += sizeof(dirent_t)) {
    inode_read(dd, entry, sizeof(dirent_t), sizeof(dirent_t), i);
    printf("%s  ", ((dirent_t*)entry)->name);
  }
  free(entry);
}

// fill fuse directory
void directory_readdir(int dir_inum, void* buf, fuse_fill_dir_t filler, off_t offset) {
  inode_t* di = get_inode(dir_inum);
  dirent_t* entry = (dirent_t*) malloc(sizeof(dirent_t));
  struct stat* statbuf = malloc(sizeof(struct stat));
  for (int i = offset; i < di->size / sizeof(dirent_t); i++) {
    inode_read(dir_inum, (char*) entry, sizeof(dirent_t), sizeof(dirent_t), i * sizeof(dirent_t));
    inode_t* node = get_inode(entry->inum);
    statbuf->st_mode = node->mode;
    statbuf->st_size = node->size;
    statbuf->st_nlink = node->refs;
    statbuf->st_atim = node->access_time;
    statbuf->st_mtim = node->modification_time;
    filler(buf, entry->name, statbuf , i + 1);
  }
  free(entry);
  free(statbuf);
}
