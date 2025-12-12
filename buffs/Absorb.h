#ifndef ABSORB_H_INCLUDED
#define ABSORB_H_INCLUDED
#include "Buff.h"

class Absorb : public Buff {
    public:
        Absorb(BuffType type);
        void effect();
        void clear_effect();
    private:
        float shield_amount;
        bool used = false;
};

#endif