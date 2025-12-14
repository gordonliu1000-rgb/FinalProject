#ifndef THUNDER_H_INCLUDED
#define THUNDER_H_INCLUDED
#include "Spell.h"
#include "../shapes/Circle.h"



class Strike{
public:
    bool end = false;
    int wait_time;
    int bitmap_img_id = 0;
    int bitmap_switch_counter;
    int bitmap_switch_freq = 5;
    int attack_img_id = 5;
    double atk;
    bool can_atk = true;
    Strike(double x, double y, double r, int wait_time, double atk);
    void update();
    std::unique_ptr<Shape> shape;
};


class Thunder : public Spell{    
public:
    Thunder(SpellType type);
    void update();
    void draw();
    double atk_radius = 200;
    int num_of_strikes;
    double strike_radius;
    std::vector<std::unique_ptr<Strike>> strikes;
    static void init_img();
    static std::vector<ALLEGRO_BITMAP*> img;
    
};






#endif