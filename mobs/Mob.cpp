#include "Mob.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../data/SoundCenter.h"
#include "../Camera.h"
#include "../Hero.h"
#include "../Random.h"
#include "../Utils.h"
#include "../shapes/Circle.h"
#include "../shapes/Rectangle.h"
#include <cmath>
#include "Slime.h"
#include "Flower.h"
#include "Vampire.h"
#include "../buffs/Buff.h"
#include "../algif5/algif.h"
#include <memory>


enum class MobGenPos {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    MOBGENPOS_MAX
};


void Mob::init(){
    Slime::init_img();
    Flower::init_img();
    Vampire::init_img();
}


std::map<MobState, std::map<MobDir, std::vector<ALLEGRO_BITMAP *>>> Slime::img;
std::map<MobState, std::map<MobDir, std::vector<ALLEGRO_BITMAP *>>> Flower::img;
std::map<MobState, std::map<MobDir, std::vector<ALLEGRO_BITMAP *>>> Vampire::img;

std::unique_ptr<Mob> Mob::create_mob(MobType type){
    switch(type){
        case MobType::SLIME : {
            return std::make_unique<Slime>(type);
        }
        case MobType::FLOWER : {
            return std::make_unique<Flower>(type);
        }
        case MobType::VAMPIRE : {
            return std::make_unique<Vampire>(type);
        }
        default :{
            GAME_ASSERT(0, "Unknow mob type");
            return nullptr;
        }
    }
}

Mob::Mob(MobType type){
    this->type = type;
    bitmap_switch_counter = 0;
    dir = MobDir::DOWN;
    state = MobState::IDLE;
    DataCenter *DC = DataCenter::get_instance();
    

    //MobGenPos mob_gen_pos = 
    //static_cast<MobGenPos>(Random::range(0, 3));//生成怪物的位置(hero的上下左右其中一邊)

    float x = 0, y = 0;
    x = Random::range((float)DC->wall_width, (float)DC->game_field_width - DC->wall_width);
    y = Random::range(92.0, DC->game_field_height - DC->wall_width);
    shape.reset(new Point{x, y});
}

void Mob::hurt(float dmg){
    if(state == MobState::DIE || hurt_cooldown != 0) return;
    hp -= dmg;
    SoundCenter *SC = SoundCenter::get_instance();
	SC->play(MobSetting::weapon_hit_sound_path, ALLEGRO_PLAYMODE_ONCE, 1);
    hurt_cooldown = hurt_init_cooldown;
    bitmap_img_id = 0;
    if(hp <= 0){
        state = MobState::DIE;
        DataCenter *DC = DataCenter::get_instance();
        Hero *hero = DC->hero;
        hero->gain_exp(20);
        hero->score++;
        return;
    }
    state = MobState::HURT;
}

void Mob::chase_hero(){
    DataCenter *DC = DataCenter::get_instance();
    Hero *hero = DC->hero;
    float fspeed = speed * 1/DC->FPS;
    float dx = hero->shape->center_x() - shape->center_x();
    float dy = hero->shape->center_y() - shape->center_y();
    float new_x = shape->center_x() + dx/distance * fspeed;
    float new_y = shape->center_y() + dy/distance * fspeed;//將方向正規化後乘以速度計算步數
    shape->update_center_x(new_x);
    shape->update_center_y(new_y);// 走動
    atk_range->update_center_x(new_x);
    atk_range->update_center_y(new_y);// 更新攻擊範圍位置

    if (std::abs(dx) >= std::abs(dy)) {
        dir = (dx >= 0 ? MobDir::RIGHT : MobDir::LEFT);
    } else {
        dir = (dy >= 0 ? MobDir::DOWN : MobDir::UP);
    }
}

void Mob::calculate_distance(){
    Hero *hero = DataCenter::get_instance()->hero;
    float dx = hero->shape->center_x() - shape->center_x();
    float dy = hero->shape->center_y() - shape->center_y();
    if(dx==0 && dy==0) {
        distance = 1;
        return;
    }
    distance = sqrt(dx*dx + dy*dy);
}

void Mob::update(){
    if(die) return;
    Hero *hero = DataCenter::get_instance()->hero;
    

    if(bitmap_switch_counter > 0) //一張圖要持續多久
        --bitmap_switch_counter;
    else{
        bitmap_switch_counter = bitmap_switch_freq;
        GAME_ASSERT(bitmap_img_id < get_bitmaps_last_idx(state) + 1, "Index out of range!\n");
        bitmap_img_id = (bitmap_img_id + 1) % (get_bitmaps_last_idx(state) + 1);// 重複播放怪物動畫
    }

    if (atk_cool_down > 0) --atk_cool_down; //防止冷卻小於0
    if (hurt_cooldown > 0) --hurt_cooldown; //受傷無敵時間
    if (update_distance_counter > 0)
        update_distance_counter--;
    else{
        update_distance_counter = update_distance_cooldown;
        calculate_distance();
    }
       
    switch(state){
        case MobState::WALK:{
            if(bitmap_switch_counter == 0 && bitmap_img_id==get_bitmaps_last_idx(MobState::WALK) && 
                atk_range->overlap(*(hero->shape))/*跳完到地上後才能攻擊人*/){
                bitmap_img_id = 0;
                state = MobState::IDLE;
                bitmap_switch_counter = bitmap_switch_freq;
                break;
            }
            // walk
            chase_hero();
            break;
        }
        case MobState::ATK:{
            if(bitmap_switch_counter==0 && bitmap_img_id == attack_frame_id){
                atk_hero();
                break;
            }

            if(bitmap_switch_counter == 0 && bitmap_img_id==get_bitmaps_last_idx(MobState::ATK)){//攻擊完後(image id == end id)才能切成閒置
                state = MobState::IDLE;
                bitmap_img_id = 0;
                bitmap_switch_counter = bitmap_switch_freq;
            }
            break;
        }
        case MobState::HURT:{
            if(bitmap_switch_counter == 0 && bitmap_img_id==get_bitmaps_last_idx(MobState::HURT)){
                state = MobState::IDLE;
                bitmap_img_id = 0;
                bitmap_switch_counter = bitmap_switch_freq;
            }

            chase_hero();
                
            break;
        }
        case MobState::DIE:{
            // to do
            if(bitmap_switch_counter == 0 && bitmap_img_id==get_bitmaps_last_idx(MobState::DIE)/*last idx of die pic*/){
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
                break;
            }

            // hero 還在範圍內且冷卻好了 → 再進一次 ATK
            if (atk_cool_down == 0) {
                state = MobState::ATK;
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
    if(atk_range->overlap(*(DC->hero->shape))){ 
        DC->hero->hurt(atk);
        atk_cool_down = init_atk_cool_down;//重置冷卻
    }
}

void Flower::atk_hero(){
    DataCenter *DC = DataCenter::get_instance();
    if(atk_range->overlap(*(DC->hero->shape))){ 
        DC->hero->hurt(atk);
        atk_cool_down = init_atk_cool_down;//重置冷卻
    }
}

void Vampire::atk_hero(){
    DataCenter *DC = DataCenter::get_instance();
    DC->enemy_spells.emplace_back(EnemySpell::create_spell(this, atk, EnemySpellType::FIREBALL)); 
    atk_cool_down = init_atk_cool_down;//重置冷卻
}



void Mob::dropItem(){
    //bool drop = Random::range(1, 100) <= 10;// 10%掉落率
    bool drop = 100;
    if(drop){
        DataCenter *DC = DataCenter::get_instance();
        BuffType type = static_cast<BuffType>(Random::range(0, 3));
        DC->buff_items.emplace_back(std::make_unique<Buffitem>(type, 
            Point{shape->center_x(), shape->center_y()}));
        debug_log("spawn buff at x=%.1f y=%.1f type=%d\n", shape->center_x(), shape->center_y(), (int)type);
    }
}

void Mob::draw(){
    //超出範圍不draw
    static const int &window_width = DataCenter::get_instance()->window_width;
    static const int &window_height = DataCenter::get_instance()->window_height;
    const double camera_x = DataCenter::get_instance()->camera->top_left_x();
    const double camera_y = DataCenter::get_instance()->camera->top_left_y();
    if(!(camera_x < shape->center_x() && shape->center_x() < camera_x + window_width )||
    !(camera_y < shape->center_y() && shape->center_y() < camera_y + window_height)){
        return;
    }

	ALLEGRO_BITMAP *bitmap = get_bitmap(bitmap_img_id);
    float scale_rate = 1.5;
    al_draw_scaled_bitmap(
        bitmap,
        0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap),
        shape->center_x() - (al_get_bitmap_width(bitmap) * scale_rate) / 2,
        shape->center_y() - (al_get_bitmap_height(bitmap) * scale_rate) / 2,
        al_get_bitmap_width(bitmap) * scale_rate, // dw
        al_get_bitmap_height(bitmap) * scale_rate, // dh
        0
    );
    
}
