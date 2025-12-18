#include "Hero.h"
#include "data/DataCenter.h"
#include "data/GIFCenter.h"
#include "algif5/algif.h"
#include "shapes/Rectangle.h"
#include "weapon/Sword.h"
#include "weapon/Lightball.h"
#include <cmath>
#include "Utils.h"
using namespace std;

const float PI = 3.14159;
namespace HeroSetting{
    static constexpr char gif_root_path[50] = "./assets/gif/crow";
    static constexpr char gif_postfix[][10] = {
        "Left",
        "Right",
        "Up",
        "Down"
    };
    static constexpr float init_ATK = 5.0;
    static constexpr double init_SPEED = 5;

} 


void Hero::init() {
    for (size_t type = 0; type < static_cast<size_t>(HeroState::HEROSTATE_MAX); ++type) {
        char buffer[50];
        sprintf(buffer, "%s/crow_%s.gif",
                HeroSetting::gif_root_path,
                HeroSetting::gif_postfix[static_cast<int>(type)]);
        gifPath[static_cast<HeroState>(type)] = string{buffer};
    }

    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    DataCenter *DC = DataCenter::get_instance();

    // 設定 hero 在「地圖世界」裡的起點
    float start_x = DC->game_field_height / 2;      // 例：地圖中間或左上某點
    float start_y = DC->game_field_height / 2;
    //Hitbox
    max_hp = 100;
    hp = max_hp;
    level = 1;
    exp = 0;
    exp_to_next = 100;

    shape.reset(new Rectangle{start_x, start_y, start_x + gif->width, start_y + gif->height});
    atk = HeroSetting::init_ATK;
    init_atk = HeroSetting::init_ATK;
    speed = HeroSetting::init_SPEED;

    weapons.emplace_back(std::make_unique<Sword>(80.0f, 4.0f));

    Spell::init();
    spells.emplace_back(Spell::create_spell(SpellType::THUNDER));
}

void Hero::draw(){
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    algif_draw_gif(gif,
                shape->center_x() - gif->width / 2,
                shape->center_y() - gif->height / 2,
                0);
    for(auto &w : weapons){
        w -> draw();
    }

    for(auto &s : spells){
        s->draw();
    }
}

void Hero::update(){
    levelup = false;
    DataCenter *DC = DataCenter::get_instance();
    float dx = 0, dy = 0;
    if(DC->key_state[ALLEGRO_KEY_UP] || DC->key_state[ALLEGRO_KEY_W]){
        state = HeroState::BACK;
        dy -= 1;
    }
    if(DC->key_state[ALLEGRO_KEY_LEFT] || DC->key_state[ALLEGRO_KEY_A]){
        state = HeroState::LEFT;
        dx -= 1;
    }
    if(DC->key_state[ALLEGRO_KEY_DOWN] || DC->key_state[ALLEGRO_KEY_S]){
        state = HeroState::FRONT;
        dy += 1;
    }
    if(DC->key_state[ALLEGRO_KEY_RIGHT] || DC->key_state[ALLEGRO_KEY_D]){
        state = HeroState::RIGHT;
        dx += 1;
    }
    //normalization
    float length = sqrt(dx*dx + dy*dy);
    if(length > 0){
        dx /= length;
        dy /= length;
        int new_x = shape->center_x() + dx * speed;
        int new_y = shape->center_y() + dy * speed;
        
        if(shape->center_y() + dy * speed > DC->game_field_height - DC->wall_width){
            new_y = DC->game_field_height - DC->wall_width;
        }
        else if(shape->center_y() + dy * speed < 92.0){
            new_y = 92.0;
        }

        if(shape->center_x() + dx * speed > DC->game_field_width - DC->wall_width){
            new_x = DC->game_field_width - DC->wall_width;
        }
        else if(shape->center_x() + dx * speed < DC->wall_width){
            new_x = DC->wall_width;
        }
        
        shape->update_center_x(new_x);
        shape->update_center_y(new_y);
    }
    float dt = 1.0f / DC->FPS;
    for(auto &w : weapons){
        w -> update(*this, dt);
        //debug_log("the weapon attack is %.2f\n", w->get_dmg());
    }

    for(auto &buff:buffs){
        buff->update(); // 每個buff隨hero update
    }

    for(auto &s : spells){
        s->update();
    }

    if(hurt_cooldown > 0) --hurt_cooldown;

}

void Hero::hurt(float dmg){
    if(hurt_cooldown > 0) return;

    if(shield > 0.0){
        float shield_absorb = std::min(shield, dmg);
        shield -= shield_absorb;
        dmg -= shield_absorb;
    }

    if(dmg > 0){
        hp -= dmg;
        if(hp < 0) hp = 0;
        damage_hp = 18;
    }

    hurt_cooldown = 120;

}

void Hero::gain_shield(float amount){
    shield = amount;
    max_shield = amount;
}

void Hero::gain_exp(int amount){
    levelup = false;
    exp += amount;
    while(exp >= exp_to_next){
        exp -= exp_to_next;
        level_up();
        debug_log("hero lvl up! atk : %lf\n", atk);
    }
}

void Hero::level_up(){
    level++;
    max_hp += 20;
    init_atk += 20;
    atk = init_atk;
    exp_to_next = static_cast<int>(exp_to_next * 1.2);
    levelup = true;

    if(level % 5 == 0 || level % 8 == 0){
        debug_log("level up\n");
        weapons.clear();
        int sword_count = 1 + (level / 5);
        int light_count = level / 8;

        for(int i=0; i<sword_count; i++){
            float angle0 = 2.0 * PI * i / sword_count;
            auto sword = std::make_unique<Sword>(80.0, 4.0);
            sword->set_angle(angle0);
            weapons.emplace_back(std::move(sword));
        }

        for(int i=0; i<light_count; i++){
            float angle0 = 2.0 * PI * i / light_count;
            auto lightball = std::make_unique<Lightball>(120.0, 6.0);
            lightball->set_angle(angle0);
            weapons.emplace_back(std::move(lightball));
        }

        debug_log("weapon reset done\n");
    }
}

Hero::~Hero(){
    for(auto &buff:buffs){
        buff->clear_effect(); // 清除buff效果
        delete buff;
    }
}