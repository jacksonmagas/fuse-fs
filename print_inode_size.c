#include "inode.h"
#include <stdio.h>

int main(int argc, char** argv) {
  printf("The inode size is %ld", sizeof(inode_t));
  return 0;
}
