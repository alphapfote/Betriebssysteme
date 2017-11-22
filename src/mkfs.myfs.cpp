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
using namespace std;

// Wird als Struktur gebaut, da Inodes mehrfach vorhanden sind.
struct Inode {
	string filename; // Dateiname
	unsigned long filesize; // Deteigröße
	unsigned int uid_t; // User-ID
	unsigned int gid_t; // Group-ID
	unsigned short mode_t; // zugriffsberechtigungen (mode)
	unsigned long atime; // Acces Time (letzter Zugriff)
	unsigned long mtime; // Modification Time (letzte Änderung)
	unsigned long ctime; // Status Change Time (letzte Statusänderung)
	unsigned int* pointerFirstDatablock; // Zeiger	auf	ersten	Datenblock
};

// Superblock
unsigned long sizeOfFileSystem;
unsigned long sizeofInodeList;



















int main(int argc, char *argv[]) {

	// TODO: Implement file system generation & copying of files here

	return 0;
}
