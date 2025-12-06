#ifndef MOB_H_INCLUDED
#define MOB_H_INCLUDED
#include "../Object.h"

enum class State;
enum class Dir;

enum class MobType {
    SLIME, MOBTYPE_MAX
};


class Mob : public Object{
public :
    float hp;
    float atk;
    float speed;
    
    Mob(MobType type);
    static Mob *create_mob(MobType type);
    virtual void draw() = 0;
    void update();
    virtual void atk_hero() = 0;
private :
    std::unique_ptr<Circle> atk_range;
    double atk_range_radius;
    MobType type;
    State state;
    Dir dir;
    int atk_cool_down;//以幀率計算
    int init_atk_cool_down;

};




#endif