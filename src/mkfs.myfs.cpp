//
//  mk.myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//  Copyright © 2017 Alice Rühle, Hendrik Diemer, Jana Becker
//

#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
#include "mkfs.myfs.h"
#include <string>
#include <iostream>62465
#include <fstream>
#include <sys/stat.h>
#include <cstring>
using namespace std;

#define NAME_LENGTH 255 			// Maximale Länge eines Dateinamens in Zeichen
#define BLOCK_SIZE 512 				// Logische Blockgröße (Bytes)
#define NUM_INODES 64 				// Anzahl Inodes (Bytes)
#define NUM_MAX_FILES NUM_INODES 	// Maximale Anzahl Dateien
#define AMOUNT_BLOCKS 64000			// Anzahl der Blöcke
#define FIRST_DATABLOCK	568			// Erster Block mit Dateiinhalt
#define FIRST_INODEBLOCK 502		// Erster Block mit Inodeinhalt
//char buffer[BLOCK_SIZE];

unsigned long blocksOccupied = 0;
unsigned short filesWritten = 0;

// Wird als Struktur gebaut, da Inodes mehrfach vorhanden sind.
struct Inode {
	char* filename; // Dateiname
	unsigned long filesize; // Dateigröße
	unsigned int uid_t; // User-ID
	unsigned int gid_t; // Group-ID
	unsigned short mode_t; // zugriffsberechtigungen (mode)
	unsigned long atime; // Access Time (letzter Zugriff)
	unsigned long mtime; // Modification Time (letzte Änderung)
	unsigned long ctime; // Status Change Time (letzte Statusänderung)
	unsigned int firstBlock; // Zeiger auf ersten Eintrag in der FAT für die Datei
};

Inode* inodeList = new Inode[NUM_INODES];

// Superblock
unsigned long sizeOfFileSystem = AMOUNT_BLOCKS * BLOCK_SIZE;

// FAT
// die FAT ist ein eindimensionales Array, der zu den Datei-Blöcken die Verlinkungen anzeigt.
// Durch den Iterator ist immer klar, wo man ist
// Die FAT ist 4 Blöcke groß
unsigned int FAT[AMOUNT_BLOCKS]; //Größe: Anzahl der Blöcke


// Root-Verzeichnis
unsigned short inodeID[64]; // ID's der Inodes




/**
 * Die Methode füllt die Inodes für das Schreiben einer Datei
 * @param metadata, die Metadaten die für das Schreiben der Inode gebraucht werden
 * @param file, Name der Datei
 */
void writeInodeData(BlockDevice* bd, char* file, struct stat metadata) {

	struct Inode inodeName;
	inodeName.filename = basename(file);
	inodeName.filesize = metadata.st_size;
	inodeName.uid_t = metadata.st_uid;
	inodeName.gid_t = metadata.st_gid;
	inodeName.mode_t = S_IFREG | 0444; // In der Aufgabenstellung gegeben
	inodeName.atime = metadata.st_atime;
	inodeName.mtime = metadata.st_mtime;
	inodeName.ctime = metadata.st_ctime;
	inodeName.firstBlock = FIRST_DATABLOCK + blocksOccupied;

	write_device(bd, FIRST_INODEBLOCK + filesWritten, &inodeName);
//	inodeList[sizeof(inodeList)] = inodeName;
}

// Liest den Inhalt einer Datei aus, zerlegt die Datei in Blöcke, die im FS gespeichert werden können.
void readAndWriteFile(BlockDevice* device, char* file) {
    Inode readInode;

    for (int i = 0; i < filesWritten; ++i) {
        read_device(device, FIRST_INODEBLOCK + i, &readInode);
        if(readInode.filename == file){
            cerr << "EEXIST: Dateiname existiert bereits";
        }
    }


	// Erstellen einer leeren Struktur für die Metadaten
	struct stat metadata;
	// Füllen der Struktur mit den durch stat ausgelesenen Daten
	stat(file, &metadata);

	// Erstellung der Inode für die Datei
    // Die Klammer steht für den ersten freien Block
	writeInodeData(device, file, metadata);

	blocksOccupied += metadata.st_size / 512; //Anzahl der benötigten Blöcke wird hochgezählt.

	static char buffer[BLOCK_SIZE];
	ifstream input(file); // Input File Stream
	for (u_int32_t block = (FIRST_DATABLOCK + blocksOccupied); input; ++block) { //Wir schreiben ab Block 100, davor Inodes, Superblock, FAT und Root-Verzeichnis
		memset(buffer, 0, sizeof(buffer));
		input.read(buffer, sizeof(buffer));
		// TEST cout << buffer << endl;
		write_device(device, block, &buffer[0]);

		// TODO: FAT dynamisch erstellen (Aufgabe 2)
		FAT[block] = block + 1;
	}
    ++filesWritten;
}

template<typename T> void write_device(BlockDevice* device, u_int32_t block, const T* data) {
    static_assert(sizeof(T) <= BLOCK_SIZE, "T must not be bigger than the block size");

    static char buffer[BLOCK_SIZE];

    memset(buffer, 0, BLOCK_SIZE); // Die 512 Byte des Puffers werden mit 0en belegt, damit er ausgefüllt ist.
    memcpy(buffer, data, sizeof(T)); // So viel Byte wie nötig werden mit dem Inhalt von Data überschrieben, der Rest bleibt 0

    device->write(block, buffer);
}


// argv = argument value
// generische form, mit der ein c++ programm aufgerufen wird
// erste sagt, wie viele elemente es in argv sind
// argv[0] ist immer der Programmname -> default vom betriebssystem
// argv[1] ist der Name des Containers
int main(int argc, char *argv[]) {
	// TODO: Implement file system generation & copying of files here
	char* containerPath = argv[1]; //Adresse unseres Containers, in den wir die Daten des FS reinschreiben
	BlockDevice bd(BLOCK_SIZE);
	bd.create(containerPath);

	// Superblock
	write_device(&bd, 0, &sizeOfFileSystem);

	// FAT
//	for (int i = 0; i < AMOUNT_BLOCKS; ++i){
//		FAT[i] = 0;
//	}
	//wird initialisiert, alle blöcke werden belegt & auf 0 gesetzt
	static char buffer[BLOCK_SIZE];
	memset(buffer, 0, BLOCK_SIZE);

    for (int i = 1; i <= 501; ++i){
        write_device(&bd, i, buffer);
    }


//	TEST
//	Inode inode { "foobar", 0, 1000, 1000, 0777, 0, 0, 0, 0 }, inode2;
//	write_device(&bd, 1, &inode); // Beispiele / Tests
//	read_device(&bd, 1, &inode2);

	// Root-Verzeichnis (eigentlich unnötig)
	for (unsigned short i = 0; i < 64; ++i) {
		inodeID[i] = i + 1;
	}
	write_device(&bd, 70, inodeID);
//	TEST
//	unsigned short inodeID2[64];
//	write_device(&bd, 67, inodeID);
//	read_device<64>(&bd, 67, inodeID2);
//	for (int i = 0; i < 64; ++i) {
//		cout << inodeID2[i] << endl;
//	}

	// Jede Datei wird ausgelesen und auf das FS geschrieben
	for (int i = 2; i < argc; i++) {
		readAndWriteFile(&bd, argv[i]);
	}
	return 0;
}
