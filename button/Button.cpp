#include "Button.h"
#include "../data/DataCenter.h"
#include "../shapes/Point.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_primitives.h>

Button::Button() : x(0), y(0), w(0), h(0), text(nullptr) {}

Button::Button(float x, float y, float w, float h, const char* text) : x(x), y(y), w(w), h(h), text(text) {}

bool Button::update(const Point &mouse, bool mouse_down, bool mouse_prev){
    return mouse.overlap(Rectangle{x, y, x + w, y + h}) && mouse_down && !mouse_prev;
}

void Button::draw(ALLEGRO_FONT *font, ALLEGRO_COLOR normal_color, ALLEGRO_COLOR hover_color,
                    ALLEGRO_COLOR border_color, ALLEGRO_COLOR text_color, const Point &mouse) const{
                        
    bool hover = mouse.overlap(Rectangle{x, y, x + w, y + h});
    ALLEGRO_COLOR fill = hover ? hover_color : normal_color;
    al_draw_filled_rectangle(this->x, this->y, this->x + this->w, this->y + this->h, fill);
    al_draw_rectangle(this->x, this->y, this->x + this->w, this->y + this->h, border_color, 2);

    if(font && this->text){
        float ty = this->y + this->h / 2.0f - al_get_font_line_height(font) / 2.0f;
        al_draw_text(font, text_color, this->x + this->w / 2.0f, ty, ALLEGRO_ALIGN_CENTER, this->text);
    }
}