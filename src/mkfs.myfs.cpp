//
//  mk.myfs.cpp
//  myfs
//
//mithilfe dieser datei wird eine containerdatei erstellt - oder das ganze dateisystem?
//(mithilfe des kommandos mkfs.myfs)
//
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
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
	unsigned int* pointerToFirstFileBlock; // Zeiger auf ersten Eintrag in der FAT für die Datei
};
Inode* inodeList = new Inode[NUM_INODES];

// Superblock
unsigned long sizeOfFileSystem;

// die FAT ist eigentlich ein zweidimensionaler array,
//der zu den datei-blöcken die die verlinkungen anzeigt,
//kann man auch als eindimensionalen array visualisieren, da durch den iterator immer klar ist, wo man ist
unsigned int* FAT[AMOUNT_BLOCKS]; //größe:anzahl der blöcke

// Root-Verzeichnis
unsigned char* inodeID; // ID's der Inodes

/**
 *  Die Methode schreibt eine Datei in den Container
 *
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
void writeInodeData(char* file, struct stat metadata) {

	struct Inode inodeName;
	inodeName.filename = basename(file);
	inodeName.filesize = metadata.st_size;
	inodeName.uid_t = metadata.st_uid;
	inodeName.gid_t = metadata.st_gid;
	inodeName.mode_t = S_IFREG | 0444; // In der Aufgabenstellung gegeben
	inodeName.atime = metadata.st_atime;
	inodeName.mtime = metadata.st_mtime;
	inodeName.ctime = metadata.st_ctime;
	// TODO Pointer pointer = malloc(filesize)

	inodeList[sizeof(inodeList)] = inodeName;
}

// Liest den Inhalt einer Datei aus, zerlegt die Datei in Blöcke, die im FS gespeichert werden können-.
void readInputFile(char* file) {

	// Erstellen einer leeren Struktur für die Metadaten
	struct stat metadata;
	// Füllen der Struktur mit den durch stat ausgelesenen Daten
	stat(file, &metadata);

	// Erstellung der Inode für die Datei
	writeInodeData(file, metadata);

	int blockAmount = metadata.st_size / 512;//anzahl der benötigten blöcke





//while(datei >512byte) {
	//schneide 512 byte ab
	//erstelle adresse zum blcok, indem dieser teil abgelegt wird
	//beim 1. durchlauf: adresse wird in inode gespeichert
	//beim 1 - (x-1). durchlauf: adresse wird in der fat gespeichert
	//beim x. durchlauf: eintrag NULL in der fat, zeichen dafür, dass das der letzte block ist
//}
}


void write_superblock(char* buffer, unsigned long file_system_size) {
	memset(buffer, 0, BLOCK_SIZE);
	memcpy(buffer, &file_system_size, sizeof(unsigned long));
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

	//Superblock
	write_superblock(buffer, AMOUNT_BLOCKS * BLOCK_SIZE);
	bd.write(0, buffer);

	memset(buffer, 0, BLOCK_SIZE);
	bd.read(0, buffer);
	std::cout << "superblock: " << *(reinterpret_cast<unsigned long*>(buffer)) << std::endl;

	// Jede Datei wird ausgelesen und auf das FS geschrieben
	for (int i = 2; i < argc; i++) {

		writeFileInContainer(argv[2], containerPath);
	}
	return 0;
}
