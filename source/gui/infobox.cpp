#include <infobox.h>
#include <sf2d.h>
#include <sftd.h>

infobox::infobox(sf2d_texture* t, sftd_font* f, const int xpos, const int ypos, const std::string b, const std::string h) {
    tex = t;
    font = f;
    x = xpos;
    y = ypos;
    button = b;
    help = h;
}

void infobox::draw() {
    sf2d_draw_rectangle(x, y, WIDTH, HEIGHT, RGBA8(0x99, 0xCC, 0xFF, 0xFF));
    sftd_draw_text(font, x+5, y+5,  RGBA8(0, 0, 0, 255), 6, (button+": "+help).c_str());
}