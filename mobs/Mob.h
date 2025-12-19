#ifndef MOB_H_INCLUDED
#define MOB_H_INCLUDED
#include "../Object.h"
#include "../shapes/Circle.h"
#include <allegro5/allegro.h>
#include <vector>
#include <memory>
#include <map>
#include "../data/ImageCenter.h"
#include "../Creature.h"

enum class MobType {
    SLIME1, SLIME2, FLOWER, VAMPIRE, MOBTYPE_MAX
};

namespace MobSetting {
    static constexpr char mob_imgs_root_path[static_cast<size_t>(MobType::MOBTYPE_MAX)][50] = {
        "./assets/image/mob/slime1",
        "./assets/image/mob/slime2",
        "./assets/image/mob/flower1",
        "./assets/image/mob/vampire1"
    };
    static constexpr char state_path_prefix[][10] = {
        "walk", "atk", "hurt", "die", "idle"
    };
    static constexpr char dir_path_prefix[][10] = {
        "up", "down", "left", "right"
    };
    constexpr char weapon_hit_sound_path[] = "./assets/sound/Hit.ogg";
}

enum class MobState {
    WALK,
    ATK,
    HURT,
    DIE,
    IDLE
};

enum class MobDir{
    UP,
    DOWN,
    LEFT,
    RIGHT
};




class Mob : public Creature{
public:
    static void init();
    static void reset();
    static void mob_level_up();
public :
    float hp;
    float atk;
    float speed;
    
    Mob(MobType type);
    ~Mob(){}
    static std::unique_ptr<Mob> create_mob(MobType type);
    void draw();
    virtual void update();
    virtual void atk_hero() = 0;
    void hurt(float dmg);
    bool die = false;
    // bool swapped = false;
    bool explosive = false;
private :
    MobType type;
    void dropItem();
    void chase_hero();
    void calculate_distance();
    float distance = 1;
    int update_distance_cooldown = 5;
    int update_distance_counter = 0;
protected :
    MobState state = MobState::WALK;
    std::unique_ptr<Circle> atk_range;
    double atk_range_radius;
    int atk_cool_down;//以幀率計算
    int init_atk_cool_down;
    MobDir dir = MobDir::DOWN;
    bool can_attack = true;
    int bitmap_switch_counter;
	int bitmap_switch_freq;
	int bitmap_img_id = 0;
    int attack_frame_id;
    int hurt_init_cooldown = 30;
    int hurt_cooldown = 0;
    
    virtual ALLEGRO_BITMAP* get_bitmap(int bitmap_id) = 0;
    virtual int get_bitmaps_last_idx(MobState state) = 0;

};


#endif