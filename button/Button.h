#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "../shapes/Point.h"
#include "../shapes/Rectangle.h"

class Button{
    public:
        Button();
        Button(float x, float y, float w, float h, const char* text);
        bool update(const Point &mouse, bool mouse_down, bool mouse_prev);
        void draw(ALLEGRO_FONT *font, ALLEGRO_COLOR normal_color, ALLEGRO_COLOR hover_color,
                    ALLEGRO_COLOR border_color, ALLEGRO_COLOR text_color, const Point &mouse) const;
        void draw_bitmap(ALLEGRO_BITMAP *bitmap, const Point &mouse, float hover) const;
                    
    private:
        float x, y, w, h;
        const char* text = nullptr;
};


#endif