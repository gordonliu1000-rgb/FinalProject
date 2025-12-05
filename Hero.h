#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "Object.h"
#include "weapon/Weapon.h"

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

        float x() const {return shape->center_x();}
        float y() const {return shape->center_y();}
        std::vector<std::unique_ptr<Weapon>> weapons; //store the weapon that hero have

    private:
        HeroState state = HeroState::FRONT;
        double speed = 5;
        std::map<HeroState, std::string> gifPath;
};
#endif