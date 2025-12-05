#include "Sword.h"
#include "../Hero.h"
#include "../shapes/Rectangle.h"
#include <cmath>
#include <allegro5/allegro_primitives.h>  

static constexpr char sword_img_path[] = 
    "./assets/image/weapon/Sword.png";

Sword::Sword(float radius, float angular_speed) : Weapon(radius, angular_speed)
{
    ImageCenter *IC = ImageCenter::get_instance();
    bitmap = IC->get(sword_img_path);

    float w = al_get_bitmap_width(bitmap);
    float h = al_get_bitmap_height(bitmap);
    shape.reset(new Rectangle{0.0, 0.0, w, h});

}

void Sword::draw(){
    if(!bitmap) return;

    float w = al_get_bitmap_width(bitmap);
    float h = al_get_bitmap_height(bitmap);
    float cx = shape->center_x();
    float cy = shape->center_y();

    float bmp_cx = w / 2.0f;
    float bmp_cy = h / 2.0f;

    al_draw_rotated_bitmap(bitmap, bmp_cx, bmp_cy, cx, cy, angle, 0);
}

