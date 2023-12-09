//Implementation of disc storage, including creation of bitmaps and inode_t table

#include "storage.h"
#include <string.h>
#include "inode.h"
#include "directory.h"
#include "blocks.h"

// Initialize the storage for the file system
void storage_init(const char *path) {
  printf("initialize storage with %s as data file", path);
  // Initialize the data blocks
  blocks_init(path);   

  // Permanently set aside blocks 2, 3, 4 as inode table blocks
  for (int i = 0; i < NUM_INODE_BLOCKS; i++) {
    alloc_block();
  }
  printf("allocate root");
  // allocate inode_t for root by giving it a non-existant parent
  directory_init(-1);
}


// Get the inum at a given path
int get_inum(const char *path) {
  printf("get_inum of %s", path);
  if (strncmp(path, "/", 4) == 0) {
    printf(" = 0 because path is /");
    return 0;
  }
  // start from root directory which is inum 0
  int inum = 0;
  slist_t *split_path = directory_list(path);
  slist_t* whole_list = split_path;
  
  while (split_path != NULL) {
    printf("looking for %s in inode %d -> ", split_path->data, inum);
    inum = directory_lookup(inum, split_path->data);
    if (inum < 0) {
      printf(" = %d. %s not found or parent not a dir", -1, split_path->data);
      s_free(whole_list);
      return -1;
    }
    split_path = split_path->next;
  }

  s_free(whole_list);
  return inum;
}

// Get the file information for the file at the specified path
int storage_stat(const char *path, struct stat *st) {
  printf("storage_stat -> ");
  int path_inum = get_inum(path);
  if (path_inum >= 0) {
    inode_t *path_inode = get_inode(path_inum);
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
  int path_inum = get_inum(path);
  return inode_read(path_inum, buf, n, size, offset);
}

// Write the specified number of bytes from the given offset in the file at path to the buffer
int storage_write(const char *path, const char *buf, size_t n, off_t offset) {
  int path_inum = get_inum(path);
  return inode_write(path_inum, buf, n, offset);
}

// truncate the file at the given path by the given offset
int storage_truncate(const char *path, off_t size) {
  int path_inum = get_inum(path);
  if (path_inum >= 0) {
    inode_t *path_inode = get_inode(path_inum);

    if (path_inode->size > size){
	shrink_inode(path_inode, size);
    }

    return 0;
  }

  return -1;
}

// Takes a path and isolates the filename from the full directory path
// param path: the path to split
// param dir_path: output buffer for parent directory path
// param filename: the output buffer for file name
void iso_filename(const char *path, char *dir_path, char *filename) {
    slist_t *split_path = directory_list(path);

    while (split_path->next != NULL) {
        char *add_string = strncat("/", split_path->data, 128);
        strncat(dir_path, add_string, 129);
        split_path = split_path->next;
    }

    char *path_rest = split_path->data;
    memcpy(filename, path_rest, strnlen(path_rest, 256));

    s_free(split_path);
}

// Make a new file system object (file or directory) at the given path
int storage_mknod(const char *path, int mode) {
  char* parent_path = malloc(sizeof(path));
  char* filename = malloc(sizeof(path));
  iso_filename(path, parent_path, filename);
  int parent = get_inum(parent_path); //lookup parent inum on path
  int result = directory_put(parent, filename, mode);
  free(parent_path);
  free(filename);
  return result;
}

// Remove the file or directory at the given path
int storage_unlink(const char *path) {
  int path_inum = get_inum(path);
  if (path_inum >= 0) {
    char* dir = malloc(strnlen(path, 256));
    char* filename = malloc(strnlen(path, 256));
    iso_filename(path, dir, filename);

    int dir_inum = get_inum(dir);
    int ret = directory_delete(dir_inum, filename);

    free(dir);
    free(filename);
    return ret;
  }

  return -1;
}

// Create a new hard link from the source path to the destination path
int storage_link(const char *from, const char *to) {
  int to_inum = get_inum(to);
  // no need to support linking inode 0 because you shouldn't be linking root to something else
  if (to_inum > 0) {

    char* dir = malloc(strnlen(from, 128));
    char* filename = malloc(128);
    iso_filename(from, dir, filename);

    int dir_inum = get_inum(dir);
    int ret = directory_link(dir_inum, filename, to_inum);

    free(dir);
    free(filename);
    return ret;
  }

  return -1;
}

// Rename the file or directory at the given path to the new path
int storage_rename(const char *from, const char *to) {
  int from_inum = get_inum(from);
  if (from_inum > 0) {
    storage_link(from, to);
    storage_unlink(from);
    
    return 0;
  }

  return -1;
}

// Set the access and modification times for the specified path
int storage_set_time(const char *path, const struct timespec ts[2]) {
  int path_inum = get_inum(path);
  if (path_inum > 0) {
    inode_t *path_inode = get_inode(path_inum);
    path_inode->access_time = ts[0];
    path_inode->modification_time = ts[1];
    
    return 0;
  }

  return -1;
}

// Get a list of the contents of the directory at the given path
slist_t *storage_list(const char *path) {
  return directory_list(path);
}
