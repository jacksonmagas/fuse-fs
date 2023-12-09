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
// param parent: the inode of the parent directory. If parent is -1 the directory be root
// returns: the inode number of the directory
int directory_init(int parent);

// get the inum of the file or directory with the given name in the given inode
// param di: pointer to the directory inode
// param name: the name of the file or directory
// returns: the inum if found, otherwise -1
int directory_lookup(inode_t *di, const char *name);

// add a new directory entry with the given name in the given directory
// param di: inode number of the directory
// param name: the name of the file to add
// param name: the mode of the new directory object, either directory or file and perms
// returns the inode number of the new directory if successful and -1 if unsuccessful
int directory_put(int di, const char *name, int mode);

// add a new hard link with the given name to the given inode
// param di: the directory to put the link in
// param name: the name of the hard link
// param target: the inode number to link to
// returns: the inode number of the target if it exists, otherwise -1
int directory_link(int di, const char *name, int target);

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
