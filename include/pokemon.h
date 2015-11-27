/*
 * This class provides an interface for a pokemon. It gets constructed with its
 * box and index numbers and it gets automatically loaded from the given savemanager.
 * note that it gets AUTOMATICALLY loaded from savefile and initialized so there is no need for 
 * the initialize() function to be called explicitly.
 * 
 * the constructor asks to the savemanager interface for a 232 byte
 * unencrypted array which contains all the needed information (and is, in fact,
 * the pk6 file associated with a pokemon) and initialize() uses those bytes to initialize all the
 * class parameters. 
 * 
 * After the pokemon has been constructed (and automatically initialized) any getter or
 * setter could be called and they should provide correct information, this structure
 * should be maintained as this is an interface which interacts with the gui.
 * 
 * Note that the actual savefile writing is done inside the SavePkmn() functions which writes
 * to the savemanager and then launches a commit().
 * 
 * IDEA: This class breaks the multi-layer approach in some ways. The pokemon 
 * class shouldn't be aware of the underlying 232 s which define a pokemon
 * but this is an easy and fast solution. This class can be structurally improved
 */

#ifndef POKEMON_H
#define	POKEMON_H

#include <string>
#include <savemanager.h>
#include <3ds.h>

class pokemon {
    private:
        savemanager* save;
        int boxnumber;
        int indexnumber;
        char data[savemanager::PKMNLENGHT];
        std::string nickname;
        u8 nature;
        
        void initialize();
        void savePkmn();
        
    public:
        pokemon(const int boxnum, const int indexnum, savemanager* savem);
        int getBoxNumber() { return boxnumber; }
        int getIndexNumber() { return indexnumber; }
        std::string getNickname() { return nickname; }
        u8 getNature() { return nature; }
        void setNature(const u8 n);
        bool exportPK6(std::string path);
        bool importPK6(std::string path);
        void clone(pokemon& destination);
        ~pokemon();
};

#endif	/* POKEMON_H */

