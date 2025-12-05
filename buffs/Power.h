#ifndef POWER_H_INCLUDED
#define POWER_H_INCLUDED
#include "Buff.h"

class Power : public Buff {
public:
    Power(BuffType type) : Buff{BuffType::POWER}{
        DataCenter *DC = DataCenter::get_instance();
        init_attack = DC->hero->atk;
        init_duration = 600; 
        duration = 0;
    }
    ~Power(){};
    void effect();
    void clear_effect();
private:
    double init_attack;
};

#endif