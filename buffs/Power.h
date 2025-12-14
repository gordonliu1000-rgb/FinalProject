#ifndef POWER_H_INCLUDED
#define POWER_H_INCLUDED
#include "Buff.h"
class DataCenter;

class Power : public Buff {
public:
    Power(BuffType type);
    ~Power(){};
    void effect();
    void clear_effect();
// private:
//     double init_attack;
//     bool init_attack_saved = false;
};

#endif