#ifndef SPEEDPOISON_H_INCLUDED
#define SPEEDPOISON_H_INCLUDED
#include "Buff.h"
class DataCenter;

class Speed : public Buff {
public:
    Speed(BuffType type);
    ~Speed(){};
    
    void effect();
    void clear_effect();
private:
    double init_speed;
};

#endif