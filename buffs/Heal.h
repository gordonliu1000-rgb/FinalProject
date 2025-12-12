#ifndef HEAL_H_INCLUDED
#define HEAL_H_INCLUDED
#include "Buff.h"

class Heal : public Buff{
    public:
        Heal(BuffType type);
        void effect();
        void clear_effect();
    private:
        bool used = false;
        float heal_amount;
};

#endif