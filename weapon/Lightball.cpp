#include "Lightball.h"
#include "../data/GifCenter.h"
#include "../shapes/Circle.h"
#include "../Utils.h"

static constexpr char lightball_img_path[] = 
        "./assets/gif/Weapon/lightball.gif";

Lightball::Lightball(float radius, float angular_speed) : Weapon(radius, angular_speed)
{
    GIFCenter *GIFC = GIFCenter::get_instance();
    gif = GIFC->get(lightball_img_path);
    float r = std::max(gif->width, gif->height) / 2.0;
    shape.reset(new Circle{0.0, 0.0, r});
}

void Lightball::draw(){
    if(!gif) return;
    float cx = shape->center_x();
    float cy = shape->center_y();
    algif_draw_gif(gif, cx - gif->width / 2, cy - gif->height / 2, 0);
}