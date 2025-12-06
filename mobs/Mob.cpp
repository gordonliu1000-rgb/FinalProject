#include "Mob.h"
#include "../data/DataCenter.h"
#include "../data/ImageCenter.h"
#include "../Hero.h"
#include "../Random.h"
#include "../Utils.h"
#include "../shapes/Circle.h"
#include "../shapes/Rectangle.h"
#include <cmath>

enum class State {
    WALK,
    ATK
};

enum class Dir{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class MobGenPos {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    MOBGENPOS_MAX
};

Mob *Mob::create_mob(MobType type){
    switch(type){
        case MobType::SLIME : {

        }
        default :{
            GAME_ASSERT(1, "Unknow mob type");
            return nullptr;
        }
    }
}

Mob::Mob(MobType type){
    shape.reset(new Rectangle{0, 0, 0, 0});// reset shape
    this->type = type;
    DataCenter *DC = DataCenter::get_instance();
    const double &hero_x = DC->hero->shape->center_x();
    const double &hero_y = DC->hero->shape->center_y();
    int r = static_cast<int>(MobGenPos::MOBGENPOS_MAX);
    MobGenPos mob_gen_pos = 
    static_cast<MobGenPos>(Random::range(0, r));//生成怪物的位置(hero的上下左右其中一邊)
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
    GAME_ASSERT(x!=-1 || y!=-1, "Random generating mob's position error\n");
    shape->update_center_x(x);
    shape->update_center_y(y);

    atk_range.reset(new Circle{shape->center_x(), 
        shape->center_y(), 
        atk_range_radius});

    dir = Dir::RIGHT;
    state = State::WALK;
}

void Mob::update(){
    DataCenter *DC = DataCenter::get_instance();
    Hero *hero = DC->hero;
    atk_cool_down--;
    atk_cool_down = std::max(0, atk_cool_down); //防止冷卻小於0
    switch(state){
        case State::WALK:{
            if(atk_range->overlap(*(hero->shape))){
                state = State::ATK;
                break;
            }
            break;
        }
        case State::ATK:{
            if(atk_cool_down==0){//攻擊冷卻時間
                atk_hero();// 依怪獸自己的攻擊方式進行攻擊
                atk_cool_down = init_atk_cool_down;
            }
            
            if(!atk_range->overlap(*(hero->shape))){
                state = State::WALK;
                break;
            }
            break;
        }
        default:{
            break;
        }
    }

    if(state==State::WALK){
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
    }

}