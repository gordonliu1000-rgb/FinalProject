#ifndef SLIME_H_INCLUDED
#define SLIME_H_INCLUDED
#include <vector>
#include <array>
#include "Mob.h"


class Slime : public Mob
{
public :
    Slime(MobType type) : Mob{type}{
        hp = 200;
        atk = 10;
        speed = 5;
        atk_range_radius = 32;
        init_atk_cool_down = 180;
        atk_cool_down = 20;
        attack_frame_id = 6;
        int state = static_cast<int>(MobState::WALK);
        for(int i=0;i<4;i++){
            bitmap_img_ids[state][i] = 8; 
        }
        state = static_cast<int>(MobState::ATK);
        for(int i=0;i<4;i++){
            bitmap_img_ids[state][i] = 10; 
        }
        state = static_cast<int>(MobState::HURT);
        for(int i=0;i<4;i++){
            bitmap_img_ids[state][i] = 5; 
        }
        state = static_cast<int>(MobState::DIE);
        for(int i=0;i<4;i++){
            bitmap_img_ids[state][i] = 10; 
        }
        state = static_cast<int>(MobState::IDLE);
        for(int i=0;i<4;i++){
            bitmap_img_ids[state][i] = 6; 
        }
        bitmap_switch_freq = 9;
    }
    void atk_hero();
};

#endif