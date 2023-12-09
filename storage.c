//Implementation of disc storage, including creation of bitmaps and inode table

#include "storage.h"
#include "inode.h"
#include "helpers/block.h"

// Initialize the storage for the file system
void storage_init(const char *path) {
  // Initialize the data blocks
  blocks_init(path);   

  // Permanently set aside 3 data blocks for inodes
  for (int i = 0; i < NUM_INODE_BLOCKS; i++) {
    int perm_block = alloc_block();
  }
}

// Get the file information for the file at the specified path
int storage_stat(const char *path, struct stat *st) {
  int path_inum = 0; // lookup(path) method to get inum from path
  if (path_inum > 0) {
    inode *path_inode = get_inode(path_inum);
    st->st_ino = path_inum;
    st->st_mode = path_inode->mode;
    st->st_nlink = path_inode->refs;
    st->st_size = path_inode->size;
    return 0;  
  }

  return -1;
}

// Read the specified number of bytes from the given offset in the file at path to the buffer
int storage_read(const char *path, char *buf, size_t n, size_t size, off_t offset) {
  int path_inum = 0; // lookup(path) method to get inum from path
  return inode_read(path_inum, buf, n, size, offset);
}

// Write the specified number of bytes from the given offset in the file at path to the buffer
int storage_write(const char *path, const char *buf, size_t n, off_t offset) {
  int path_inum = 0; // lookup(path) method to get inum from path
  return inode_write(path_inum, buf, n, size, offset);
}

// truncate the file at the given path to the given size
int storage_truncate(const char *path, off_t size) {
  int path_inum = 0; // lookup(path) method to get inum from path
  if (path_inum > 0) {
    inode *path_inode = get_inode(path_inum);

    if (path_inode->size > size){
	shrink_inode(path_inode, size);
    }

    return 0;
  }

  return -1;
}

// Takes a path and isolates the filename from the full directory path
void iso_filename(const char *path, char *dir, char *filename) {
    slist *split_path = s_split(path, '/');

    while (split_path->next != NULL) {
        char *add_string = strncat("/", split_path->data, 128);
        strncat(dir, add_string, 129);
        split_path = split_path->next;
    }

    char *path_rest = split_path->data;
    memcpy(filename, path_rest, strnlen(path_rest));

    s_free(split_path);
}

// Make a new file system object (file or directory) at the given path
int storage_mknod(const char *path, int mode) {
  if (path_inum == -1) {
    char *dir = malloc(strnlen(path));
    char *filename = malloc(128);
    iso_filename(path, dir, filename);

    dir_inum = 0; // lookup(dir) method to get inum from path
    if (dir_inum < 0) {
      free(dir);
      free(filename);   
      return -1;
    } else {
      inode* dir_inode = get_inode(dir_inum);
    }

    int new_inum = alloc_inode();
    inode* new_inode = get_inode(new_inode);
    node->refs = 1;
    new_inode->mode = mode;
    directory_put(parent_dir, item, new_inode);
    
    free(dir);
    free(filename);
    return ret;
  }

  return -1;
}

// Remove the file or directory at the given path
int storage_unlink(const char *path) {
  int path_inum = 0; // lookup(path) method to get inum from path
  if (path_inum > 0) {
    char* dir = malloc(strnlen(path);
    char* filename = malloc(strnlen(128));
    iso_filename(dir, filename);

    int dir_inum = 0; // lookup(parent_path)
    inode *dir_inode = get_inode(dir_inum);
    int ret = directory_delete(dir_inode, filename);

    free(dir);
    free(filename);
    return ret;
  }

  return -1;
}

// Create a new hard link from the source path to the destination path
int storage_link(const char *from, const char *to) {
  int from_inum = 0; // lookup(from) method to get inum from path
  int to_inum = 0; // lookup(to) method to get inum from path
  if (from_inum > 0 & to_inum > 0) {
    char* dir = malloc(strnlen(from);
    char* filename = malloc(strnlen(128));
    iso_filename(dir, filename);

    int dir_inum = 0 // lookup(dir)
    inode *dir_inode = get_inode(dir_inum);
    int ret = directory_put(dir_inode, filename, inum);

    free(dir);
    free(filename);
    return ret;
  }

  return -1;
}

// Rename the file or directory at the given path to the new path
int storage_rename(const char *from, const char *to) {
  int path_inum = 0; // lookup(path) method to get inum from path
  if (path_inum > 0) {
    storage_link(from, to);
    storage_unlink(from);
    
    return 0;
  }

  return -1;
}

// Set the access and modification times for the specified path
int storage_set_time(const char *path, const struct timespec ts[2]) {
  int path_inum = 0; // lookup(path) method to get inum from path
  if (path_inum > 0) {
    inode *path_inode = get_inode(inum);
    time_t new_time = ts->tv_sec;
    inode->time = new_time;
    
    return 0;
  }

  return -1;
}

// Get a list of the contents of the directory at the given path
slist_t *storage_list(const char *path) {
  return directory_list(path);
}
