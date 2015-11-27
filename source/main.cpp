#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <3ds.h>
#include <sf2d.h>
#include <boxviewer.h>
#include <savemanager.h>
#include <gui.h>
#include "GB_ttf.h"

int main()
{
        sf2d_init();
	sf2d_set_clear_color(RGBA8(0xFF, 0x66, 0x66, 0xFF));
        sftd_init();
        sftd_font* font = sftd_load_font_mem(GB_ttf, GB_ttf_size);
        int selected = 0; int boxnumber = 0; int mode = EDITMODE; int selected2 = -1; int boxclone; int indexclone;
        savemanager save("/PCHex++/main");
        save.loadSaveFile();
	
        while (aptMainLoop()) {
            inputhandler(boxnumber, selected, selected2, mode, boxclone, indexclone, &save);
            
            drawtop(&save, font, selected, selected2, boxnumber, mode); 

            sf2d_swapbuffers();
	}
        
        sftd_fini();
        sf2d_fini();
	return 0;
}



