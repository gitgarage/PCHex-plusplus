#include <boxviewer.h>
#include <sftd.h>
#include <3ds.h>
#include <sf2d.h>

boxviewer::boxviewer(sf2d_texture* i, sftd_font* f, const int number, const int s, const int s2, pokemon* p) {
    img = i;
    pika = p;
    font = f;
    if(number == s) selected = true;
    else selected = false;
    if(number == s2) selected2 = true;
    else selected2 = false;
    const int STARTOFFSET = 25;
    const int YDISTANCE = 5;
    const int XDISTANCE = 15;
    const int NUMBERROW = 5;
    const int NUMBERCOLUMN = 3;
    
    int row = number / NUMBERCOLUMN;
    int column = number % NUMBERCOLUMN;
    
    x = STARTOFFSET + (XDISTANCE*column) + (WIDTH*column);
    y = STARTOFFSET + (YDISTANCE*row) + (HEIGHT*row);
}

void boxviewer::draw() {
    if(selected) sf2d_draw_rectangle(x, y, WIDTH, HEIGHT, RGBA8(0x33, 0x99, 0xFF, 0xFF));
    else if(selected2) sf2d_draw_rectangle(x, y, WIDTH, HEIGHT, RGBA8(0x99, 0xFF, 0x33, 0xFF));
    else sf2d_draw_rectangle(x, y, WIDTH, HEIGHT, RGBA8(0x99, 0xCC, 0xFF, 0xFF));
    sftd_draw_text(font, x+5, y+10,  RGBA8(0, 0, 0, 255), 8, pika->getNickname().c_str());
}