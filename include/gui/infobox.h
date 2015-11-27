#ifndef INFOBOX_H
#define INFOBOX_H

#include <sftd.h>
#include <sf2d.h>
#include <string>

class infobox {
    private:
        int x;
        int y;
        sf2d_texture* tex;
        sftd_font* font;
        std::string button;
        std::string help;
        
    public:
        static const int WIDTH = 110;
        static const int HEIGHT = 15;
        infobox(sf2d_texture* t, sftd_font* f, const int xpos, const int ypos, const std::string b, const std::string h);
        void draw();
};

#endif /* INFOBOX_H */

