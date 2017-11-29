//
//  mk.myfs.cpp
//  myfs
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
using namespace std;

#define NAME_LENGTH 255 			// Maximale Länge eines Dateinamens in Zeichen
#define BLOCK_SIZE 512 				// Logische Blockgröße
#define NUM_INODES 64 				// Anzahl Inodes
#define NUM_MAX_FILES NUM_INODES 	// Maximale Anzahl Dateien
#define AMOUNT_BLOCKS 500			// Anzahl der Blöcke

// Wird als Struktur gebaut, da Inodes mehrfach vorhanden sind.
struct Inode {
	string filename; // Dateiname
	unsigned long filesize; // Deteigröße
	unsigned int uid_t; // User-ID
	unsigned int gid_t; // Group-ID
	unsigned short mode_t; // zugriffsberechtigungen (mode)
	unsigned long atime; // Access Time (letzter Zugriff)
	unsigned long mtime; // Modification Time (letzte Änderung)
	unsigned long ctime; // Status Change Time (letzte Statusänderung)
	unsigned int* pointerToFirstFileBlock // Zeiger auf ersten Eintrag in der FAT für die Datei
};
struct Inode inodeList[];

// Superblock
unsigned long sizeOfFileSystem;
unsigned long sizeofInodeList;

// FAT
unsigned int* FAT[];

// Root-Verzeichnis
unsigned char* inodeID; // ID's der Inodes

/**
 *  Die Methode schreibt eine Datei in den Container
 *
 *  @param data die zu schreibende Datei
 *  @param containerPath Name der zu schreibenden Datei
 */
void writeFileInContainer(char* data, char* containerPath) {
	BlockDevice bd = BlockDevice();
	bd.create(containerPath);

//	bd.write(1, data);
	bd.write(1, "HelloWorld");

}


/**
 * Die Methode füllt die Inodes für das Schreiben einer Datei
 * @param metadata, die Metadaten die für das schreiben der Inode gebraucht werden
 * @param fileName, Name der Datei
 */
void writeInodeData(char* file) {
	// Erstellen einer leeren Struktur für die Metadaten
	struct stat metadata;
	// Füllen der Struktur mit den durch stat ausgelesenen Daten
	stat(file, &metadata);

	struct Inode inodeName = new Inode;
	inodeName.filename = file;
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


// argv = argument value
// generische form, mit der ein c++ programm aufgerufen wird
// erste sagt, wie viele elemente es in argv sind
// argv[0] ist immer der (Container)Dateiname, besser gesagt Programmname -> default vom betriebssystem
// argv[1] ist der Name des Containers
int main(int argc, char *argv[]) {


	// TODO: Implement file system generation & copying of files here

	char* containerPath = *argv[1];
	writeInodeData(argv[2]);
	writeFileInContainer(argv[2], containerPath);

	return 0;
}
