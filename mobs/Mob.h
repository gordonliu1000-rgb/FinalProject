#ifndef MOB_H_INCLUDED
#define MOB_H_INCLUDED
#include "../Object.h"
#include <allegro5/allegro.h>
#include <array>
#include <vector>

enum class State {
    WALK,
    ATK,
    HURT,
    DIE
};

enum class Dir{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class MobType {
    SLIME, MOBTYPE_MAX
};


class Mob : public Object{
public :
    float hp;
    float atk;
    float speed;
    
    Mob(MobType type);
    ~Mob(){}
    static Mob *create_mob(MobType type);
    void draw();
    void update();
    virtual void atk_hero() = 0;
    void hurt(float dmg);
private :
    MobType type;
    State state = State::WALK;
protected :
    std::unique_ptr<Circle> atk_range;
    double atk_range_radius;
    int atk_cool_down;//以幀率計算
    int init_atk_cool_down;
    Dir dir = Dir::DOWN;
    bool can_attack = true;
    std::array<std::vector<std::vector<int>>, 4> bitmap_img_ids;
    std::map<HeroState, std::string> gifPath;
    int bitmap_switch_counter;
	int bitmap_switch_freq;
	int bitmap_img_id;
};


#endif