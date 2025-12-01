#include "Hero.h"
#include "data/DataCenter.h"
#include "data/GIFCenter.h"
#include "algif5/algif.h"
#include "shapes/Rectangle.h"
#include <cmath>
using namespace std;

namespace HeroSetting{
    static constexpr char gif_root_path[50] = "./assets/gif/crow";
    static constexpr char gif_postfix[][10] = {
        "Left",
        "Right",
        "Up",
        "Down"
    };
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

    // 設定 hero 在「地圖世界」裡的起點，不要用 window_width/2 那種螢幕座標
    float start_x = DC->game_field_length;      // 例：地圖中間或左上某點
    float start_y = DC->game_field_length;

    shape.reset(new Rectangle{start_x, start_y, start_x + gif->width, start_y + gif->height});
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


    // 2. 用 hero 中心來更新攝影機位置（世界座標 -> 視窗中心）
    float hero_cx = shape->center_x();
    float hero_cy = shape->center_y();

    DC->camera_x = hero_cx - DC->window_width  / 2.0f;
    DC->camera_y = hero_cy - DC->window_height / 2.0f;

    // 3. 限制攝影機不要超出地圖邊界（假設地圖是 game_field_length*2）
    float map_w = DC->game_field_length * 2.0f;
    float map_h = DC->game_field_length * 2.0f;

    if (DC->camera_x < 0) DC->camera_x = 0;
    if (DC->camera_y < 0) DC->camera_y = 0;
    if (DC->camera_x > map_w - DC->window_width)
        DC->camera_x = map_w - DC->window_width;
    if (DC->camera_y > map_h - DC->window_height)
        DC->camera_y = map_h - DC->window_height;

}
//|| DC->key_state[ALLEGRO_KEY_W]