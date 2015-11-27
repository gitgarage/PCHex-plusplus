#include <gui.h>
#include <boxviewer.h>
#include <sf2d.h>
#include <infobox.h>

void inputhandler(int& boxnumber, int& selected, int& selected2, int& mode, int& boxclone, int& indexclone, savemanager* save) {
    
    
    hidScanInput();
    u32 kdown = hidKeysDown();
    
    if(kdown & KEY_RIGHT && selected < 29 && mode == EDITMODE) selected++;
    if(kdown & KEY_LEFT && selected > 0 && mode == EDITMODE) selected--;
    if(kdown & KEY_DOWN && selected < 27 && mode == EDITMODE) selected = selected + 3;
    if(kdown & KEY_UP && selected > 2 && mode == EDITMODE) selected = selected - 3;
            
    if(kdown & KEY_RIGHT && selected2 < 29 && mode == CLONEMODE) selected2++;
    if(kdown & KEY_LEFT && selected2 > 0 && mode == CLONEMODE) selected2--;
    if(kdown & KEY_DOWN && selected2 < 27 && mode == CLONEMODE) selected2 = selected2 + 3;
    if(kdown & KEY_UP && selected2 > 2 && mode == CLONEMODE) selected2 = selected2 - 3;
            
    if(kdown & KEY_R && boxnumber < 30 && mode == EDITMODE) { boxnumber++; selected = 0; }
    if(kdown & KEY_L && boxnumber > 0 && mode == EDITMODE) { boxnumber--; selected = 0; }
            
    if(kdown & KEY_R && boxnumber < 30 && mode == CLONEMODE) { boxnumber++; selected2 = 0; }
    if(kdown & KEY_L && boxnumber > 0 && mode == CLONEMODE) { boxnumber--; selected2 = 0; }
            
    if(kdown & KEY_X && mode == EDITMODE) { 
        boxclone = boxnumber; 
        indexclone = selected;  
        mode = CLONEMODE; 
        selected2 = selected;
        selected = -1; 
    }
            
    if(kdown & KEY_A && mode == CLONEMODE) { 
        pokemon toclone(boxclone, indexclone, save); 
        pokemon destination(boxnumber, selected2, save); 
        toclone.clone(destination); 
        mode = EDITMODE;
        selected = selected2;
        selected2 = -1;
    }
}

void drawtop(savemanager* s, sftd_font* f, const int selected, const int selected2, const int boxnumber, const int mode) {
    sf2d_start_frame(GFX_TOP, GFX_LEFT);
    sf2d_texture* temp;
    
    sftd_draw_text(f, 20, 6,  RGBA8(0, 0, 0, 255), 10, s->getBoxName(boxnumber).c_str());
    
    infobox infol(temp, f, 0, 240-(infobox::HEIGHT), "L", s->getBoxName(boxnumber-1));
    infobox infor(temp, f, 400-(infobox::WIDTH), 240-(infobox::HEIGHT), "R", s->getBoxName(boxnumber+1));
    infobox infox(temp, f, 400-(infobox::WIDTH), 0, "X", "CLONE");
    infobox infoa(temp, f, 400-(infobox::WIDTH*2)-10, 0, "A", "EDIT");
    infobox infoaclone(temp, f, 400-(infobox::WIDTH*2)-10, 0, "A", "SELECT");
    
    if(boxnumber > 0) infol.draw();
    if(boxnumber < 30) infor.draw();
    if(mode == EDITMODE) infox.draw();
    if(mode == EDITMODE) infoa.draw();
    if(mode == CLONEMODE) infoaclone.draw();
    
    for(int i = 0; i < 15; i++) {
        int j = 0; int tempselected = selected; int tempselected2 = selected2;
        if(selected >= 15) { j  = 15; tempselected = selected % 15; }
        if(selected2 >= 15) { j  = 15; tempselected2 = selected2 % 15; }
        
        pokemon pika(boxnumber, j+i, s);
        boxviewer box(temp, f, i, tempselected, tempselected2, &pika);
        box.draw();
    }
		
    sf2d_end_frame();
}

