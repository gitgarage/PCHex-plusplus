#include <pokemon.h>
#include <savemanager.h>
#include <cstring>
#include <string>
#include <3ds.h>

pokemon::pokemon(const int boxnum, const int indexnum, savemanager* savem) {
    boxnumber = boxnum;
    indexnumber = indexnum;
    save = savem;
    loadFromSave();
}

void pokemon::loadFromSave() {
    //const with positions and sizes of every needed parameter
    const int NICKNAMEPOS = 0x40;
    const int NICKNAMELENGTH = 23;
    const int NATUREPOS = 0x1C;
    const int NATURELENGTH = 1;
    
    //gets the 232 byte bytes of a pokemon from the savefile
    save->getPkmn(boxnumber, indexnumber, data);
    
    //initializes the nickname of the pokemon instance
    char* temp = new char[NICKNAMELENGTH];
    char* nick = new char[NICKNAMELENGTH];
    memcpy(temp, &data[NICKNAMEPOS], NICKNAMELENGTH);
    
    //inelegant trick to fix the way the game unnaturally handles string
    for(int i = 0; i < NICKNAMELENGTH/2; i++)
        nick[i] = temp[i*2];
    
    nick[NICKNAMELENGTH/2] = 0;
    nickname.assign(nick);
    
    //initialize the nature of the pokemon instance
    memcpy(&nature, &data[NATUREPOS], NATURELENGTH);
}

void pokemon::setNature(const u8 n) {
    const int NATUREPOS = 0x1C;
    const int NATURELENGHT = 1;
    
    nature = n;
    memcpy(&data[NATUREPOS], &n, NATURELENGHT);
    savePkmn();
}

void pokemon::clone(pokemon& destination) {
    save->setPkmn(destination.getBoxNumber(), destination.getIndexNumber(), data);
    save->commit();
    destination = *this;
}

void pokemon::savePkmn() {
    save->setPkmn(boxnumber, indexnumber, data);
    save->commit();
}

pokemon::~pokemon() {
    
}