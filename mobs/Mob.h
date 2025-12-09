#ifndef MOB_H_INCLUDED
#define MOB_H_INCLUDED
#include "../Object.h"
#include "../shapes/Circle.h"
#include <allegro5/allegro.h>
#include <array>
#include <vector>
#include <memory>

enum class MobState {
    WALK,
    ATK,
    HURT,
    DIE,
    IDLE
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
    static std::unique_ptr<Mob> create_mob(MobType type);
    void draw();
    void update();
    virtual void atk_hero() = 0;
    void hurt(float dmg);
    bool die = false;
private :
    MobType type;
    MobState state = MobState::WALK;
    void dropItem();
protected :
    std::unique_ptr<Circle> atk_range;
    double atk_range_radius;
    int atk_cool_down;//以幀率計算
    int init_atk_cool_down;
    Dir dir = Dir::DOWN;
    bool can_attack = true;
    std::array<std::array<int, 4>, 4> bitmap_img_ids;//the size of pictures in every dir
    int bitmap_switch_counter;
	int bitmap_switch_freq;
	int bitmap_img_id = 0;
    int attack_frame_id;
};


#endif