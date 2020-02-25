#ifndef root_h
#define root_h

#include <cassert>
#include "inode.h"

class Root {

public:
    Root();

    ~Root();


    int getFreeRootEntry(); //Gibt den ersten freien Root Block als Index zurück

    void writeInode(int rootIndex, Inode *inode);

    Inode *getInodeByName(char *name);

    Inode *getInodeByIndex(int rootIndex);

    void readInodeFromBlockdevice(int blockDevIndex, char *buffer);


//TODO: Wahrschl in DMAP schieben int writeDataInDMap(int blocksNecessary);
};

#endif //root_h