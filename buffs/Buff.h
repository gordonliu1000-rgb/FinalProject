#ifndef BUFFS_H_INCLUDED
#define BUFFS_H_INCLUDED
#include <allegro5/allegro.h>
#include "../Object.h"
#include "../shapes/Point.h"

class Hero;
class DenterCenter;

enum class BuffType{
    SPEED,
    POWER,
    HEAL,
    ABSORB,
    INVINCIBLE,
    REVIVE,
    BUFFTYPE_MAX
};


class Buff{
public:
    static Buff* create_buff(BuffType type);
    virtual void reset_duration();
    Buff(BuffType type);
    virtual ~Buff() {al_destroy_bitmap(icon);}
    BuffType get_type() const {return type;}
    void update(); // 更新持續時間
    ALLEGRO_BITMAP *get_icon() {return icon;} 
    void draw_icon();
    int duration; // 持續時間(幀數)
    
    virtual void effect() = 0; // 各自的效果
    virtual void clear_effect() = 0;
protected:
    BuffType type;
    ALLEGRO_BITMAP *icon;
    int init_duration; // 初始持續時間(幀數)
    
};  

#endif