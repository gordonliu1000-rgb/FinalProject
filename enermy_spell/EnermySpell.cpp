#include "EnermySpell.h"
#include "FireBall.h"
#include "../Utils.h"

std::unique_ptr<EnermySpell> EnermySpell::create_spell(Object *shooter, const double &atk, EnermySpellType type){
    switch(type){
        case EnermySpellType::FIREBALL:{

        }
        case EnermySpellType::LASER:{

        }
        default:{
            return nullptr;
        }
    }
}

void EnermySpell::init(){
    FireBall::init_img();
}
