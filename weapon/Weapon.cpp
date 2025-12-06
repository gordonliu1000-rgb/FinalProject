#include "Weapon.h"
#include "../Hero.h"
#include <cmath>

void Weapon::update(const Hero &hero, float dt){
    if(cooldown >= 0.0) cooldown -= dt;
    angle += angular_speed * dt;

    float hx = hero.x();
    float hy = hero.y();
    float wx = hx + radius * std::cos(angle);
    float wy = hy + radius * std::sin(angle);

    shape -> update_center_x(wx);
    shape -> update_center_y(wy);
}