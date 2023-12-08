Implementation of disc storage, including creation of bitmaps and inode table

#include "storage.h"
#include "inode.h"


// initialize the file system at the given path.
// create the inode bitmap, inode table, and file bitmap
void storage_init(const char *path) {
  blocks_init(path);   
  // allocate 3 
  allocate_inode_blocks(3);
}
