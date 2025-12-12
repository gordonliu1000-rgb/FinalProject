#ifndef HEROSPELL_H_INCLUDED
#define HEROSPELL_H_INCLUDED
#include "Spell.h"


class HeroSpell : Spell{
public :
    int cooldown;// 幀
    int init_cooldown;
    void draw();
};



#endif