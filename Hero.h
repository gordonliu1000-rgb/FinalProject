#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED
#include <string>
#include <map>
#include "Object.h"
#include "buffs/Buff.h"
#include <vector>


class Buff;

enum class HeroState{
    LEFT,
    RIGHT,
    FRONT,
    BACK,
    HEROSTATE_MAX
};

class Hero : public Object{
    public:
        void init();
        void update();
        void draw();
    private:
        HeroState state = HeroState::FRONT;
        std::map<HeroState, std::string> gifPath;
    public:
        std::vector<Buff*>buffs;
        int hp;
        int atk;
        double speed;
        
};
#endif