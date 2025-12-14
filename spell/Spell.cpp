#include "Spell.h"
#include <memory>
#include "../Utils.h"
#include "Thunder.h"

void Spell::init(){
    Thunder::init_img();
}

std::unique_ptr<Spell> Spell::create_spell(SpellType type){
    switch(type){
        case SpellType::THUNDER:{
            return std::make_unique<Thunder>(type);
        }
        default :{
            GAME_ASSERT(0, "Unknow mob type");
            return nullptr;
        }
    }
};