//
// Created by hendrik on 10.01.20.
//

#include "root.h"
#include "string.h"

// blocksNeeded = Anzahl Blöcke die noch geschrieben werden sollen
int MyFS::writeDataInDMap(int blocksNeeded = 0) {
    int i = 0;
    int j = 0;

    int freeBlocks[NUM_DIR_ENTRIES] = {IS_FREE};

    // Zählt ob genügend Platz frei ist, schaut in der DMap ob genügend freie Blöcke vorhanden sind
    // und schreibt deren Indices ind "freeBlocks", der als Zwischenspeicher für die Indices dient

    while (blocksNeeded != 0 && i < NUM_DIR_ENTRIES) {
        if (dMap[i] == IS_FREE) {
            freeBlocks[j++] = i;
            blocksNeeded--;
            i++;
        }
    }

    // Falls nicht genug Platz fürs Schreiben vorhanden ist wird ein Fehler zurückgegeben
    if (blocksNeeded > 0) {
        return NOTHING_IS_FREE;
    }

    //FAT Befüllen
    //In freeBlocks stehen die Indices des FS die frei sind, hier werden in freie FAT Einträge die
    j = 0;
    while (freeBlocks[j] != IS_FREE) {
        fileAllocationTable[getFirstFreeFat()] = freeBlocks[j++];
    }

    return ALL_OK;

};


