#include "Buff.h"
#include "Speed.h"
#include "Power.h"
#include <allegro5/allegro.h>
#include "../data/ImageCenter.h"
#include <allegro5/allegro_primitives.h>
#include "../Utils.h"

Buff* Buff::create_buff(BuffType type){
    switch(type){
        case BuffType::SPEED : {
            return new Speed(type);
        }
        case BuffType::POWER : {
            return new Power(type);
        }
        default:{
            break;
        }
    }
    return nullptr;
}


char buff_icon_img_prefix[] = "./assets/image/buff/icons";
char buff_icon_img_file_name[static_cast<int>(BuffType::BUFFTYPE_MAX)][15]={
    "speed",
    "power",
    "heal",
    "absorb",
    "invincible",
    "revive",
};

Buff::Buff(BuffType type){
    this->type = type;
    icon = nullptr;
}


void Buff::reset_duration(){
    duration = init_duration;
}

void Speed::effect(){
    static DataCenter *DC = DataCenter::get_instance();
    DC->hero->speed = init_speed * 2;
}

void Speed::clear_effect(){
    static DataCenter *DC = DataCenter::get_instance();
    DC->hero->speed = init_speed;
}

void Power::effect(){
    static DataCenter *DC = DataCenter::get_instance();
    DC->hero->atk = init_attack * 2;
}

void Power::clear_effect(){
    static DataCenter *DC = DataCenter::get_instance();
    DC->hero->atk = init_attack;
}
//new buff write here

void Buff::draw_icon(){
    if(duration==0){
        icon = nullptr;
        return; 
    }
    char icon_path[50];
    sprintf(icon_path, 
        "%s/%s_buff_icon.png", 
        buff_icon_img_prefix,
        buff_icon_img_file_name[static_cast<int>(type)]
    );
    ImageCenter *IC = ImageCenter::get_instance();
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    ALLEGRO_BITMAP *base = IC->get(icon_path);
    GAME_ASSERT(base, "path incorrect");
    int w = al_get_bitmap_width(base);
    int h = al_get_bitmap_height(base);
    ALLEGRO_BITMAP *result = al_create_bitmap(2*w, 2*h);
    ALLEGRO_BITMAP *display = al_get_target_bitmap();
    al_set_target_bitmap(result);//將繪圖對象設成目標
    al_clear_to_color(al_map_rgba(0, 0, 0, 0)); //透明底
    al_draw_scaled_bitmap(base, 0, 0, w, h, 0, 0, 2*w, 2*h, 0);// 背景底圖
    double mask_y = duration/static_cast<double>(init_duration) * 2 * h;// 計算遮罩位置
    al_draw_filled_rectangle(0, mask_y, 2*w, 2*h, al_map_rgba(255,255,255,175));
    
    icon = result;
    al_set_target_bitmap(display);// 回到原先繪圖對象
}

void Buff::update(){
    if(duration == 0){
        clear_effect();
        draw_icon();
        return ;
    }
    effect();
    draw_icon();
    duration--;
    return ;
}


