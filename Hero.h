#ifndef HERO_H_INCLUDED
#define HERO_H_INCLUDED
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "Object.h"
#include "Creature.h"
#include "buffs/Buff.h"
#include <vector>
#include "weapon/Weapon.h"
#include "spell/Spell.h"

class Buff;

enum class HeroState{
    LEFT,
    RIGHT,
    FRONT,
    BACK,
    HEROSTATE_MAX
};

class Hero : public Creature{
    public:
        void init();
        void update();
        void draw();
        void hurt(float dmg);
        void gain_exp(int amount);
        void level_up();
        void gain_shield(float amount);
        ~Hero();
        float x() const {return shape->center_x();}
        float y() const {return shape->center_y();}
        std::vector<std::unique_ptr<Weapon>> weapons; //store the weapon that hero have
        std::vector<std::unique_ptr<Spell>> spells;

    private:
        HeroState state = HeroState::FRONT;
        std::map<HeroState, std::string> gifPath;
    public:
        std::vector<Buff*>buffs;
        float hp; //目前血量
        float max_hp; //最大血量
        int level;
        float exp; // 經驗值
        float exp_to_next; //生到下一級所需的經驗值
        float atk;
        float init_atk;
        float shield;
        float max_shield;
        double speed;
        int hurt_cooldown = 0;
        bool levelup = false;
        
};
#endif