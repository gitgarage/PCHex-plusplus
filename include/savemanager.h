/*
 * This class provides an interface to abstract a Pokemon Gen VI save file.
 * The class is build with the path of the save file. Note that the savefile 
 * is not automatically loaded upon construction. The actual loading process is done
 * within loadSaveFile() that should be explicitly called.
 * 
 * This class is intended to be the only I/O interface. No file writing and no
 * savefile modification should be done outside this class in order to keep 
 * the multi-layered approach intact. Higher level class should get every 
 * needed information using the savemanager's function.
 * 
 * Also note that no savefile modification is done until the commit() function is
 * called. Every change to the savefile is done in an internal copy of the savefile
 * until commit() is called for safety reasons and to limit the access to disk.
 * 
 * The interface has five public functions:
 * loadSaveFile() - copies the content of a given savefile in RAM
 * getBoxName() - returns a string with the name of the box 
 * getPkmn() - returns a 232 byte array with the decrypted information of a pokemon
 * setPkmn() - writes an encrypted 232 byte array int the savefile's copy in RAM
 * commit() - actually writes the modified savefile 
*/

#ifndef SAVEMANAGER_H
#define	SAVEMANAGER_H

#include <string>
#include <3ds.h>

class savemanager {
    private:
        static const int OFFSET = 0x5400; //the offset to subtract to every address
        static const int XY = 0;
        static const int ORAS = 1;
        
        char* save; //the internal copy of the savefile
        int size; //size of the savefile
        int mode; //XY or ORAS mode
        std::string path; //path of the savefile
        
        void shuffleArray(char* pkmn, const u32 encryptionkey);
        u32 seedStep(const u32 seed);
        void decryptPkmn(char* pkmn);
        void encryptPkmn(char* pkmn);
        int getPkmnAddress(const int boxnumber, const int indexnumber);
        u32 getCHKOffset(const int index);
        u32 getCHKLenght(const int index);
        u16 ccitt16(char* data, const int lenght);
        void calculateChecksum();
        
    public:
        static const int PKMNLENGHT = 232;
        
        savemanager(const std::string p);
        bool loadSaveFile();
        std::string getBoxName(const int boxnumber);
        void getPkmn(const int boxnumber, const int indexnumber, char* pkmn);
        bool getPkmn(std::string path, char* data);
        void setPkmn(const int boxnumber, const int indexnumber, char* pkmn);
        bool setPkmn(std::string path, char* data);
        bool commit();
        ~savemanager();
};

#endif	/* SAVEMANAGER_H */

