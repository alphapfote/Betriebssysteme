//
// Created by hendrik on 10.01.20.
//

#include "root.h"
#include "string.h"

//FAT

int fat::getFirstFreeFat() {
    for (int i = 0; i < NUM_DIR_ENTRIES; i++) {
        if (fileAllocationTable[i] == IS_FREE) {
            return i;
        }
    }
    return NOTHING_IS_FREE;
}

int fat::getNextDataBlock(int index) {
    return fileAllocationTable[index];
}

/*
 * Löscht Eine Datei in der Fat und läuft dann rekursiv durch die Einträge bis alle Einträge der Datei gelöscht sind.
 * */
void fat::deleteFileInFat(int index) {
    int nextBlock = getNextDataBlock(index);

    if (nextBlock == IS_FREE) { //Ist der aktuelle Block nicht belegt wird die Methode abgebrochen
        return;
    }

    fileAllocationTable[index] = IS_FREE; //Eintrag in der FAT wird gelöscht

    if (nextBlock == IS_END) { //Ist der aktuelle Block ein End of File Block wird die Methode abgebrochen
        return;
    }

    deleteFileInFat( nextBlock); //Ist die Datei noch über weitere Blöcke verteilt wird die Methode rekursiv erneut aufgerufen
}

/*
 * Speichert neue Dateien, berechnet aus der Size die nötigen Blöcke die zu belegen sind
 */
void fat::saveNewFileInFat(int index, int size) {
}

/**
* lädt die Fat von der Platte in den RAM
*/
void Fat::load(char *data, uint32_t size) {
    std::memcpy(&nextblockaddr, data, size);
}

/**
 * schreibt die Fat vom RAM auf die Platte
 */
void Fat::flush(char *data) {
    std::memcpy(data, nextblockaddr, sizeof(nextblockaddr));
}
