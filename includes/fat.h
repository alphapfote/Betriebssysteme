#ifndef fat_h
#define fat_h

class Fat {
public:

Fat();
~Fat();


//FAT
int getFirstFreeFat();
int getNextDataBlock(int index);
void deleteFileInFat(int index);
void saveNewFileInFat(int index, int size);

};

#endif //fat_h