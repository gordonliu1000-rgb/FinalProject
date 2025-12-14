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
<<<<<<< HEAD
private:
    float factor = 2.0;
    double init_attack;
=======
>>>>>>> 160bf07cca04d884b0bf25760b10d821af972e27
};

#endif