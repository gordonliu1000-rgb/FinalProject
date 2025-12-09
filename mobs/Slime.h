#ifndef SLIME_H_INCLUDED
#define SLIME_H_INCLUDED
#include <vector>
#include <array>
#include "Mob.h"


class Slime : public Mob
{
public :
    Slime(MobType type) : Mob{type}{
        hp = 100;
        atk = 10;
        speed = 5;
        atk_range_radius = 32;
        init_atk_cool_down   = 120;
        atk_cool_down = 20;
        attack_frame_id = 6;
        int state = static_cast<int>(MobState::WALK);
        bitmap_img_ids[state][0] = 8;
        bitmap_img_ids[state][1] = 8;
        bitmap_img_ids[state][2] = 8;
        bitmap_img_ids[state][3] = 8;
        state = static_cast<int>(MobState::ATK);
        bitmap_img_ids[state][0] = 10;
        bitmap_img_ids[state][1] = 10;
        bitmap_img_ids[state][2] = 10;
        bitmap_img_ids[state][3] = 10;
        state = static_cast<int>(MobState::HURT);
        bitmap_img_ids[state][0] = 5;
        bitmap_img_ids[state][1] = 5;
        bitmap_img_ids[state][2] = 5;
        bitmap_img_ids[state][3] = 5;
        state = static_cast<int>(MobState::DIE);
        bitmap_img_ids[state][0] = 10;
        bitmap_img_ids[state][1] = 10;
        bitmap_img_ids[state][2] = 10;
        bitmap_img_ids[state][3] = 10;
        bitmap_switch_freq = 9;
    }
    void atk_hero();
};

#endif