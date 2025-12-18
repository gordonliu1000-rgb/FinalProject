#include "Thunder.h"
#include "../data/ImageCenter.h"
#include "../data/DataCenter.h"
#include "../data/SoundCenter.h"
#include "../Hero.h"
#include "../mobs/Mob.h"
#include <algorithm>
#include <memory>
#include "../Utils.h"
#include "../Random.h"



std::vector<ALLEGRO_BITMAP*> Thunder::img;
void Thunder::init_img(){
    ImageCenter *IC = ImageCenter::get_instance();
    char buffer[60];
    for(int i=0;i<10;i++){
        sprintf(
            buffer, "./assets/image/spell/thunder/%d.png",
            i
        );
        img.emplace_back(IC->get(buffer));
    }
}


Thunder::Thunder(SpellType type) : Spell{type}{
    GAME_ASSERT(!img.empty(), "Thunder img haven't init!\n");
    DataCenter *DC = DataCenter::get_instance();
    init_cooldown = 300;// 幀
    level = 1;
    atk = (DC->hero->atk)*2;
    num_of_strikes = 3 + level*2;
    strike_radius = al_get_bitmap_width(img[0]);
}

void Thunder::draw(){
    for(auto& s : strikes){
        if(s->end || s->wait_time > 0){
            continue;
        }
        static int bitmap_offset_y = 20;
        al_draw_bitmap(
            img[s->bitmap_img_id],
            s->shape->center_x() - al_get_bitmap_width(img[s->bitmap_img_id])/2,
            s->shape->center_y() - al_get_bitmap_height(img[s->bitmap_img_id])/2 - bitmap_offset_y,
            0
        );
    }
}

void Thunder::update(){
    if(strikes.size() > 0){
        for(auto it=strikes.begin();it!=strikes.end();){
            if((*it)->end){
                it = strikes.erase(it);
            }
            else{
                (*it)->update();
                it++;
            }
        }
        return;
    }
    if(cooldown > 0){
        cooldown--;
        return;
    }
    cooldown = init_cooldown;
    DataCenter *DC = DataCenter::get_instance();
    Hero *hero = DC->hero;
    atk = (hero->atk)*2;
    const int &cell_width = DC->cell_width;
    int min_grid_x = std::max((int)(hero->shape->center_x() - atk_radius)/cell_width, 0);
    int min_grid_y = std::max((int)(hero->shape->center_y() - atk_radius)/cell_width, 0);
    int max_grid_x = std::min((int)(hero->shape->center_x() + atk_radius)/cell_width, (int)DC->grids[0].size()-1);
    int max_grid_y = std::min((int)(hero->shape->center_y() + atk_radius)/cell_width, (int)DC->grids.size()-1);

    struct MobInfo {
        Mob* mob;
        int weight;
    }; 
    std::vector<MobInfo> pool;

    for(int y=min_grid_y;y<=max_grid_y;y++){
        for(int x=min_grid_x;x<=max_grid_x;x++){
            int count = DC->grids[y][x].mobs.size();
            for (auto& mob : DC->grids[y][x].mobs) {
                pool.push_back({ mob, count });
            }
        }
    }

    std::shuffle(pool.begin(), pool.end(), Random::rng);// 決定落雷位置(怪密度較高較易選中)

    for (std::size_t i = 0; i < (std::size_t)num_of_strikes && i < pool.size(); ++i) {
        auto *m = pool[i].mob;
        // 決定落雷偏移量
        double offset_x = Random::range(-static_cast<float>(strike_radius) * 0.3f, static_cast<float>(strike_radius) * 0.3f);
        double offset_y = Random::range(-static_cast<float>(strike_radius) * 0.3f, static_cast<float>(strike_radius) * 0.3f);

        strikes.emplace_back(
            std::make_unique<Strike>(
                m->shape->center_x() + offset_x,
                m->shape->center_y() + offset_y,
                strike_radius,
                i * 20,
                this->atk
            )
        );
        strikes.shrink_to_fit();
    }
}

Strike::Strike(double x, double y, double r, int wait_time, double atk){
    shape.reset(new Circle{x, y, r});
    bitmap_switch_counter = bitmap_switch_freq;
    this->wait_time = wait_time;
    this->atk = atk;
}

void Strike::update(){
    DataCenter *DC = DataCenter::get_instance();
    if(wait_time > 0){
        wait_time--;
        return;
    }
    if(bitmap_switch_counter!=0){
        bitmap_switch_counter--;
        return;
    }
    bitmap_switch_counter = bitmap_switch_freq;
    bitmap_img_id += 1;// to do out of index handle
    if((std::size_t)bitmap_img_id >= Thunder::img.size()){
        end = true;
        return;
    }
    int grid_x = 0, grid_y = 0;
	static const int dx[9] = {1, -1, 0,  0,  0, 1,  1, -1, -1};
	static const int dy[9] = {0,  0, 1, -1,  0, 1, -1,  1, -1};
    if(can_atk && bitmap_img_id==attack_img_id){
        SoundCenter *SC = SoundCenter::get_instance();
	    SC->play("./assets/sound/thunder.wav", ALLEGRO_PLAYMODE_ONCE, 1);
        for(int i=0;i<9;i++){
			grid_x = shape->center_x()/DC->cell_width + dx[i];
			grid_y = shape->center_y()/DC->cell_width + dy[i];
			if(DC->grid_inbounds(grid_x, grid_y, DC->grids.size(), DC->grids[0].size())){
				for(auto &mob:DC->grids[grid_y][grid_x].mobs){
					if(mob->shape->overlap(*shape)){
						mob->hurt(atk);
					}
				}
			}
		}
        can_atk = false;
    }
}

