#include "EnemySpell.h"
#include "FireBall.h"
#include "../Utils.h"
#include "../shapes/Point.h"

std::unique_ptr<EnemySpell> EnemySpell::create_spell(Object *shooter, const double &atk, EnemySpellType type){
    switch(type){
        case EnemySpellType::FIREBALL:{
            return std::make_unique<FireBall>(shooter, atk, EnemySpellType::FIREBALL);
        }
        case EnemySpellType::LASER:{

        }
        default:{
            return nullptr;
        }
    }
}

void EnemySpell::init(){
    FireBall::init_img();
}
