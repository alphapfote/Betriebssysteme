//
//  myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 02.08.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include <iostream>
#include <cstring>
#include <cmath>

#include "myfs.h"
#include "myfs-info.h"

MyFS *MyFS::_instance = NULL;

#define RETURN_ERRNO(x) (x) == 0 ? 0 : -errno

#define LOGF(fmt, ...) \
do { fprintf(this->logFile, fmt "\n", __VA_ARGS__); } while (0)

#define LOG(text) \
do { fprintf(this->logFile, text "\n"); } while (0)

#define LOGM() \
do { fprintf(this->logFile, "%s:%d:%s()\n", __FILE__, \
__LINE__, __func__); } while (0)

MyFS *MyFS::Instance() {
    if (_instance == NULL) {
        _instance = new MyFS();
    }
    return _instance;
}

MyFS::MyFS() {
    this->logFile = stderr;
}

MyFS::~MyFS() {

}

int MyFS::fuseGetattr(const char *path, struct stat *statbuf) {
    LOGM();

    // Unser Zeug
    LOGF("\tAttributes of %s requested\n", path);

    statbuf->st_uid = getuid(); // The owner of the file/directory is the user who mounted the filesystem
    statbuf->st_gid = getgid(); // The group of the file/directory is the same as the group of the user who mounted the filesystem
    statbuf->st_atime = time(NULL); // The last "a"ccess of the file/directory is right now
    statbuf->st_mtime = time(NULL); // The last "m"odification of the file/directory is right now

    // Wenn es das RootVZ ist braucht man nicht tiefer gehen
    if (strcmp(path, "/") == 0) {
        statbuf->st_mode = S_IFDIR | 0755;
        statbuf->st_nlink = 2; // Why "two" hardlinks instead of "one"? The answer is here: http://unix.stackexchange.com/a/101536
    } else {// Wenn der String nicht das RootVZ ist
        Inode *inode = root.getInodeByName(getFileNameFromPath(path));
        if (inode == NULL) {
            LOG("Es wurde keine Datei mit dem Namen gefunden");
            RETURN(ENOENT);  //Keine Datei mit dem Namen gefunden
        } else {
            // Aus der erhaltenen Inode werden die Attribute gelesen und in den Puffer geschrieben
            statbuf->st_atime = inode->atime;
            statbuf->st_mtime = inode->mtime;
            statbuf->st_ctime = inode->ctime;
            statbuf->st_mode = S_IFREG | 0644;
            statbuf->st_nlink = 1;
            statbuf->st_size = inode->size;
        }
    }
    //Ende unser Zeug

    return 0;
}

int MyFS::fuseReadlink(const char *path, char *link, size_t size) {
    LOGM();

    return 0;
}

int MyFS::fuseMknod(const char *path, mode_t mode, dev_t dev) {
    LOGM();

    //Unser Zeug

    //Ende unser Zeug

    return 0;
}

int MyFS::fuseMkdir(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

int MyFS::fuseUnlink(const char *path) {
    LOGM();

    //Unser Zeug
    /*
    void free(void path);
    fat.deleteFileInFat();//parameter:index
    */

    //Ende unser Zeug

    return 0;
}

int MyFS::fuseRmdir(const char *path) {
    LOGM();
    return 0;
}

int MyFS::fuseSymlink(const char *path, const char *link) {
    LOGM();
    return 0;
}

int MyFS::fuseRename(const char *path, const char *newpath) {
    LOGM();
    return 0;
}

int MyFS::fuseLink(const char *path, const char *newpath) {
    LOGM();
    return 0;
}

int MyFS::fuseChmod(const char *path, mode_t mode) {
    LOGM();
    return 0;
}

int MyFS::fuseChown(const char *path, uid_t uid, gid_t gid) {
    LOGM();
    return 0;
}

int MyFS::fuseTruncate(const char *path, off_t newSize) {
    LOGM();
    return 0;
}

int MyFS::fuseUtime(const char *path, struct utimbuf *ubuf) {
    LOGM();
    return 0;
}

int MyFS::fuseOpen(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();

    //Unser Zeug bzw Waldhorst?
    // TODO: Implement this!

    LOGF("--> Trying to read %s, %lu, %lu\n", path, (unsigned long) offset, size);

    char file54Text[] = "Hello Woooooooooooooorld From File54!\n";
    char file349Text[] = "Hello World From File349!\n";
    char *selectedText = NULL;

    // ... //

    if (strcmp(path, "/file54") == 0)
        selectedText = file54Text;
    else if (strcmp(path, "/file349") == 0)
        selectedText = file349Text;
    else
        return -ENOENT;

    // ... //

    memcpy(buf, selectedText + offset, size);

    return ((int) (strlen(selectedText) - offset));
    //Ende unser Zeug

}

int MyFS::fuseWrite(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseStatfs(const char *path, struct statvfs *statInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseFlush(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseRelease(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseFsync(const char *path, int datasync, struct fuse_file_info *fi) {
    LOGM();
    return 0;
}

int MyFS::fuseSetxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
    LOGM();
    return 0;
}

int MyFS::fuseGetxattr(const char *path, const char *name, char *value, size_t size) {
    LOGM();
    return 0;
}

int MyFS::fuseListxattr(const char *path, char *list, size_t size) {
    LOGM();
    return 0;
}

int MyFS::fuseRemovexattr(const char *path, const char *name) {
    LOGM();
    return 0;
}

int MyFS::fuseOpendir(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();


    //Unser Zeug
    // (TODO: Implement this!)

    // Wir erlauben nur "/" oder "." als korrekte Pfade, alles andere würde ein anderes VZ benötigen
    if (strcmp(path, "/") == 0 || strcmp(path, ".") == 0) {
        RETURN(0);
    } else {
        RETURN(-ENOTDIR);
    }
    //Ende unser Zeug

    return 0;
}

int
MyFS::fuseReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();


    //Unser Zeug
    // TODO: Implement this!

    LOGF("--> Getting The List of Files of %s\n", path);

    // Buffer in den geschrieben wird: Filename, Attribute, Offset of next entry oder 0
    filler(buf, ".", NULL, 0); // Current Directory
    filler(buf, "..", NULL, 0); // Parent Directory

    // Möchte der Benutzer den Inhalt des root haben, gehe durch alle Inodes und suche vorhandene Dateien
    if (strcmp(path, "/") == 0) {
        Inode *inode = NULL;
        for (int i = 0; i < NUM_DIR_ENTRIES; i++) {
            inode = root.getInodeByIndex(i);
            //Wenn ein Dateiname in der Inode existiert, füge ihn dem Buffer zu
            if (strlen(inode->name) != 0) {
                filler(buf, inode->name, NULL, 0);
            }
        }
    } else {
        LOG("Readdir: Angefragter Pfad ist nicht zulässig. Es gibt nur ein Verzeichnis.")
        RETURN(-ENOTDIR);
    }
    //Ende unser Zeug

    return 0;
}

int MyFS::fuseReleasedir(const char *path, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseFsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseInit(struct fuse_conn_info *conn) {

    // Open logfile
    this->logFile = fopen(((MyFsInfo *) fuse_get_context()->private_data)->logFile, "w");
    if (this->logFile == NULL) {
        fprintf(stderr, "ERROR: Cannot open logfile %s\n", ((MyFsInfo *) fuse_get_context()->private_data)->logFile);
        return -1;
    } else {

        // turn of logfile buffering
        setvbuf(this->logFile, NULL, _IOLBF, 0);

        LOG("Starting logging...\n");
        LOGM();

        // you can get the containfer file name here:
        LOGF("Container file name: %s", ((MyFsInfo *) fuse_get_context()->private_data)->contFile);

        // TODO: Enter your code here!


        //Unser Zeug


        // Get in-memory flag
        this->inMemoryFs = (((MyFsInfo *) fuse_get_context()->private_data)->inMemoryFs == 1);

        if (this->inMemoryFs) {
            LOG("Using in-memory mode");
            // TODO: Move your in-memory initialization methods here!

            //long *p = (long *)8; so initialisiert man pointer

            struct SBlock { //groeße in Bloecken angegeben: bei welchem Block beginnt
                int firstDMapBlock = 1, firstInodeBlock = 2, firstFATBlock = 65, firstRootBlock = 66, firstDataBlock = 67;
            };

        } else {
            LOGF("Container file name: %s", ((MyFsInfo *) fuse_get_context()->private_data)->contFile);

            //betrete blockdevice/containerdatei?
            int ret = this->blockDevice.open(((MyFsInfo *) fuse_get_context()->private_data)->contFile);

            if (ret >= 0) {
                LOG("Container file does exist, reading");

                // TODO: Read existing structures form file


            } else if (ret == -ENOENT) {
                LOG("Container file does not exist, creating new one");

                //BlockDevice *device = new BlockDevice(512); //ein (simulierter)Datenträger wird initialisiert
                ret = this->blockDevice.create(((MyFsInfo *) fuse_get_context()->private_data)->contFile);

                if (ret >= 0) {
                    // Create empty structures in file


                    struct SBlock { //groeße in Bloecken angegeben: bei welchem Block beginnt
                        int sBlock = 0, firstDMapBlock = 1, firstInodeBlock = 2, firstFATBlock = 65, firstRootBlock = 66, firstDataBlock = 67;
                        //int FIRST_DMAP_BLOCK, FIRST_INODE_BLOCK, FIRST_FAT_BLOCK, FIRST_ROOT_BLOCK, FIRST_DATA_BLOCK;
                    };
                    // groeße SBlock == 4 int-werte=1Block==1
                    // groeße dMap: 512 == 1 Block ==1
                    // groeße inode-block: 32768 == 64 x 512  == NUM_DIR_ENTRIES x 1 Block == 64
                    // groeße rootBlock: = 512 == 1Block == 1
                    // groeße fat: 64 == NUM_DIR_ENTRIES (wir schreiben nur ganze blöcke)<= 1Block = 512 == 1
                    // groeße data dMapByte/groeßeEinesEintrages == 512/4 = 128

                    char *SBlock_buffer = new char[128 * BLOCK_SIZE]();

                    std::memcpy(SBlock_buffer, BLOCK_SIZE, sizeof(BLOCK_SIZE));

                    //char *dMap_buffer = new char[1 * BLOCK_SIZE]();
                    //char *inode_buffer = new char[64 * BLOCK_SIZE]();
                    //char *rootDir_buffer = new char[1 * BLOCK_SIZE]();
                    //char *fat_buffer = new char[(1 * BLOCK_SIZE)];

                    //LOG("Fat wird auf Platte geschrieben");
                    //fat.flush(fat_buffer);
                    //for (int i = 0; i < 64; i++) { // bsp: von 0000:0200 bis 001f:41ff
                    //    blockDevice.write(FIRST_FAT_BLOCK + i, fat_buffer + (512 * i));
                    //}
                    //std::cout << "die Fat ist nun auf der Platte!" << std::endl;

                    //root.flush(inode_buffer);

                    //LOG("Inpdes werden auf die Platte geschrieben");
                    //for (int i = 0; i < 64; i++) { // bsp: von 001f:4200 bis 001f:c1ff
                    //    blockDevice.write(FIRST_INODE_BLOCK + i, inode_buffer + (512 * i));
                    //}
                    //std::cout << "inode on disk" << std::endl;

                    //Root::flush(rootdir_buffer);
                    //bd.write(ROOT_DIR_START,rootdir_buffer ); //001f:c200 :
                    //std::cout << "Rootdir on disk" << std::endl;

                }
            }

            if (ret < 0) {
                LOGF("ERROR: Access to container file failed with error %d", ret);
            }
        }


    }

    MyFS *file_system = MyFS::Instance();

//Ende unser Zeug

    return 0;
}

int MyFS::fuseTruncate(const char *path, off_t offset, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

int MyFS::fuseCreate(const char *path, mode_t mode, struct fuse_file_info *fileInfo) {
    LOGM();
    return 0;
}

void MyFS::fuseDestroy() {
    LOGM();
}

// TODO: Add your own additional methods here!

/*
 * Template Funktion um Daten auf das Blockdevice zu schreiben, erstellt einen Puffer,
 * liest nach und nach die Daten aus und schreibt sie Stückchenweit auf das Blockdevice
 * TODO: Block dynamisch anpassen
 * TODO: Ist unbenutzt
 */
template<typename T>
int writeOnBlockDevice(BlockDevice *device, u_int32_t block, const T *data) {
    char buffer[BLOCK_SIZE]; //Puffer in dem die Daten zwischengespeichert werden

    memset(buffer, 0,
           BLOCK_SIZE); // Die 512 Byte des Puffers werden mit 0en belegt, damit er ausgefüllt ist und keine Fehler beim Schreiben auftreten

    int blocksNecessary = (sizeof(T) / BLOCK_SIZE) + 1;

    for (int i = 0; i < blocksNecessary; ++i) {
        memcpy(buffer, data + BLOCK_SIZE * i, BLOCK_SIZE); //Kopiert 512 Byte in den Puffer
        memset(buffer, 0,
               BLOCK_SIZE); // Die 512 Byte des Puffers werden mit 0en belegt, damit er ausgefüllt ist und keine Fehler beim Schreiben auftreten
    }
    //TODO?
    device->write(block, buffer);

    return ALL_OK;
}
