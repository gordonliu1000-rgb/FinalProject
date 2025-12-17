#ifndef CREATURE_H_INCLUDED
#define CREATURE_H_INCLUDED
#include "Object.h"

enum class CreatureType{
    MOB, HERO
};

class Creature : public Object{
public:
    virtual void hurt(float dmg) = 0;
};


#endif