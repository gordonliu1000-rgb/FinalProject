#ifndef ENERMY_SPELL_H_INCLUDED
#define ENERMY_SPELL_H_INCLUDED

#include "../Object.h"

enum class EnermySpellType{
    FIREBALL, LASER
};

class EnermySpell : public Object{
public:
    // 有不同的type，激光、火球...  
    static std::unique_ptr<EnermySpell> create_spell(Object *shooter, const double &atk, EnermySpellType type);
    static void init();
public:
    Object *shooter;
    double atk;
    EnermySpellType type;
    bool end = false;
    virtual void update() = 0;
    EnermySpell(Object *shooter, const double &atk, EnermySpellType type) 
    : shooter{shooter}, atk{atk}, type{type} {};
protected:
    int bitmap_switch_counter;
	int bitmap_switch_freq;
	int bitmap_img_id = 0;
    
    
    
    
};

#endif