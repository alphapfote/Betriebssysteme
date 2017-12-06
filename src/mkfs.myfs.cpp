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
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <cstring>
using namespace std;

#define NAME_LENGTH 255 			// Maximale Länge eines Dateinamens in Zeichen
#define BLOCK_SIZE 512 				// Logische Blockgröße
#define NUM_INODES 64 				// Anzahl Inodes
#define NUM_MAX_FILES NUM_INODES 	// Maximale Anzahl Dateien
#define AMOUNT_BLOCKS 500			// Anzahl der Blöcke
char buffer[BLOCK_SIZE];

// Wird als Struktur gebaut, da Inodes mehrfach vorhanden sind.
struct Inode {
	string filename; // Dateiname
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
unsigned int FAT[AMOUNT_BLOCKS]; //Größe: Anzahl der Blöcke

// Root-Verzeichnis
unsigned short inodeID[64]; // ID's der Inodes

/**
 *  Die Methode schreibt eine Datei in den Container
 *  @param data die zu schreibende Datei
 *  @param containerPath Name der zu schreibenden Datei
 */
void writeFileInContainer(char* file, char* containerPath) {
//	bd.write(1, file);
//	bd.write(1, "HelloWorld");
}

/**
 * Die Methode füllt die Inodes für das Schreiben einer Datei
 * @param metadata, die Metadaten die für das Schreiben der Inode gebraucht werden
 * @param file, Name der Datei
 */
void writeInodeData(char* file, struct stat metadata, unsigned int firstBlock) {

	struct Inode inodeName;
	inodeName.filename = basename(file);
	inodeName.filesize = metadata.st_size;
	inodeName.uid_t = metadata.st_uid;
	inodeName.gid_t = metadata.st_gid;
	inodeName.mode_t = S_IFREG | 0444; // In der Aufgabenstellung gegeben
	inodeName.atime = metadata.st_atime;
	inodeName.mtime = metadata.st_mtime;
	inodeName.ctime = metadata.st_ctime;
	inodeName.firstBlock = firstBlock;

	inodeList[sizeof(inodeList)] = inodeName;
}

template<typename T> void read_device(BlockDevice* device, u_int32_t block, T* data) {
	static char buffer[BLOCK_SIZE];
	device->read(block, buffer);
	*data = *reinterpret_cast<T*>(buffer);
}

template<std::size_t N, typename T> void read_device(BlockDevice* device, u_int32_t block, T (&data)[N]) {
	static char buffer[BLOCK_SIZE];
	device->read(block, buffer);
	memcpy(&data, buffer, sizeof(T[N])); //geht als schleife durch den array
}

// data (T) muss ein Array oder Struct sein, kein Pointer
template<typename T> void write_device(BlockDevice* device, u_int32_t block, const T* data) {
	static_assert(sizeof(T) <= BLOCK_SIZE, "T must not be bigger than the block size");

	static char buffer[BLOCK_SIZE];

	memset(buffer, 0, BLOCK_SIZE);
	memcpy(buffer, data, sizeof(T));

	device->write(block, buffer);
}

template<std::size_t N, typename T> void write_device(BlockDevice* device, u_int32_t block, const T(&data)[N]) {
	static_assert(sizeof(T[N]) <= BLOCK_SIZE, "T must not be bigger than the block size");

	static char buffer[BLOCK_SIZE];

	memset(buffer, 0, BLOCK_SIZE);
	memcpy(buffer, data, sizeof(T[N]));

	device->write(block, buffer);
}

// Liest den Inhalt einer Datei aus, zerlegt die Datei in Blöcke, die im FS gespeichert werden können-.
void readInputFile(BlockDevice* device, char* file) {

	// Erstellen einer leeren Struktur für die Metadaten
	struct stat metadata;
	// Füllen der Struktur mit den durch stat ausgelesenen Daten
	stat(file, &metadata);

	// Erstellung der Inode für die Datei
	writeInodeData(file, metadata, 99);

	int blockAmount = metadata.st_size / 512; //anzahl der benötigten blöcke

	static char buffer[BLOCK_SIZE];

	ifstream input(file);
	for (u_int32_t block = 100; input; ++block) {
		memset(buffer, 0, sizeof(buffer));
		input.read(buffer, sizeof(buffer));
		write_device(device, block, &buffer[0]);
	}

//	while (file >= BLOCK_SIZE) {
//		//schneide 512 byte ab
//		//erstelle adresse zum blcok, indem dieser teil abgelegt wird
//		//beim 1. durchlauf: adresse wird in inode gespeichert
//		//beim 1 - (x-1). durchlauf: adresse wird in der fat gespeichert
//		//beim x. durchlauf: eintrag NULL in der fat, zeichen dafür, dass das der letzte block ist
//	}
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
	Inode inode { "foobar", 0, 1000, 1000, 0777, 0, 0, 0, 0 }, inode2;
	write_device(&bd, 1, &inode); // Beispiele / Tests
	read_device(&bd, 1, &inode2);

	// Root-Verzeichnis
	for (int i = 0; i < 64; ++i) {
		inodeID[i] = i + 1;
	}
	unsigned short inodeID2[64];
	write_device(&bd, 67, inodeID);
	read_device<64>(&bd, 67, inodeID2);
	for (int i = 0; i < 64; ++i) {
		cout << inodeID2[i] << endl;
	}


	// Jede Datei wird ausgelesen und auf das FS geschrieben
	for (int i = 2; i < argc; i++) {

		writeFileInContainer(argv[2], containerPath);
	}
	return 0;
}
