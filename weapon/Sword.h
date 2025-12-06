#ifndef SWORD_H_INCLUDED
#define SWORD_H_INCLUDED

#include "Weapon.h"
#include "../data/ImageCenter.h"
#include <allegro5/allegro.h>

class Sword: public Weapon{
    public:
        Sword(float get_dmg, float radius, float angular_speed);
        float get(float get_dmg);
        void draw() override;
    
    private:
        ALLEGRO_BITMAP *bitmap = nullptr;
};

#endif