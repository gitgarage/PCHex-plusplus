/*
 * This class provides an interface for a pokemon. It gets constructed with its
 * box and index numbers and it gets automatically loaded from the given savemanager.
 * note that it gets AUTOMATICALLY loaded from savefile and there is no need for 
 * the loadFromSave() function to be called explicitly.
 * 
 * the loadFromSave() function asks to the savemanager interface for a 232 byte
 * unencrypted array which contains all the needed information (and is, in fact,
 * the pk6 file associated with a pokemon) and uses it to initialize all the
 * class parameters. 
 * 
 * After the pokemon has been constructed (and automatically loaded) any getter or
 * setter could be called and they should provide correct information, this structure
 * should be maintained as this is an interface which interacts with the gui.
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
        
        void loadFromSave();
        void savePkmn();
        
    public:
        pokemon(const int boxnum, const int indexnum, savemanager* savem);
        int getBoxNumber() { return boxnumber; }
        int getIndexNumber() { return indexnumber; }
        std::string getNickname() { return nickname; }
        u8 getNature() { return nature; }
        void setNature(const u8 n);
        void clone(pokemon& destination);
        ~pokemon();
};

#endif	/* POKEMON_H */

