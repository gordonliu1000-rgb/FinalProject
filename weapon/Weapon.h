#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include "../Object.h"

class Hero;

class Weapon : public Object{
    public:
        Weapon(float dmg, float radius, float angular_speed) : 
            dmg(dmg), radius(radius), angular_speed(angular_speed), angle(0.0f) {}
        virtual ~Weapon() {}  
        virtual void update(const Hero &hero, float dt);
        virtual float get_dmg() {return dmg;}
    
    protected:
        float dmg; 
        float radius;
        float angular_speed;
        float angle;
};

#endif