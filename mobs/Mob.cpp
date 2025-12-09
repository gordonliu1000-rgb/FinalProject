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
#include "../algif5/algif.h"
    




namespace MobSetting {
    static constexpr char mob_imgs_root_path[static_cast<size_t>(MobType::MOBTYPE_MAX)][50] = {
        "./assets/images/mob/slime1"
    };
    static constexpr char state_path_prefix[][10] = {
        "walk", "atk", "hurt", "die"
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



Mob* Mob::create_mob(MobType type){
    switch(type){
        case MobType::SLIME : {
            break;
        }
        default :{
            GAME_ASSERT(0, "Unknow mob type");
            return nullptr;
        }
    }
    return nullptr;
}

Mob::Mob(MobType type){
    shape.reset(new Rectangle{0, 0, 0, 0});
    atk_range.reset(new Circle{0, 0, 0});

    this->type = type;
    DataCenter *DC = DataCenter::get_instance();
    const double &hero_x = DC->hero->shape->center_x();
    const double &hero_y = DC->hero->shape->center_y();
    int mob_gen_dir = static_cast<int>(MobGenPos::MOBGENPOS_MAX);
    MobGenPos mob_gen_pos = 
    static_cast<MobGenPos>(Random::range(0, mob_gen_dir));//生成怪物的位置(hero的上下左右其中一邊)
    float x = -1, y = -1;
    switch(mob_gen_pos){
        case MobGenPos::UP:{
            x = Random::range(0, DC->game_field_width);
            y = Random::range(0, hero_y - DC->window_height/2);
            break;
        }
        case MobGenPos::DOWN:{
            x = Random::range(0, DC->game_field_width);
            y = Random::range(hero_y + DC->window_height/2, DC->window_height);
            break;
        }
        case MobGenPos::LEFT:{
            x = Random::range(0, hero_x - DC->window_width/2);
            y = Random::range(0, DC->window_height);
            break;
        }
        case MobGenPos::RIGHT:{
            x = Random::range(hero_x + DC->window_width/2, DC->window_width);
            y = Random::range(0, DC->window_height);
            break;
        }
        default:{
            break;
        }
    }
    GAME_ASSERT(x!=-1 && y!=-1, "Random generating mob's position error\n");
    shape->update_center_x(x);
    shape->update_center_y(y);

    atk_range.reset(new Circle{shape->center_x(), 
        shape->center_y(), 
        atk_range_radius});

    dir = Dir::RIGHT;
    state = State::WALK;
}

void Mob::hurt(float dmg){
    hp -= dmg;
    bitmap_img_id = 0;
    if(hp <= 0){
        state = State::DIE;
        return;
    }
    state = State::HURT;
}

void Mob::update(){
    DataCenter *DC = DataCenter::get_instance();
    ImageCenter *IC = ImageCenter::get_instance();
    Hero *hero = DC->hero;
    bool can_switch_state_bitmap = true;

    if(bitmap_switch_counter) --bitmap_switch_counter;// 重複播放怪物動畫
    else if(bitmap_img_id == 0 && atk_cool_down && state==State::ATK){// 攻擊完後冷卻期間不做攻擊動作
        bitmap_img_id = 0;
        can_switch_state_bitmap = true;
    }
    else{
        bitmap_img_id = (bitmap_img_id + 1) % bitmap_img_ids[static_cast<int>(state)][static_cast<int>(dir)].size();
        if(bitmap_img_id == 0) can_switch_state_bitmap = true;
        else can_switch_state_bitmap = false;
    }

    atk_cool_down--;
    atk_cool_down = std::max(0, atk_cool_down); //防止冷卻小於0
    switch(state){
        case State::WALK:{
            if(can_switch_state_bitmap && atk_range->overlap(*(hero->shape))/*跳完到地上後才能攻擊人*/){
                state = State::ATK;
                break;
            }
            // walk
            float dx = hero->shape->center_x() - shape->center_x();
            float dy = hero->shape->center_y() - shape->center_y();
            float length = sqrt(dx*dx + dy*dy);
            float new_x = shape->center_x() + dx/length * speed;
            float new_y = shape->center_y() + dy/length * speed;//將方向正規化後乘以速度計算步數
            shape->update_center_x(new_x);
            shape->update_center_y(new_y);// 走動
            atk_range->update_center_x(new_x);
            atk_range->update_center_y(new_y);// 更新攻擊範圍位置
            if(dx <= 0){
                dir = Dir::LEFT;
            }
            else if(dx > 0){
                dir = Dir::RIGHT;
            }
            else if(dy > 0){
                dir = Dir::DOWN;
            }
            else{
                dir = Dir::UP;
            }
            break;
        }
        case State::ATK:{
            if(!atk_cool_down){
                atk_hero();
            }

            if(!atk_range->overlap(*(hero->shape)) && can_switch_state_bitmap){//攻擊完後才能切成走路
                state = State::WALK;
            }
            break;
        }
        case State::HURT:{
            break;
        }
        case State::DIE:{

            // to do
            break;
        }
        default:{
            break;
        }

        
    }
}

void Mob::draw(){
    ImageCenter *IC = ImageCenter::get_instance();
    char buffer[50];
	sprintf(
		buffer, "%s/%s/%d.png",
		MobSetting::mob_imgs_root_path[static_cast<int>(type)],
		MobSetting::dir_path_prefix[static_cast<int>(dir)],
		bitmap_img_ids[static_cast<int>(dir)][bitmap_img_id]);
	ALLEGRO_BITMAP *bitmap = IC->get(buffer);
	al_draw_bitmap(
		bitmap,
		shape->center_x() - al_get_bitmap_width(bitmap) / 2,
		shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
}

void Slime::atk_hero(){
    DataCenter *DC = DataCenter::get_instance();
    //第六張圖是攻擊動作
    bool stop_atk = false;
    if(bitmap_img_id == 6){
        if(stop_atk && atk_range->overlap(*(DC->hero->shape))){ 
            DC->hero->hurt(atk);
            stop_atk = true;
            atk_cool_down = init_atk_cool_down;//重置冷卻
        }
    }
    if(bitmap_img_id == 0){
        stop_atk = false;
    }
}