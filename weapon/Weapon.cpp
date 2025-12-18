#include "Weapon.h"
#include "../Hero.h"
#include "../Utils.h"
#include <cmath>
#include "../data/DataCenter.h"
#include "../Utils.h"

void Weapon::update(const Hero &hero, float dt){
    angle += angular_speed * dt;
    dmg = hero.atk;
    
    
    float hx = hero.x();
    float hy = hero.y();
    float wx = hx + radius * std::cos(angle);
    float wy = hy + radius * std::sin(angle);

    shape -> update_center_x(wx);
    shape -> update_center_y(wy);
}