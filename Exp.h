#ifndef EXP_H_INCLUDED
#define EXP_H_INCLUDED
#include "Object.h"
#include <allegro5/allegro.h>

class Exp : public Object{
public:
    static void init_img();
public:
    bool picked = false;
    Exp(double x, double y);
    const int get_val() const{return val;}
    void draw();
private:
    int val = 20;
    double w, h;
    static ALLEGRO_BITMAP* img;
};



#endif