#ifndef ENERMY_SPELL_FIREBALL_H_INCLUDED
#define ENERMY_SPELL_FIREBALL_H_INCLUDED
#include "EnermySpell.h"
#include <allegro5/allegro.h>
#include <vector>
#include <map>

class FireBall : public EnermySpell{
public:
    enum class State{
        FLYING, EXPLODE
    };
    
    State state = State::FLYING;
public:
    static std::map<State, std::vector<ALLEGRO_BITMAP*>> img;
    static void init_img();
public:
    FireBall(Object *shooter, const double &atk, EnermySpellType type);
    void update();
    void draw();
    double speed = 30;
    double bitmap_angle = 0;
    float dir_x, dir_y; 
    std::unique_ptr<Point> des;
};

#endif