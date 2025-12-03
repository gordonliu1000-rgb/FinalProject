#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

class Button{
    public:
        Button();
        Button(float x, float y, float w, float h, const char* text);
        bool update(float mx, float my, bool mouse_down, bool mouse_prev);
        void draw(ALLEGRO_FONT *font, ALLEGRO_COLOR normal_color, ALLEGRO_COLOR hover_color,
                    ALLEGRO_COLOR border_color, ALLEGRO_COLOR text_color, float mx, float my) const;
        void set_rect(float x, float y, float w, float h);
        void set_text(const char* text);
        
    private:
        float x_, y_, w_, h_;
        const char* text_ = nullptr;
};


#endif