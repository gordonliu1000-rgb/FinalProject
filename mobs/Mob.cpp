#include "Mob.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Hero.h"
#include "../Random.h"
#include "../Utils.h"
#include "../shapes/Circle.h"
#include "../shapes/Rectangle.h"
#include <cmath>
#include "Slime.h"
#include "../buffs/Buff.h"
#include "../algif5/algif.h"
#include <memory>
    




namespace MobSetting {
    static constexpr char mob_imgs_root_path[static_cast<size_t>(MobType::MOBTYPE_MAX)][50] = {
        "./assets/image/mob/slime1"
    };
    static constexpr char state_path_prefix[][10] = {
        "walk", "atk", "hurt", "die", "idle"
    };
    static constexpr char dir_path_prefix[][10] = {
        "up", "down", "left", "right"
    };
}



enum class MobGenPos {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    MOBGENPOS_MAX
};



std::unique_ptr<Mob> Mob::create_mob(MobType type){
    switch(type){
        case MobType::SLIME : {
            return std::make_unique<Slime>(type);
        }
        default :{
            GAME_ASSERT(0, "Unknow mob type");
            return nullptr;
        }
    }
}

Mob::Mob(MobType type){

    this->type = type;
    DataCenter *DC = DataCenter::get_instance();
    const double &hero_x = DC->hero->shape->center_x();
    const double &hero_y = DC->hero->shape->center_y();

    MobGenPos mob_gen_pos = 
    static_cast<MobGenPos>(Random::range(0, 3));//生成怪物的位置(hero的上下左右其中一邊)

    float x = -1, y = -1;
    switch(mob_gen_pos){
        case MobGenPos::UP:{
            x = Random::range(0.0, (float)DC->game_field_width);
            y = Random::range(0.0, (float)hero_y - DC->window_height/2);
            break;
        }
        case MobGenPos::DOWN:{
            x = Random::range(0.0, (float)DC->game_field_width);
            y = Random::range((float)(hero_y + DC->window_height/2), (float)DC->window_height);
            break;
        }
        case MobGenPos::LEFT:{
            x = Random::range(0.0, (float)(hero_x - DC->window_width/2));
            y = Random::range(0.0, (float)DC->window_height);
            break;
        }
        case MobGenPos::RIGHT:{
            x = Random::range((float)(hero_x + DC->window_width/2), (float)DC->window_width);
            y = Random::range(0.0, (float)DC->window_height);
            break;
        }
        default:{
            break;
        }
    }

    ImageCenter *IC = ImageCenter::get_instance();
    char buffer[50];
	sprintf(
		buffer, "%s/%s/%s/%d.png",
		MobSetting::mob_imgs_root_path[static_cast<int>(type)],
        MobSetting::state_path_prefix[static_cast<int>(state)],
        MobSetting::dir_path_prefix[static_cast<int>(dir)],
		bitmap_img_id);
	ALLEGRO_BITMAP *bitmap = IC->get(buffer);

    const float &w = al_get_bitmap_width(bitmap) * 0.8;
    const float &h = al_get_bitmap_height(bitmap) * 0.8;

    atk_range.reset(new Circle{x, 
        y, 
        atk_range_radius});
    shape.reset(new Rectangle{
        x - w/2.,
        y - h/2.,
        x + w/2.,
        y + h/2.
    });

    dir = Dir::DOWN;
    state = MobState::IDLE;
    bitmap_switch_counter = 0;

    debug_log("mob spawn at x=%f y=%f\n", shape->center_x(), shape->center_y());
}

void Mob::hurt(float dmg){
    if(state == MobState::DIE || hurt_cooldown != 0) return;
    hp -= dmg;
    hurt_cooldown = hurt_init_cooldown;
    bitmap_img_id = 0;
    if(hp <= 0){
        state = MobState::DIE;
        DataCenter *DC = DataCenter::get_instance();
        Hero *hero = DC->hero;
        hero->gain_exp(20);
        return;
    }
    state = MobState::HURT;
    debug_log("mobState : Hurt\n");
}

void Mob::update(){
    if(die) return;
    DataCenter *DC = DataCenter::get_instance();
    Hero *hero = DC->hero;
    

    if(bitmap_switch_counter > 0) //一張圖要持續多久
        --bitmap_switch_counter;
    else{
        bitmap_switch_counter = bitmap_switch_freq;
        GAME_ASSERT(bitmap_img_id < bitmap_img_ids[static_cast<int>(state)][static_cast<int>(dir)], "Index out of range!\n");
        bitmap_img_id = (bitmap_img_id + 1) % bitmap_img_ids[static_cast<int>(state)][static_cast<int>(dir)];// 重複播放怪物動畫
    }

    if (atk_cool_down > 0) --atk_cool_down; //防止冷卻小於0
    if (hurt_cooldown > 0) --hurt_cooldown; //受傷無敵時間
        

    switch(state){
        case MobState::WALK:{
            if(bitmap_switch_counter==0 && 
                (bitmap_img_ids[static_cast<int>(MobState::WALK)][static_cast<int>(dir)]-1) && 
                atk_range->overlap(*(hero->shape))/*跳完到地上後才能攻擊人*/){
                bitmap_img_id = 0;
                state = MobState::IDLE;
                bitmap_switch_counter = bitmap_switch_freq;
                break;
            }
            // walk
            float fspeed = speed * 1/DC->FPS;
            float dx = hero->shape->center_x() - shape->center_x();
            float dy = hero->shape->center_y() - shape->center_y();
            float length = sqrt(dx*dx + dy*dy);
            float new_x = shape->center_x() + dx/length * fspeed;
            float new_y = shape->center_y() + dy/length * fspeed;//將方向正規化後乘以速度計算步數
            shape->update_center_x(new_x);
            shape->update_center_y(new_y);// 走動
            atk_range->update_center_x(new_x);
            atk_range->update_center_y(new_y);// 更新攻擊範圍位置
            if (std::abs(dx) >= std::abs(dy)) {
                dir = (dx >= 0 ? Dir::RIGHT : Dir::LEFT);
            } else {
                dir = (dy >= 0 ? Dir::DOWN : Dir::UP);
            }
            break;
        }
        case MobState::ATK:{
            if(bitmap_switch_counter==0 && bitmap_img_id == attack_frame_id){
                atk_hero();
                break;
            }

            if(bitmap_switch_counter && bitmap_img_id==(bitmap_img_ids[static_cast<int>(MobState::ATK)][static_cast<int>(dir)]-1)){//攻擊完後(image id == end id)才能切成閒置
                state = MobState::IDLE;
                bitmap_img_id = 0;
                bitmap_switch_counter = bitmap_switch_freq;
                
                debug_log("mobState : Idle\n");
            }
            break;
        }
        case MobState::HURT:{
            if(bitmap_img_id==(bitmap_img_ids[static_cast<int>(MobState::HURT)][static_cast<int>(dir)]-1) && bitmap_switch_counter == 0){
                state = MobState::WALK;
                bitmap_img_id = 0;
                bitmap_switch_counter = bitmap_switch_freq;
            }
                
            break;
        }
        case MobState::DIE:{
            // to do
            if(bitmap_switch_counter == 0 && 
                bitmap_img_id==(bitmap_img_ids[static_cast<int>(MobState::DIE)][static_cast<int>(dir)]-1)/*last idx of die pic*/){
                die = true;
                dropItem();
            }
                
            break;
        }
        case MobState::IDLE: {
            // hero 不在範圍回 WALK
            
            if (!atk_range->overlap(*(hero->shape))) {
                bitmap_img_id = 0;
                state = MobState::WALK;
                debug_log("MobState : Walk\n");
                break;
            }

            // hero 還在範圍內且冷卻好了 → 再進一次 ATK
            if (atk_cool_down == 0) {
                state = MobState::ATK;
                debug_log("MobState : Atk\n");
                bitmap_switch_counter = 0;
                bitmap_img_id = 0;
            }
            break;
        }
        default:{
            break;
        }
    }

}

void Slime::atk_hero(){
    DataCenter *DC = DataCenter::get_instance();
    //第六張圖是攻擊動作
    if(atk_range->overlap(*(DC->hero->shape))){ 
        DC->hero->hp -= atk;
        atk_cool_down = init_atk_cool_down;//重置冷卻
    }
}

void Mob::dropItem(){
    bool drop = Random::range(1, 100) <= 10;// 10%掉落率
    if(drop){
        DataCenter *DC = DataCenter::get_instance();
        BuffType type = static_cast<BuffType>(Random::range(0, 1));
        DC->buff_items.emplace_back(std::make_unique<Buffitem>(type, 
            Point{shape->center_x(), shape->center_y()}));
        debug_log("spawn buff at x=%.1f y=%.1f type=%d\n", shape->center_x(), shape->center_y(), (int)type);
    }
}

void Mob::draw(){


    ImageCenter *IC = ImageCenter::get_instance();
    char buffer[50];

    GAME_ASSERT(bitmap_img_id < bitmap_img_ids[static_cast<int>(state)][static_cast<int>(dir)], "Index out of range!\n");
    sprintf(
    buffer, "%s/%s/%s/%d.png",
    MobSetting::mob_imgs_root_path[static_cast<int>(type)],
    MobSetting::state_path_prefix[static_cast<int>(state)],
    MobSetting::dir_path_prefix[static_cast<int>(dir)],
    bitmap_img_id);
    
	ALLEGRO_BITMAP *bitmap = IC->get(buffer);
	al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap) / 2,
		shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
}
