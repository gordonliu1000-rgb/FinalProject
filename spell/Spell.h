#ifndef SPELL_H
#define SPELL_H
#include "../Object.h"
#include <allegro5/allegro.h>
#include <vector>

enum class UserType{
    ENEMY, HERO
};

enum class SpellType{
    LAZER, LIGHTNING, FIRE_BALL
};


class Spell : public Object{
public:
    UserType user_type;
    SpellType type;
    std::vector<ALLEGRO_BITMAP*> img;
    int bitmap_switch_counter;
	int bitmap_switch_freq;
	int bitmap_img_id = 0;
    int attack_frame_id;
    double atk;
    virtual void update() = 0;
};


#endif