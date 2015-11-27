#ifndef DRAWTOP_H
#define DRAWTOP_H

const int EDITMODE = 0;
const int CLONEMODE = 1;

#include <savemanager.h>
#include <sftd.h>

void inputhandler(int& boxnumber, int& selected, int& selected2, int& mode, int& boxclone, int& indexclone, savemanager* save);
void drawtop(savemanager* s, sftd_font* f, const int selected, const int selected2, const int boxnumber, const int mode);

#endif /* DRAWTOP_H */

