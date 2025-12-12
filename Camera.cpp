#include "Camera.h"
#include "Hero.h"
#include "Utils.h"
#include "data/DataCenter.h"

void Camera::set_target(Hero *target){
    //初始化相機座標至跟隨人物
    GAME_ASSERT(target != nullptr, "Camera target cannot be null.");
    hero = target;
}

void Camera::init(){
    //初始化相機座標至跟隨人物
    //更新相機座標至跟隨人物，並限制不超出地圖邊界(相機座標設為左上角)
    DataCenter *DC = DataCenter::get_instance();
    set_target(DC->hero);
    double map_w = DC->game_field_width;
    double map_h = DC->game_field_height;
    if(hero->shape->center_x()-DC->window_width/2.0f < 0)
        x = 0;
    else if(hero->shape->center_x()+DC->window_width / 2.0f > map_w)
        x = map_w - DC->window_width;
    else
        x = hero->shape->center_x() - DC->window_width / 2.0f;

    if(hero->shape->center_y()-DC->window_height/2.0f <0)
        y = 0;
    else if(hero->shape->center_y()+DC->window_height/2.0f > map_h)
        y = map_h - DC->window_height;
    else
        y = hero->shape->center_y() - DC->window_height/2.0f;
}

void Camera::update(){
    DataCenter *DC = DataCenter::get_instance();
    double map_w = DC->game_field_width;
    double map_h = DC->game_field_height;
    
    if(hero->shape->center_x()-DC->window_width/2.0f <0)
        x = 0;
    else if(hero->shape->center_x()+DC->window_width / 2.0f > map_w)
        x = map_w - DC->window_width;
    else
        x = hero->shape->center_x() - DC->window_width / 2.0f;

    if(hero->shape->center_y()-DC->window_height/2.0f <0)
        y = 0;
    else if(hero->shape->center_y()+DC->window_height/2.0f > map_h)
        y = map_h - DC->window_height;
    else
        y = hero->shape->center_y() - DC->window_height/2.0f;
}

