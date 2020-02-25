//
// Created by hendrik on 10.01.20.
//

#include "root.h"
#include "string.h"

Root root::rootDir = new Root[NUM_DIR_ENTRIES]; //Liste aller Verweise auf Inodes identifizierbar mit ID (Platzierung)

//TODO: Kann glaub ich weg
root:Root() {

};

root:~Root() {

};


//Root ist eine Liste aller Inodes, die Funktion checkt jede Inode des Arrays und gibt die erste ohne Inhalt zurück
int root::getFreeRootEntry() {
    for (int i = 0; i < NUM_DIR_ENTRIES; ++i) {
        if (rootDir[i].size == 0) {
            return i;
        }
    }
    return NOTHING_IS_FREE;
}

// Hier übergibt man der Funktion einen Namen und die Methode prüft ob es eine Inode mit diesem Namen gibt und gibt sie ggf. zurück
Inode *root::getInodeByName(char *name) {
    for (int i = 0; i < NUM_DIR_ENTRIES; i++) {
        if (strcmp(rootDir[i].name, name) == 0) {
            return rootDir[i];
        }
    }
    return nullptr;
}

//Setzt den Eintrag im rootDir auf eine Inodeadresse
void root::setInode(int rootIndex, Inode *inode) {
    rootDir[rootIndex] = inode;
}

//Schreibt eine Inode auf das BD
void root::writeInodeOnBlockdevice(int rootIndex, Inode *inode) {
    assert((rootIndex > ROOT_INDEX) && (rootIndex < (NUM_DIR_ENTRIES + ROOT_INDEX));

/*TODO: Inodes löschen
 *if (_inodes== nullptr){
//        char reset_buff[_block_size];
//        memset(reset_buff, 0, _block_size);
//        assert(_block_device_ptr->write(block_idx, reset_buff) == 0);
//        return;
//    }
*/

    char buffer[BLOCK_SIZE];
    memset(buffer, 0, BLOCK_SIZE);
    memcpy(buffer, inode, sizeof(inode));


    //device->read(rootIndex, buffer); -> TODO: benötigen wir das?

    // S-Block ist der erste Block im Device, da root-Verzeichnis Indices bei 0 beginnen, muss 1 addiert werden.
    // Also Befinden sich die Inodes bei Block 2-66
    device->write(rootIndex + 2, buffer);
}

// Gibt die Adresse einer Inode zurück
Inode *root::getInodeByIndex(int rootIndex) {
    assert((rootIndex > ROOT_INDEX) && (rootIndex < (NUM_DIR_ENTRIES + ROOT_INDEX));

    return rootDir[rootIndex];
}

//Speichert den Inhalt einer Inode in einen Buffer
void root::readInodeFromBlockdevice(int rootIndex, char *buffer) {
    assert((rootIndex > ROOT_INDEX) && (rootIndex < (NUM_DIR_ENTRIES + ROOT_INDEX));

    char buffer[BLOCK_SIZE];

    // +2 siehe write
    bd.read(rootIndex+2, buffer);

}


