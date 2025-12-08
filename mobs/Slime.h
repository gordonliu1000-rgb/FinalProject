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
        atk_range_radius = 5;
        init_atk_cool_down   = 120;
        atk_cool_down = 0;
        int state = static_cast<int>(State::WALK);
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7});
        state = static_cast<int>(State::ATK);
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        state = static_cast<int>(State::HURT);
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4});
        state = static_cast<int>(State::HURT);
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        bitmap_img_ids[state].emplace_back(std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        bitmap_switch_freq = 9;
    }

    void atk_hero();
};

#endif