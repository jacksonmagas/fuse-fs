// Inode manipulation functions

#include "inode.h"
#include <stdio.h>

void print_inode(inode_t *node) {
  printf("Inode %p: number of references = %d, mode = %d, size = %d, block numbes = %d",
         node, node->refs, node->mode, node->size, node->block);
}

inode_t *get_inode(int inum) {
  int block_num = 1 + inum / (BLOCK_SIZE / sizeof(inode_t));
  int inum_in_block =  inum % (BLOCK_SIZE / sizeof(inode_t));
  assert(block_num > 0 && block_num <= NUM_INODE_BLOCKS);i
  // get the block of the inode and then get the inode in the block
  return ((inode_t*) blocks_get_block(block_num))[inum_in_block];
}

int alloc_inode() {
  int inum = first_free_inode();
  inode_t new_node = get_inode(inum);
  new_node.block[0] = alloc_block();
  return inum;
}

// returns in inum of the first free inode, and -1 if there are no free inodes
int first_free_inode() {
  void* inode_bitmap = get_inode_bitmap;
  int inum = -1;
  // loop over each inode in the bitmap
  for (int i = 0; i < (BLOCK_SIZE / sizeof(inode_t)) * NUM_INODE_BLOCKS; i++) {
    if (!bitmap_get(inode_bitmap, i)) {
      inum = i;
    }
  }
}

// decrease reference count
void free_inode(int inum) {
  inode_t node = get_inode(inum);
  if (node.refs > 1) {
    nodes.refs--;
    return;
  } else {
    node.refs = 0;
    node.mode = 0;
    node.size = 0;
    node.indirect_block = -1; 
    for (int i = 0; i < NUM_DIRECT_BLOCKS; i++) {
      free_block(new_node.block[i]);
      new_node.block[i] = -1;
    }
  }
}

int grow_inode(inode_t *node, int size) {
  int space_remaining = (node->num_blocks * BLOCK_SIZE) - node->size; 
  if (space_remaining >= size) {
    node->size += size;
  } else {
    int next_block = alloc_block()
    if (next_block < 0) {
      return -1;
    }
    if (node->num_blocks < NUM_DIRECT_BLOCKS) {
      // allocate normal block
      node->size += size;
      node->num_blocks++;
      node->block[num_blocks] = next_block;
    } else {
      if (node->num_blocks == NUM_DIRECT_BLOCKS) {
        // allocate indirect block with all blocks as -1
        node->indirect_block = next_block;
        next_block = alloc_block();
        for (int i = 0; i < BLOCK_SIZE; i++) {
          ((int*) blocks_get_block(node->indirect_block))[i] = -1;
        }
        if (next_block < 0) {
          return -1;
        }
      }
      // allocate new block in indirect block
      ((int*) blocks_get_block(node->indirect_block))[num_blocks - NUM_DIRECT_BLOCKS] = next_block;
      node->num_blocks++;
      node->size += size;
    }
  }
  return 0;
}

int shrink_inode(inode_t *node, int size) {
  if (node->num_blocks == 1 && node->size < size) {
    return -1;
  }
  int space_remaining = (node->num_blocks * BLOCK_SIZE) - node->size; 
  if (BLOCK_SIZE - space_remaining >= size) {
    node->size -= size;
  } else {
    if (node->num_blocks <= NUM_DIRECT_BLOCKS) {
      // deallocate normal block
      node->size -= size;
      free_block(node->block[num_blocks]);
      node->block[num_blocks] = -1;
      node->num_blocks--;
    } else {
      // deallocate new block in indirect block
      int cur_block = ((int*) blocks_get_block(node->indirect_block))[num_blocks - NUM_DIRECT_BLOCKS];
      free_block(cur_block);
      cur_block = -1;
      if (node->num_blocks == (NUM_DIRECT_BLOCKS + 1)) {
        // deallocate indirect block
        free_block(node->indirect_block);
        node->indirect_block = next_block;
      }
      node->num_blocks++;
      node->size -= size;
    }
  }
  return 0;
}

int inode_get_bnum(inode_t *node, int file_bnum) {
  if (file_bnum >= num_blocks) {
    return -1;
  }
  if (file_bnum <= NUM_DIRECT_BLOCKS) {
    return block[file_bnum];
  } else {
    return ((int*) blocks_get_block(node->indirect_block))[file_bnum - NUM_DIRECT_BLOCKS];
  }
}
