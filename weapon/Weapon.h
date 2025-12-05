#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include "../Object.h"

class Hero;

class Weapon : public Object{
    public:
        Weapon(float radius, float angular_speed) : radius(radius), angular_speed(angular_speed),
            angle(0.0f) {}
        virtual ~Weapon() {}  
        virtual void update(const Hero &hero, float dt);
    
    protected:
        float radius;
        float angular_speed;
        float angle;
        
};

#endif