// erstellt/schreibt/liest eine einzelne Inode
//
// created by alice on 13.1.20

#ifndef inode_h
#define inode_h

#include "myfs-info.h"

class Inode {
public:

    char name[NAME_LENGTH]; //fixe größe für name festlegen?
    size_t size = 0;        //Größe der Datei in Byte?
    char *data;             //Pointer auf den Beginn des Inhalts
    uid_t uid;              // User-ID
    gid_t gid;              // Gruppen-ID
    mode_t mode;            //Zugriffsberechtigungen
    time_t atime;           //Letzter Zugriff
    time_t ctime;           //Letzte Änderung
    time_t mtime;           //Letzte Statusänderung

    Inode() {
        Inode *inode = new Inode(); //reserviert platz usw, gibt pointer auf inode zurück
    }

    ~Inode();


    //Inode
//    int inodeFlush();
//    int inodeRead();
};

#endif //fat_h