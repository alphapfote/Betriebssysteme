#ifndef root_h
#define root_h

#include <cassert>
#include "inodes.h"

class Root {

Root();
~Root();

public:

int getFreeRootEntry();
Inode* getInodeByName(char* name);
void writeInode(int rootIndex, Inode* inode);
int writeDataInDMap(int blocksNecessary);
Inode* getInodeByIndex(int rootIndex);
void readInodeFromBlockdevice(int rootIndex, char *buffer);

};

#endif //root_h