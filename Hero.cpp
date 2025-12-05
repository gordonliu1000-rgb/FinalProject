#include "Hero.h"
#include "data/DataCenter.h"
#include "data/GIFCenter.h"
#include "algif5/algif.h"
#include "shapes/Rectangle.h"
#include <cmath>
#include "Utils.h"
using namespace std;

namespace HeroSetting{
    static constexpr char gif_root_path[50] = "./assets/gif/crow";
    static constexpr char gif_postfix[][10] = {
        "Left",
        "Right",
        "Up",
        "Down"
    };
    static constexpr int init_ATK = 10;
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
    float start_x = DC->game_field_length;      // 例：地圖中間或左上某點
    float start_y = DC->game_field_length;
    //Hitbox
    shape.reset(new Rectangle{start_x, start_y, start_x + gif->width, start_y + gif->height});

    atk = HeroSetting::init_ATK;
    speed = HeroSetting::init_SPEED;


    //buff initialize
    buffs.emplace_back(Buff::create_buff(BuffType::SPEED));
    buffs[0]->reset_duration();//測試用，給一個speed buff
}

void Hero::draw(){
    GIFCenter *GIFC = GIFCenter::get_instance();
    ALGIF_ANIMATION *gif = GIFC->get(gifPath[state]);
    algif_draw_gif(gif,
                shape->center_x() - gif->width / 2,
                shape->center_y() - gif->height / 2,
                0);
}

void Hero::update(){
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
        shape->update_center_x(shape->center_x() + dx * speed);
        shape->update_center_y(shape->center_y() + dy * speed);
    }

    for(auto &buff:buffs){
        buff->update(); // 每個buff隨hero update
    }

}