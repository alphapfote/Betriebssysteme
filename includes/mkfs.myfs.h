//
// Created by hendr on 15.12.2017.
//

#ifndef BETRIEBSSYSTEME_MKFS_MYFS_H
#define BETRIEBSSYSTEME_MKFS_MYFS_H

#define NAME_LENGTH 255 			// Maximale Länge eines Dateinamens in Zeichen
#define BLOCK_SIZE 512 				// Logische Blockgröße
#define NUM_INODES 64 				// Anzahl Inodes
#define NUM_MAX_FILES NUM_INODES 	// Maximale Anzahl Dateien
#define AMOUNT_BLOCKS 64000			// Anzahl der Blöcke
#define FIRST_DATABLOCK	568			// Erster Block mit Dateiinhalt
#define FIRST_INODEBLOCK 502		// Erster Block mit Inodeinhalt
char buffer[BLOCK_SIZE];



#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
#include "mkfs.myfs.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstring>
using namespace std;

// vom Blockdevice, nimmt normale Datentypen und Structs entgegen
template<typename T> void read_device(BlockDevice* device, u_int32_t block,
                                      T* data) {
    static char buffer[BLOCK_SIZE];
    device->read(block, buffer);
    *data = *reinterpret_cast<T*>(buffer);
}

// Funktion zum Lesen vom  Blockdevice, nimmt Arrays entgegen
template<std::size_t N, typename T> void read_device(BlockDevice* device,
                                                     u_int32_t block, T (&data)[N]) {
    static char buffer[BLOCK_SIZE];
    device->read(block, buffer);
    memcpy(&data, buffer, sizeof(T[N])); //geht als schleife durch den array
}

// data (T) muss ein Array oder Struct sein, kein Pointer
// Funktion zum Schreiben auf das Blockdevice, nimmt normale Datentypen und Structs entgegen
template<typename T> void write_device(BlockDevice* device, u_int32_t block, const T* data) {
    static_assert(sizeof(T) <= BLOCK_SIZE, "T must not be bigger than the block size");

    static char buffer[BLOCK_SIZE];

    memset(buffer, 0, BLOCK_SIZE); // Die 512 Byte des Puffers werden mit 0en belegt, damit er ausgefüllt ist.
    memcpy(buffer, data, sizeof(T)); // So viel Byte wie nötig werden mit dem Inhalt von Data überschrieben, der Rest bleibt 0

    device->write(block, buffer);
}

// Funktion zum Schreiben auf das Blockdevice, nimmt Arrays entgegen
template<std::size_t N, typename T> void write_device(BlockDevice* device,
                                                      u_int32_t block, const T (&data)[N]) {
    static_assert(sizeof(T[N]) <= BLOCK_SIZE, "T must not be bigger than the block size");

    static char buffer[BLOCK_SIZE];

    memset(buffer, 0, BLOCK_SIZE);
    memcpy(buffer, data, sizeof(T[N]));

    device->write(block, buffer);
}

void writeInodeData(BlockDevice* bd, char* file, struct stat metadata);
void readAndWriteFile(BlockDevice* device, char* file);

#endif //BETRIEBSSYSTEME_MKFS_MYFS_H