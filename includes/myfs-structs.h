//
//  myfs-structs.h
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#ifndef myfs_structs_h
#define myfs_structs_h


// TODO: Add structures of your file system
#define NAME_LENGTH 255
#define BLOCK_SIZE 512
#define NUM_DIR_ENTRIES 64
#define NUM_OPEN_FILES 64
#define ROOT_INDEX 42


struct SBlock {
    //wo fängt was an: zeiger an die orte im filesystem
    int firstDMapBlock; //char *firstDMapBlock
    int firstInodeBlock;
    int firstFATBlock;
    int firstDataBlock;
};



#endif /* myfs_structs_h */
