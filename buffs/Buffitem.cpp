#include "Buffitem.h"
#include "Buff.h"
#include "../data/ImageCenter.h"
#include "../Utils.h"
#include <allegro5/allegro_primitives.h>

extern char buff_icon_img_prefix[];
extern char buff_icon_img_file_name[static_cast<int>(BuffType::BUFFTYPE_MAX)][15];

Buffitem::Buffitem(BuffType type, const Point &pos) : type(type), bitmap_map_id(0){

    ImageCenter *IC = ImageCenter::get_instance();
    char icon_path[64];
    sprintf(icon_path, "%s/%s_buff_icon.png", buff_icon_img_prefix,
        buff_icon_img_file_name[static_cast<int>(type)]);

    ALLEGRO_BITMAP *bitmap = IC -> get(icon_path);
    GAME_ASSERT(bitmap, "Buffitem icon path incorrect.");

    int w = al_get_bitmap_width(bitmap);
    int h = al_get_bitmap_height(bitmap);
    shape.reset(new Rectangle{pos.x, pos.y, pos.x + w, pos.y + h});

}

void Buffitem::draw(){
    ImageCenter *IC = ImageCenter::get_instance();

    char icon_path[64];
    sprintf(icon_path, "%s/%s_buff_icon.png", buff_icon_img_prefix,
        buff_icon_img_file_name[static_cast<int>(type)]);
    
    ALLEGRO_BITMAP *bitmap = IC -> get(icon_path);
    GAME_ASSERT(bitmap, "Buffitem icon path incorrect")
    
    al_draw_bitmap(bitmap, 
        shape->center_x() - al_get_bitmap_width(bitmap) / 2,
        shape->center_y() - al_get_bitmap_height(bitmap) / 2, 0);
}

void Buffitem::update(){

}