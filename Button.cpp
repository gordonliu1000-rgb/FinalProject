#include "Button.h"
#include <allegro5/allegro_primitives.h>

Button::Button() : x(0), y(0), w(0), h(0), text(nullptr) {}

Button::Button(float x, float y, float w, float h, const char* text) 
    : x(x), y(y), w(w), h(h), text(text) {}

bool Button::update(float mx, float my, bool mouse_down, bool mouse_prev){
    bool inside  = (mx >= this->x && mx <= this->x + this->w && my >= this->y && my <= this->y + this->h);
    return inside && mouse_down && !mouse_prev;
}

void Button::draw(ALLEGRO_FONT *font, ALLEGRO_COLOR normal_color, ALLEGRO_COLOR hover_color,
                    ALLEGRO_COLOR border_color, ALLEGRO_COLOR text_color, float mx, float my) const{
                        
    bool hover = (mx >= this->x && mx <= this->x + this->w && my >= this->y && my <= this->y + this->h);
    ALLEGRO_COLOR fill = hover ? hover_color : normal_color;
    al_draw_filled_rectangle(this->x, this->y, this->x + this->w, this->y + this->h, fill);
    al_draw_rectangle(this->x, this->y, this->x + this->w, this->y + this->h, border_color, 2);

    if(font && this->text){
        float ty = this->y + this->h / 2.0f - al_get_font_line_height(font) / 2.0f;
        al_draw_text(font, text_color, this->x + this->w / 2.0f, ty, ALLEGRO_ALIGN_CENTER, this->text);
    }
}

void Button::set_rect(float x, float y, float w, float h){
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

void Button::set_text(const char* text){
    this->text = text;
}