#include "Exp.h"
#include "./shapes/Rectangle.h"
#include "./data/ImageCenter.h"
#include "./data/DataCenter.h"
#include "Camera.h"

Exp::Exp(double x, double y){
    w = al_get_bitmap_width(img);
    h = al_get_bitmap_height(img);
    shape.reset(new Rectangle{
        x - w/2,
        y - h/2, 
        x + w/2,
        y - h/2
    });
}

ALLEGRO_BITMAP* Exp::img; 
void Exp::init_img(){
    img = ImageCenter::get_instance()->get("./assets/image/exp.png");
}

void Exp::draw(){
    if(picked) return;
    static const int &window_width = DataCenter::get_instance()->window_width;
    static const int &window_height = DataCenter::get_instance()->window_height;
    const double camera_x = DataCenter::get_instance()->camera->top_left_x();
    const double camera_y = DataCenter::get_instance()->camera->top_left_y();
    if(!(camera_x < shape->center_x() && shape->center_x() < camera_x + window_width )||
    !(camera_y < shape->center_y() && shape->center_y() < camera_y + window_height)){
        return;
    }

    al_draw_bitmap(
        img,
        shape->center_x() - w/2,
        shape->center_y() - h/2,
        0
    );
}