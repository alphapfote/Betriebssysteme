//cat
//  mk.myfs.cpp
//  myfs
//
//  Created by Oliver Waldhorst on 07.09.17.
//  Copyright © 2017 Oliver Waldhorst. All rights reserved.
//

#include <fstream>
#include <iostream>
#include "myfs.h"
#include "blockdevice.h"
#include "macros.h"
using namespace std;

struct rootElement {
	string fileName;
	unsigned long int inodeNumber;
};

struct Inode {
	string filename;
	unsigned long int filesize;
	unsigned long int uId;
	unsigned short int accessLevel;
	//Last access
	unsigned long int atime;
	//Last modification
	unsigned long int mtime;
	//Last status change
	unsigned long int ctime;
	unsigned long int firstDataBlocknumberInFAT;
};

//Superblock
unsigned long int fsSize;
unsigned long int amountFreeBlocks;

unsigned long int indexFirstFreeBlock;
unsigned long int inodeListSize;
unsigned long int amountFreeInodes;
Inode firstFreeInode;
//(Sperrfelder für Liste der freien Blöcke)
//Anzeigefeld, ob Superblock verändert wurde

//FAT
unsigned long int** fileAllocationTable;
//Superblock

//Inodes(Metadaten)
Inode* listFreeInodes;

//Root-Verzeichnis
rootElement* rootVerzeichnis;

//Datenblocks
bool* listFreeBlocks;
string data;

int main(int argc, char *argv[]) {
	for (int i = 0; i < argc; i++) {
		/*ifstream ifs(argv[0]);
		if(!ifs.is_open()){
			cout << "Failed to open" << endl;
		}else{
			cout << "open" << endl;
		}*/
		ofstream o(argv[0]);
		fstream file;
		file.open(argv[0], fstream::out);
		file << "Buuuu ich bin der Pape";
		file.close();

		//prüfe ob gültiger Pfad
		//Lese Datei aus

		// TODO: Implement file system generation & copying of files here
		printf("%s", argv[1]);

		//Schreiben der Container Datei
		return 0;
	}
}
