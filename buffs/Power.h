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
    bool used = false;
    double init_attack;
=======
// private:
//     double init_attack;
//     bool init_attack_saved = false;
>>>>>>> a6edc0fbf093f1443f43641e6ee3140de2a32266
};

#endif