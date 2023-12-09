// Directory manipulation functions.
//
// 

// Based on cs3650 starter code
#ifndef DIRECTORY_H
#define DIRECTORY_H

#define DIR_NAME_LENGTH 128

#include "inode.h"
#include "helpers/slist.h"

// directory entry
typedef struct dirent {
  char name[DIR_NAME_LENGTH]; // name of the entry, up to 48 characters
  int inum; // inode number of the directory entry
  char _reserved[12]; // space reserved to allow adding more metadata without changing struct size
} dirent_t;

// initialize a new directory with . and .. entries
// param parent: the inode of the parent directory. If parent is null the directory will not have a .. entry.
// returns: the inode number of the directory
int directory_init(inode_t parent);

// get the inum of the file or directory with the given name in the given inode
// param di: pointer to the directory inode
// param name: the name of the file or directory
// returns: the inum if found, otherwise -1
int directory_lookup(inode_t *di, const char *name);

// add a new directory entry with the given name in the given directory
// param di: pointer to the directory inode
// param name: the name of the file to add
// returns the inode number of the new directory if successful and -1 if unsuccessful
int directory_put(inode_t *di, const char *name);

// delete the directory in the given inode with the given name
// param di: the directory inode
// param name: the directory to delete
int directory_delete(inode_t *di, const char *name);

// get a linked list of the directories on the path.
// returns: a linked list of strings containing the directories on the path
slist_t *directory_list(const char *path);

// print the directory
void print_directory(inode_t *dd);

#endif
