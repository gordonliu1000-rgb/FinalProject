#ifndef SLIME2_H_INCLUDED
#define SLIME2_H_INCLUDED
#include "Mob.h"
#include "../shapes/Rectangle.h"
#include "../Utils.h"
#include <memory>

class Slime2 : public Mob
{
public:
    static float init_atk, init_hp;
    static std::map<MobState, std::map<MobDir, std::vector<ALLEGRO_BITMAP *>>> img;
    static void init_img(){
        ImageCenter *IC = ImageCenter::get_instance();
        
        char buffer[60];
        MobState state = MobState::WALK;
        for(int i=0;i<4;i++){
            for(int j=0;j<8;j++){
                sprintf(
                buffer, "%s/%s/%s/%d.png",
                MobSetting::mob_imgs_root_path[static_cast<size_t>(MobType::SLIME2)],
                MobSetting::state_path_prefix[static_cast<int>(state)],
                MobSetting::dir_path_prefix[i],
                j);
                img[state][static_cast<MobDir>(i)].emplace_back(IC->get(buffer));
            }
            
        }

        state = MobState::ATK;
        for(int i=0;i<4;i++){
            for(int j=0;j<9;j++){
                sprintf(
                buffer, "%s/%s/%s/%d.png",
                MobSetting::mob_imgs_root_path[static_cast<size_t>(MobType::SLIME2)],
                MobSetting::state_path_prefix[static_cast<int>(state)],
                MobSetting::dir_path_prefix[i],
                j);
                img[state][static_cast<MobDir>(i)].emplace_back(IC->get(buffer));
            }
        }

        state = MobState::DIE;
        for(int i=0;i<4;i++){
            for(int j=0;j<8;j++){
                sprintf(
                buffer, "%s/%s/%s/%d.png",
                MobSetting::mob_imgs_root_path[static_cast<size_t>(MobType::SLIME2)],
                MobSetting::state_path_prefix[static_cast<int>(state)],
                MobSetting::dir_path_prefix[i],
                j);
                img[state][static_cast<MobDir>(i)].emplace_back(IC->get(buffer));
            }
            
        }

        state = MobState::HURT;
        for(int i=0;i<4;i++){
            for(int j=0;j<5;j++){
                sprintf(
                buffer, "%s/%s/%s/%d.png",
                MobSetting::mob_imgs_root_path[static_cast<size_t>(MobType::SLIME2)],
                MobSetting::state_path_prefix[static_cast<int>(state)],
                MobSetting::dir_path_prefix[i],
                j);
                img[state][static_cast<MobDir>(i)].emplace_back(IC->get(buffer));
            }
            
        }

        state = MobState::IDLE;
        for(int i=0;i<4;i++){
            for(int j=0;j<6;j++){
                sprintf(
                buffer, "%s/%s/%s/%d.png",
                MobSetting::mob_imgs_root_path[static_cast<size_t>(MobType::SLIME2)],
                MobSetting::state_path_prefix[static_cast<int>(state)],
                MobSetting::dir_path_prefix[i],
                j);
                img[state][static_cast<MobDir>(i)].emplace_back(IC->get(buffer));
            }
            
        }
        
    }
public :
    ALLEGRO_BITMAP* get_bitmap(int bitmap_id){
        return img[state][dir][bitmap_id];
    }

    int get_bitmaps_last_idx(MobState state){
        GAME_ASSERT(img[state][dir].size() > 0, "No bitmap for this state");
        return img[state][dir].size() - 1;
    }

    Slime2(MobType type) : Mob{type}{
        hp = init_hp;
        atk = init_atk;
        speed = 200;
        explosive = true;
        atk_range_radius = 60;
        init_atk_cool_down = 180;
        atk_cool_down = 20;
        attack_frame_id = 4;
        const float &w = al_get_bitmap_width(img[state][dir][0]);
        const float &h = al_get_bitmap_height(img[state][dir][0]);
        shape.reset(new Rectangle{
            shape->center_x() - w/2,
            shape->center_y() - h/2,
            shape->center_x() + w/2,
            shape->center_y() + h/2
        });

        atk_range.reset(new Circle{shape->center_x(), 
            shape->center_y(), 
            this->atk_range_radius});
        bitmap_switch_freq = 3;

        debug_log("mob spawn at x=%f y=%f\n", shape->center_x(), shape->center_y());
    }
    void atk_hero();
};

#endif