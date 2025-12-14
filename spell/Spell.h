#ifndef SPELL_H
#define SPELL_H
#include "../Object.h"
#include <allegro5/allegro.h>
#include <vector>


enum class SpellType{
    LAZER, THUNDER, FIRE_BALL, SPELL_TYPE_MAX
};


class Spell : public Object{
public:
    SpellType type;

    int bitmap_switch_counter;
	int bitmap_switch_freq;
	int bitmap_img_id = 0;
    double atk;
    int cooldown = 0;// 幀
    int init_cooldown;
    int level;
    virtual void update() = 0;
    Spell(SpellType type) : type{type}{};
    static void init();
    static std::unique_ptr<Spell> create_spell(SpellType type);
};


#endif