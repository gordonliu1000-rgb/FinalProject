#ifndef LIGHTBALL_H_INCLUDED
#define LIGHTBALL_H_INCLUDED

#include "Weapon.h"
#include "../algif5/algif.h"

class Lightball : public Weapon {
    public:
        Lightball(float dmg, float radius, float angular_speed);
        void draw() override;
    private:
        ALGIF_ANIMATION *gif = nullptr;
};

#endif 