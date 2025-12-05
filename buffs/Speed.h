#ifndef SPEEDPOISON_H_INCLUDED
#define SPEEDPOISON_H_INCLUDED
#include "Buff.h"
#include <allegro5/allegro.h>

class Speed : public Buff {
public:
    Speed(BuffType type) : Buff{BuffType::SPEED}{
        DataCenter *DC = DataCenter::get_instance();
        init_speed = DC->hero->speed;
        init_duration = 300; 
        duration = 0;
    }
    ~Speed(){};
    void effect();
    void clear_effect();
private:
    double init_speed;
};

#endif