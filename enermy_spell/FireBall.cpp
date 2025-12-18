#include "../data/ImageCenter.h"
#include "../data/DataCenter.h"
#include "../Utils.h"
#include <cmath>
#include "FireBall.h"
#include "../Hero.h"

std::map<FireBall::State, std::vector<ALLEGRO_BITMAP*>> FireBall::img;
// 初始化照片
void FireBall::init_img(){
    ImageCenter *IC = ImageCenter::get_instance();
    char buffer[60];
    for(int i=0;i<4;i++){
        sprintf(
            buffer,
            "./assets/image/enermy_spell/fire_ball/%d.png",
            i
        );
        img[State::FLYING].emplace_back(IC->get(buffer));
    }

    for(int i=0;i<4;i++){
        sprintf(
            buffer,
            "./assets/image/enermy_spell/fire_ball/ex%d.png",
            i
        );
        img[State::EXPLODE].emplace_back(IC->get(buffer));
    }
}

FireBall::FireBall(Object *shooter, const double &atk, EnermySpellType type) : EnermySpell(shooter, atk, type){
    const static DataCenter *DC = DataCenter::get_instance();
    const float &des_x = DC->hero->shape->center_x();
    const float &des_y = DC->hero->shape->center_y();
    des.reset(new Point{des_x, des_y});
    const double &start_x = shooter->shape->center_x();
    const double &start_y = shooter->shape->center_y(); 
    const float &h = al_get_bitmap_width(img[state][0]) * 0.8;
    const float &w = al_get_bitmap_height(img[state][0]) * 0.8;
    bitmap_angle = std::atan2(des_y - start_y, des_x - start_x) + ALLEGRO_PI/2;// 初始化火球面對方向
    shape.reset(new Rectangle{
        start_x - w/2,
        start_y - h/2,
        start_x + w/2,
        start_y + h/2
    });
    const float &dx = des_x - start_x;
    const float &dy = des_y - start_x;
    const double &distance = std::sqrt(dx*dx + dy*dy);
    dir_x = dx / distance;
    dir_y = dy/ distance;
}

void FireBall::update(){
    if(end) return;
    if(bitmap_switch_counter!=0){
        bitmap_switch_counter--;
    }
    else{
        bitmap_img_id = (bitmap_img_id + 1) % (img[state].size());
    }
    const static DataCenter *DC = DataCenter::get_instance();
    switch(state){
        case State::FLYING:{
            if(shape->overlap(*des) || shape->overlap(*DC->hero->shape)){
                state = State::EXPLODE;
                const float &h = al_get_bitmap_width(img[state][0]);
                const float &w = al_get_bitmap_height(img[state][0]);
                const float &x = shape->center_x();
                const float &y = shape->center_y();
                shape.reset(new Rectangle{
                    x - w/2,
                    y - h/2,
                    x + w/2,
                    y + h/2
                });
                bitmap_switch_counter = 0;
                break;
            }

            // 更新位置
            float fspeed = speed * 1/DC->FPS;
            float new_x = shape->center_x() + dir_x * fspeed;
            float new_y = shape->center_y() + dir_y * fspeed;
            shape->update_center_x(new_x);
            shape->update_center_y(new_y);

            break;
        }
        case State::EXPLODE:{
            if(shape->overlap(*DC->hero->shape)){
                DC->hero->hurt(atk);   
            }
            if((std::size_t)bitmap_img_id==img[state].size()-1 && bitmap_switch_counter==0){
                end = true;
            }
            break;
        }
        default:{
            GAME_ASSERT(0, "IMPOSSIBLE FIREBALL STATE\n");
        }
    }
}

void FireBall::draw(){
    if(end) return;
    static const float &h = al_get_bitmap_width(img[state][0]);
    static const float &w = al_get_bitmap_height(img[state][0]);
    switch(state){
        case State::FLYING:{
            al_draw_rotated_bitmap(
                img[state][bitmap_img_id],
                w/2, h/2,
                shape->center_x(), shape->center_y(),
                bitmap_angle,
                0
            );
        }
        case State::EXPLODE:{
            al_draw_bitmap(
                img[state][bitmap_img_id],
                shape->center_x() - w/2,
                shape->center_y() - h/2,
                0
            );
        }
        default :{
            GAME_ASSERT(0, "Unknown FireBall state\n");
        }
    }
}