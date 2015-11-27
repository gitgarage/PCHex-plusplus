#include <fstream>
#include <string>
#include <cstring>
#include <savemanager.h>
#include <3ds.h>

savemanager::savemanager(const std::string p) {
    path = p;
}

bool savemanager::loadSaveFile() {
    const int XYSAVEFILELENGHT = 0x65600;
    const int ORASSAVEFILELENGHT = 0X76000;
    
    //Loads the savefile into ram and gets its size
    std::ifstream savefile(path, std::ios::in | std::ios::binary | std::ios::ate);
    size = savefile.tellg();
    save = new char[size];
    savefile.seekg(0, std::ios::beg);
    savefile.read(save, size);
    savefile.close();
    
    //Check whetever the file is a XY, ORAS or INVALID savefile
    if(size == XYSAVEFILELENGHT) mode = XY;
    else if(size == ORASSAVEFILELENGHT) mode = ORAS;
    else return 0;
    
    //Writes a backup of the savefile
    std::ofstream backup("/PCHex++/backup", std::ios::out | std::ios::binary);
    backup.write(save, size);
    backup.close();
    
    return savefile;
}

std::string savemanager::getBoxName(const int boxnumber) {
    const int BOXPOSITION = 0x9800 - OFFSET;
    const int BOXLENGTH = 34;
    
    //Effectively gets the box name
    char* temp = new char[BOXLENGTH]; 
    memcpy(temp, &save[BOXPOSITION + (boxnumber * BOXLENGTH)], BOXLENGTH);
    
    //Not so elegant trick to fix the unusual way the name are being stored
    char* name = new char[BOXLENGTH];
    for(int i = 0; i < BOXLENGTH/2; i++)
        name[i] = temp[i*2];
    
    name[BOXLENGTH/2] = 0;
    
    //returns the name
    std::string toreturn(name);
    delete temp;
    delete name;
    return toreturn;
    
}

void savemanager::shuffleArray(char* pkmn, const u32 encryptionkey) {
    //Function needed to decrypt and encrypt Pokemon. See the PKHex's source code for more references
    const int BLOCKLENGHT = 56;
    
    u8 seed = (((encryptionkey & 0x3E000) >> 0xD) % 24);
    
    int aloc[24] = { 0, 0, 0, 0, 0, 0, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3, 1, 1, 2, 3, 2, 3 };
    int bloc[24] = { 1, 1, 2, 3, 2, 3, 0, 0, 0, 0, 0, 0, 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2 };
    int cloc[24] = { 2, 3, 1, 1, 3, 2, 2, 3, 1, 1, 3, 2, 0, 0, 0, 0, 0, 0, 3, 2, 3, 2, 1, 1 };
    int dloc[24] = { 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 3, 2, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0 };
    int ord[4] = {aloc[seed], bloc[seed], cloc[seed], dloc[seed]};
    
    char pkmncpy[PKMNLENGHT];
    char tmp[BLOCKLENGHT];

    memcpy(&pkmncpy, pkmn, PKMNLENGHT);
    
    for (int i = 0; i < 4; i++) {
        memcpy(tmp, pkmncpy + 8 + BLOCKLENGHT * ord[i], BLOCKLENGHT);
        memcpy(pkmn + 8 + BLOCKLENGHT * i, tmp, 56);
    }
}

u32 savemanager::seedStep(const u32 seed) {
    //Function needed to calculate the seed for encrypt and decrypt pokemon. PKHex's source code for more references
    return (seed*0x41C64E6D + 0x00006073) & 0xFFFFFFFF;
}

void savemanager::decryptPkmn(char* pkmn) {
    //Alghoritm to decrypt a pokemon, google and PKHex's source code for more references
    const int ENCRYPTIONKEYPOS = 0x0;
    const int ENCRYPTIONKEYLENGHT = 4;
    const int CRYPTEDAREAPOS = 0x08;
    
    u32 encryptionkey;
    memcpy(&encryptionkey, &pkmn[ENCRYPTIONKEYPOS], ENCRYPTIONKEYLENGHT);
    u32 seed = encryptionkey;
    
    u16 temp;
    for(int i = CRYPTEDAREAPOS; i < PKMNLENGHT; i = i+2) {
        memcpy(&temp, &pkmn[i], 2);
        temp ^= (seedStep(seed) >> 16);
        seed = seedStep(seed);
        memcpy(&pkmn[i], &temp, 2);
    }
    
    shuffleArray(pkmn, encryptionkey);
}

void savemanager::encryptPkmn(char* pkmn) {
    //Alghoritm to encrypt pokemon, google and PKHex's source code for more references
    const int ENCRYPTIONKEYPOS = 0x0;
    const int ENCRYPTIONKEYLENGHT = 4;
    const int CRYPTEDAREAPOS = 0x08;
    
    u32 encryptionkey;
    memcpy(&encryptionkey, &pkmn[ENCRYPTIONKEYPOS], ENCRYPTIONKEYLENGHT);
    u32 seed = encryptionkey;
    
    for(int i = 0; i < 11; i++)
        shuffleArray(pkmn, encryptionkey);
        
    u16 temp;
    for(int i = CRYPTEDAREAPOS; i < PKMNLENGHT; i = i+2) {
        memcpy(&temp, &pkmn[i], 2);
        temp ^= (seedStep(seed) >> 16);
        seed = seedStep(seed);
        memcpy(&pkmn[i], &temp, 2);
    }
}
    
int savemanager::getPkmnAddress(const int boxnumber, const int indexnumber) {
    //given its box and index numbers alculates the physical address where a pokemon is stored in the savefile
    int boxpos;
    if(mode == XY) boxpos = 0x27A00 - OFFSET;
    if(mode == ORAS) boxpos = 0x38400 - OFFSET;
    
    const int PKMNNUM = 30;
    
    return boxpos + (PKMNLENGHT * PKMNNUM * boxnumber) + (indexnumber * PKMNLENGHT);
}

u32 savemanager::getCHKOffset(const int index) {
    //Function needed to recalculate the checksum of the savefile, PKHex's source code for references
    //IDEA: Why it is really needed? Why the checksum can't be calculated on the entire savefile without
    //this tacky and inelegant gimmick? I sticked with it because it is proved to work.
    //I think that there is room to improve the elegance of this
    u32 startoras[] = { 0x05400, 0x05800, 0x06400, 0x06600, 0x06800, 0x06A00, 0x06C00, 0x06E00, 0x07000, 0x07200, 0x07400, 0x09600, 0x09800, 0x09E00, 0x0A400, 0x0F400, 0x14400, 0x19400, 0x19600, 0x19E00, 0x1A400, 0x1B600, 0x1BE00, 0x1C000, 0x1C200, 0x1C800, 0x1CA00, 0x1CE00, 0x1D600, 0x1D800, 0x1DA00, 0x1DC00, 0x1DE00, 0x1E000, 0x1E800, 0x1EE00, 0x1F200, 0x20E00, 0x21000, 0x21400, 0x21800, 0x22000, 0x23C00, 0x24000, 0x24800, 0x24C00, 0x25600, 0x25A00, 0x26200, 0x27000, 0x27200, 0x27400, 0x28200, 0x28A00, 0x28E00, 0x30A00, 0x38400, 0x6D000, };
    u32 startxy[] = { 0x05400, 0x05800, 0x06400, 0x06600, 0x06800, 0x06A00, 0x06C00, 0x06E00, 0x07000, 0x07200, 0x07400, 0x09600, 0x09800, 0x09E00, 0x0A400, 0x0F400, 0x14400, 0x19400, 0x19600, 0x19E00, 0x1A400, 0x1AC00, 0x1B400, 0x1B600, 0x1B800, 0x1BE00, 0x1C000, 0x1C400, 0x1CC00, 0x1CE00, 0x1D000, 0x1D200, 0x1D400, 0x1D600, 0x1DE00, 0x1E400, 0x1E800, 0x20400, 0x20600, 0x20800, 0x20C00, 0x21000, 0x22C00, 0x23000, 0x23800, 0x23C00, 0x24600, 0x24A00, 0x25200, 0x26000, 0x26200, 0x26400, 0x27200, 0x27A00, 0x5C600, };
    
    if(mode == ORAS) return startoras[index] - OFFSET;
    if(mode == XY) return startxy[index] - OFFSET;
    
    return 0;
}

u32 savemanager::getCHKLenght(const int index) {
    //Function needed to recalculate the checksum of the savefile, PKHex's source code for references
    //IDEA: Why it is really needed? Why the checksum can't be calculated on the entire savefile without
    //this tacky and inelegant gimmick? I sticked with it because it is proved to work.
    //I think that there is room to improve the elegance of this
    u32 lengthoras[] = { 0x000002C8, 0x00000B90, 0x0000002C, 0x00000038, 0x00000150, 0x00000004, 0x00000008, 0x000001C0, 0x000000BE, 0x00000024, 0x00002100, 0x00000130, 0x00000440, 0x00000574, 0x00004E28, 0x00004E28, 0x00004E28, 0x00000170, 0x0000061C, 0x00000504, 0x000011CC, 0x00000644, 0x00000104, 0x00000004, 0x00000420, 0x00000064, 0x000003F0, 0x0000070C, 0x00000180, 0x00000004, 0x0000000C, 0x00000048, 0x00000054, 0x00000644, 0x000005C8, 0x000002F8, 0x00001B40, 0x000001F4, 0x000003E0, 0x00000216, 0x00000640, 0x00001A90, 0x00000400, 0x00000618, 0x0000025C, 0x00000834, 0x00000318, 0x000007D0, 0x00000C48, 0x00000078, 0x00000200, 0x00000C84, 0x00000628, 0x00000400, 0x00007AD0, 0x000078B0, 0x00034AD0, 0x0000E058, };
    u32 lengthxy[] = { 0x000002C8, 0x00000B88, 0x0000002C, 0x00000038, 0x00000150, 0x00000004, 0x00000008, 0x000001C0, 0x000000BE, 0x00000024, 0x00002100, 0x00000140, 0x00000440, 0x00000574, 0x00004E28, 0x00004E28, 0x00004E28, 0x00000170, 0x0000061C, 0x00000504, 0x000006A0, 0x00000644, 0x00000104, 0x00000004, 0x00000420, 0x00000064, 0x000003F0, 0x0000070C, 0x00000180, 0x00000004, 0x0000000C, 0x00000048, 0x00000054, 0x00000644, 0x000005C8, 0x000002F8, 0x00001B40, 0x000001F4, 0x000001F0, 0x00000216, 0x00000390, 0x00001A90, 0x00000308, 0x00000618, 0x0000025C, 0x00000834, 0x00000318, 0x000007D0, 0x00000C48, 0x00000078, 0x00000200, 0x00000C84, 0x00000628, 0x00034AD0, 0x0000E058, };
    
    if(mode == ORAS) return lengthoras[index];
    if(mode == XY) return lengthxy[index];
    
    return 0;
}

u16 savemanager::ccitt16(char* data, const int size) {
  //ccitt16 alghoritm to recalculate checksum, wikipedia for more references
  u16 crc = 0xFFFF;
  
  for (int i = 0; i < size; i++) {
    crc ^= (u16)data[i] << 8;
    for (int j = 0; j < 8; j++)
        if (crc & 0x8000)
            crc = crc << 1 ^ 0x1021;
        else
            crc = crc << 1;
    }
  
    return crc;
}

void savemanager::calculateChecksum() {
    //Alghoritm to recalculate the checksum, PKHex's source code for references
    int blocknumber;
    if(mode == ORAS) blocknumber = 58;
    if(mode == XY) blocknumber = 55;
    
    int checksumpos;
    if(mode == ORAS) checksumpos = 0x7B21A - OFFSET;
    if(mode == XY) checksumpos = 0x6A81A - OFFSET;
    
    char* temp = new char[0x35000];
    u16 checksum;
    
    for(int i = 0; i < blocknumber; i++) {
        memcpy(temp, &save[getCHKOffset(i)], getCHKLenght(i));
        checksum = ccitt16(temp, getCHKLenght(i));
        memcpy(&save[checksumpos + (i*8)], &checksum, 2);
    }
    
    delete [] temp;
}

void savemanager::getPkmn(const int boxnumber, const int indexnumber, char* pkmn) {
    //Reads a given pokemon from the savefile and decrypts it
    memcpy(pkmn, &save[getPkmnAddress(boxnumber, indexnumber)], PKMNLENGHT);
    decryptPkmn(pkmn);
}

bool savemanager::getPkmn(std::string path, char* data) {
    //Reads a pk6 file and transfers its content to data 
    std::ifstream file(path, std::ios::in | std::ios::binary);
    file.read(data, PKMNLENGHT);
    file.close();
    return file;
}

void savemanager::setPkmn(const int boxnumber, const int indexnumber, char* pkmn) {
    //encrypts and write a pokemon into THE INTERNAL COPY OF THE SAVEFILE
    encryptPkmn(pkmn);
    memcpy(&save[getPkmnAddress(boxnumber, indexnumber)], pkmn, PKMNLENGHT);
}

bool savemanager::setPkmn(std::string path, char* data) {
    //Writes a pk6 file from data 
    std::ofstream file(path, std::ios::in | std::ios::binary);
    file.write(data, PKMNLENGHT);
    file.close();
    return file;
}

bool savemanager::commit() {
    //effectively writes the modified savefile. This function is heavy and obnoxious, it should be called only when needed
    calculateChecksum();
    std::ofstream savefile(path, std::ios::out | std::ios::binary);
    savefile.write(save, size);
    savefile.close();
    
    return savefile;
}

savemanager::~savemanager() {
    //I know i should delete this enormous pointer but it caused a bug to appear so it stays commented
    //The bug has been resolved but i don't want to open a wormhole at this stage of development
    //Is not really a problem anyway because the savemanager gets distructed only at the end of the program causing no memory leak
    //delete [] save;
}