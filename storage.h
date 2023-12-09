// Disk storage abstracttion.
//
// Feel free to use as inspiration. Provided as-is.

// based on cs3650 starter code

#ifndef NUFS_STORAGE_H
#define NUFS_STORAGE_H

#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "slist.h"

// initialize the file system at the given file path
// param path: the file path as a string
void storage_init(const char *path);

// get the inode number for the given path
// param: path: the file path to get inode number for
// returns: the inode number or -1 if the file doesn't exist
int get_inum(const char *path);

// get the file information for the file at the specified path
// param path: the file path as a string
// param st: a pointer to a stat struct to store the result in
// Returns: 0 if the file is found otherwise -1
int storage_stat(const char *path, struct stat *st);

// read the specified number of bytes from the given offset in the file at path to the buffer.
// param path: the file path to read from as a string
// param buf: the char array to read data into
// param n: the number of bytes to read
// param size: the size of the buffer to prevent overflow
// param offset: the number of bytes from the start of the file to read from
// returns: number of bytes read if the read was successful, -1 if the specified file and offset couldn't be accessed
int storage_read(const char *path, char *buf, size_t n, size_t size, off_t offset);


// write the specified number of bytes from the given offset in the file at path to the buffer.
// param path: the file path to write to as a string
// param buf: the char array to write data from
// param n: the number of bytes to write
// param offset: the number of bytes from the start of the file to write from
// returns: number of bytes written if the write was successful, -1 if the specified file and offset couldn't be accessed
int storage_write(const char *path, const char *buf, size_t n, off_t offset);

// truncate the file at the given path to the given size
// param path: the file path to truncate as a string
// param size: the size to truncate the file to
// returns: 0 if successful and -1 if unsuccessful
int storage_truncate(const char *path, off_t size);

// make a new file system object (file or directory) at the given path
// param path: the file path to create a new file or directory at
// param mode: the octal representation of the mode for the new object
// 	       a standard file will be 100644 while a standard directory will be 40755
// returns: 0 if successful, -1 otherwise
int storage_mknod(const char *path, int mode);

// remove the file or directory at the given path
// param path: the file path to remove
// returns: 0 if successful, -1 otherwise
int storage_unlink(const char *path);

// create a new hard link from the source path to the destination path, i.e. making the source path point to the inode of the destination path
// param from: the file path to turn into a hard link
// param to: the file path to link to
// returns: 0 if successful, -1 otherwise
int storage_link(const char *from, const char *to);

// rename the file or directory at the given path to the new path
// param from: the file path to rename
// param to: the file path to rename to
// returns: 0 if successful, -1 otherwise
int storage_rename(const char *from, const char *to);

// set the access and modification times for the specified path
// param path: the file to update access or modification times for
// param ts: an array of two timespec structs representing access time and modification time
// returns: 0 if successful, -1 otherwise
int storage_set_time(const char *path, const struct timespec ts[2]);

// get a list of the contents of the directory at the given path
// param path: the directory t list contents of
// returns: an slist containing the names of files and subdirectories in the directory
slist_t *storage_list(const char *path);

#endif
