#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

#include "../Object.h"

class Hero;

class Weapon : public Object{
    public:
        Weapon(float radius, float angular_speed) : 
            radius(radius), angular_speed(angular_speed), angle(0.0f) {}
        virtual ~Weapon() {}  
        virtual void update(const Hero &hero, float dt);
        virtual float get_dmg() {return dmg;}
        //bool can_hit() const {return cooldown <= 0.0;}
        //void reset_cooldown() {cooldown = hit_interval;}
    
    protected:
        float dmg; 
        float radius;
        float angular_speed;
        float angle;
        //float cooldown = 0.0;
        //float hit_interval = 0.;
};

#endif