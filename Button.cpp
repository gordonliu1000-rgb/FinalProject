#include "Button.h"
#include <allegro5/allegro_primitives.h>

Button::Button() : x_(0), y_(0), w_(0), h_(0), text_(nullptr) {}

Button::Button(float x, float y, float w, float h, const char* text) 
    : x_(x), y_(y), w_(w), h_(h), text_(text) {}

bool Button::update(float mx, float my, bool mouse_down, bool mouse_prev){
    bool inside  = (mx >= x_ && mx <= x_ + w_ && my >= y_ && my <= y_ + h_);
    return inside && mouse_down && !mouse_prev;
}

void Button::draw(ALLEGRO_FONT *font, ALLEGRO_COLOR normal_color, ALLEGRO_COLOR hover_color,
                    ALLEGRO_COLOR border_color, ALLEGRO_COLOR text_color, float mx, float my) const{
                        
    bool hover = (mx >= x_ && mx <= x_ + w_ && my >= y_ && my <= y_ + h_);
    ALLEGRO_COLOR fill = hover ? hover_color : normal_color;
    al_draw_filled_rectangle(x_, y_, x_ + w_, y_ + h_, fill);
    al_draw_rectangle(x_, y_, x_ + w_, y_ + h_, border_color, 2);

    if(font && text_){
        float ty = y_ + h_ / 2.0f - al_get_font_line_height(font) / 2.0f;
        al_draw_text(font, text_color, x_ + w_ / 2.0f, ty, ALLEGRO_ALIGN_CENTER, text_);
    }
}

void Button::set_rect(float x, float y, float w, float h){
    x_ = x;
    y_ = y;
    w_ = w;
    h_ = h;
}

void Button::set_text(const char* text){
    text_ = text;
}