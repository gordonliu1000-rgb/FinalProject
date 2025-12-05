#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "Object.h"
#include "buffs/Buff.h"
#include <vector>
#include "weapon/Weapon.h"

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
        ~Hero();
        float x() const {return shape->center_x();}
        float y() const {return shape->center_y();}
        std::vector<std::unique_ptr<Weapon>> weapons; //store the weapon that hero have

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