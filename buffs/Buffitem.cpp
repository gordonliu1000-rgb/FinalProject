#include "Buffitem.h"
#include "Buff.h"
#include "../data/ImageCenter.h"
#include "../Utils.h"
#include <allegro5/allegro_primitives.h>

char buff_icon_img[] = "./assets/Material/buff_icon";
char buff_icon_img_file[static_cast<int>(BuffType::BUFFTYPE_MAX)][15]={
    "speed",
    "power",
    "heal",
    "absorb",
    "invincible",
    "revive",
};

Buffitem::Buffitem(BuffType type, const Point &pos) : type(type), bitmap_map_id(0){

    ImageCenter *IC = ImageCenter::get_instance();
    char icon_path[64];
    sprintf(icon_path, "%s/%s.png", buff_icon_img,
        buff_icon_img_file[static_cast<int>(type)]);

    ALLEGRO_BITMAP *bitmap = IC -> get(icon_path);
    GAME_ASSERT(bitmap, "Buffitem icon path incorrect.");

    int w = al_get_bitmap_width(bitmap);
    int h = al_get_bitmap_height(bitmap);
    shape.reset(new Rectangle{pos.x, pos.y, pos.x + w, pos.y + h});

}

void Buffitem::draw(){
    ImageCenter *IC = ImageCenter::get_instance();

    char icon_path[64];
    sprintf(icon_path, "%s/%s.png", buff_icon_img,
        buff_icon_img_file[static_cast<int>(type)]);
    
    ALLEGRO_BITMAP *bitmap = IC -> get(icon_path);
    GAME_ASSERT(bitmap, "Buffitem icon path incorrect")
    int w = al_get_bitmap_width(bitmap);
    int h = al_get_bitmap_height(bitmap);

    float scale = 0.5f;             
    float dw = w * scale;
    float dh = h * scale;
    float cx = shape->center_x();
    float cy = shape->center_y();
    al_draw_scaled_bitmap(bitmap, 0, 0, w, h, cx - dw / 2, cy - dh / 2, dw, dh, 0);
}

void Buffitem::update(){
    
}