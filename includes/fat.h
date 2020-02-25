#ifndef fat_h
#define fat_h

class Fat {

Fat();
~Fat();

public:

//FAT
int getFirstFreeFat();
int getNextDataBlock(int index);
void deleteFileInFat(int index);
void saveNewFileInFat(int index, int size);

};

#endif //fat_h