#ifndef ENEMY_SPELL_FIREBALL_H_INCLUDED
#define ENEMY_SPELL_FIREBALL_H_INCLUDED
#include "EnemySpell.h"
#include <allegro5/allegro.h>
#include <vector>
#include <map>

class FireBall : public EnemySpell{
public:
    enum class State{
        FLYING, EXPLODE
    };
    
    State state = State::FLYING;
public:
    static std::map<State, std::vector<ALLEGRO_BITMAP*>> img;
    static void init_img();
public:
    FireBall(Object *shooter, const double &atk, EnemySpellType type);
    void update();
    void draw();
    double speed = 100;
    double bitmap_angle = 0;
    float dir_x, dir_y; 
    std::unique_ptr<Point> des;
};

#endif