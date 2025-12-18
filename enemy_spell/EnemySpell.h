#ifndef ENEMY_SPELL_H_INCLUDED
#define ENEMY_SPELL_H_INCLUDED

#include "../Object.h"

enum class EnemySpellType{
    FIREBALL, LASER
};

class EnemySpell : public Object{
public:
    // 有不同的type，激光、火球...  
    static std::unique_ptr<EnemySpell> create_spell(Object *shooter, const double &atk, EnemySpellType type);
    static void init();
public:
    Object *shooter;
    double atk;
    EnemySpellType type;
    bool end = false;
    virtual void update() = 0;
    EnemySpell(Object *shooter, const double &atk, EnemySpellType type) 
    : shooter{shooter}, atk{atk}, type{type} {};
protected:
    int bitmap_switch_counter;
	int bitmap_switch_freq;
	int bitmap_img_id = 0;
    
    
    
    
};

#endif