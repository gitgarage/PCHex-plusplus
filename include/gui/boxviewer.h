#ifndef BOXVIEWER_H
#define BOXVIEWER_H

#include <sf2d.h>
#include <sftd.h>
#include <pokemon.h>

class boxviewer {
    private:
        sf2d_texture* img;
        sftd_font* font;
        pokemon* pika;
        bool selected;
        bool selected2;
        int x;
        int y;
      
    public:
        static const int WIDTH = 108;
        static const int HEIGHT = 34;
        boxviewer(sf2d_texture* i, sftd_font* f, const int number, const int s, const int s2, pokemon* p);
        void draw();
};

#endif /* BOXVIEWER_H */

