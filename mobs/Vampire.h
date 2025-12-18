#ifndef VAMPIRE_H_INCLUDED
#define VAMPIRE_H_INCLUDED
#include "../shapes/Rectangle.h"
#include "../Utils.h"
#include "Mob.h"

class Vampire : public Mob
{
public:
    static std::map<MobState, std::map<MobDir, std::vector<ALLEGRO_BITMAP *>>> img;
    static void init_img(){
        ImageCenter *IC = ImageCenter::get_instance();
        
        char buffer[60];
        MobState state = MobState::WALK;
        for(int i=0;i<4;i++){
            for(int j=0;j<6;j++){
                sprintf(
                buffer, "%s/%s/%s/%d.png",
                "./assets/image/mob/vampire1",
                MobSetting::state_path_prefix[static_cast<int>(state)],
                MobSetting::dir_path_prefix[i],
                j);
                img[state][static_cast<MobDir>(i)].emplace_back(IC->get(buffer));
            }
            
        }

        state = MobState::ATK;
        for(int i=0;i<4;i++){
            for(int j=0;j<12;j++){
                sprintf(
                buffer, "%s/%s/%s/%d.png",
                "./assets/image/mob/vampire1",
                MobSetting::state_path_prefix[static_cast<int>(state)],
                MobSetting::dir_path_prefix[i],
                j);
                img[state][static_cast<MobDir>(i)].emplace_back(IC->get(buffer));
            }
        }

        state = MobState::DIE;
        for(int i=0;i<4;i++){
            for(int j=0;j<11;j++){
                sprintf(
                buffer, "%s/%s/%s/%d.png",
                "./assets/image/mob/vampire1",
                MobSetting::state_path_prefix[static_cast<int>(state)],
                MobSetting::dir_path_prefix[i],
                j);
                img[state][static_cast<MobDir>(i)].emplace_back(IC->get(buffer));
            }
            
        }

        state = MobState::HURT;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                sprintf(
                buffer, "%s/%s/%s/%d.png",
                "./assets/image/mob/vampire1",
                MobSetting::state_path_prefix[static_cast<int>(state)],
                MobSetting::dir_path_prefix[i],
                j);
                img[state][static_cast<MobDir>(i)].emplace_back(IC->get(buffer));
            }
            
        }

        state = MobState::IDLE;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                sprintf(
                buffer, "%s/%s/%s/%d.png",
                "./assets/image/mob/vampire1",
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
        return img[state][dir].size() - 1;
    }

    Vampire(MobType type) : Mob{type}{
        hp = 40;
        atk = 15;
        speed = 20;
        atk_range_radius = 300;
        init_atk_cool_down = 180;
        atk_cool_down = 30;
        attack_frame_id = 8;
        const float &w = al_get_bitmap_width(img[state][dir][0]);
        const float &h = al_get_bitmap_width(img[state][dir][0]);
        shape.reset(new Rectangle{
            shape->center_x() - w/2,
            shape->center_y() - h/2,
            shape->center_x() + w/2,
            shape->center_y() + h/2
        });

        atk_range.reset(new Circle{shape->center_x(), 
            shape->center_y(), 
            this->atk_range_radius});
        bitmap_switch_freq = 7;

        debug_log("mob spawn at x=%f y=%f\n", shape->center_x(), shape->center_y());
    }
    void atk_hero();
};


#endif