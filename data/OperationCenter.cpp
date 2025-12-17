#include "OperationCenter.h"
#include "DataCenter.h"
#include "SoundCenter.h"
#include "../weapon/Weapon.h"
#include "../buffs/Buffitem.h"
#include "../Random.h"
#include "../monsters/Monster.h"
#include "../towers/Tower.h"
#include "../towers/Bullet.h"
#include "../Player.h"
#include "../Hero.h"
#include "../Utils.h"
#include "../mobs/Mob.h"
#include <algorithm>

void OperationCenter::update() {
	_update_buffitem_pickup();
	_update_buffitem_spawn();
	_update_mob_spawn();
	_update_mob();
	_update_mob_weapon();
	_update_bump_dmg();
}

bool inbounds(int x, int y, int cols, int rows){
	return x >= 0 && x < rows && y >= 0 && y < cols;
}
void OperationCenter::_update_mob_weapon(){

	DataCenter *DC = DataCenter::get_instance();
	std::vector<Weapon*> current_weapons;
	current_weapons.reserve(DC->hero->weapons.size());
	for(auto &w : DC->hero->weapons){
		if(w) current_weapons.emplace_back(w.get());
	}
	static int grid_x = 0, grid_y = 0;
	static const int dx[8] = {1, -1, 0,  0,  1,  1, -1, -1};
	static const int dy[8] = {0,  0, 1, -1,  1, -1,  1, -1};
	for(auto *weapon : current_weapons){
		for(int i=0;i<8;i++){
			grid_x = weapon->shape->center_x()/DC->cell_width + dx[i];
			grid_y = weapon->shape->center_y()/DC->cell_width + dy[i]; // find the target mobs
			if(inbounds(grid_x, grid_y, DC->grids.size(), DC->grids[0].size())){
				for(auto &mob:DC->grids[grid_y][grid_x].mobs){
					if(mob->shape->overlap(*(weapon->shape))){
						//debug_log("mob hurt1\n");
						mob->hurt(weapon->get_dmg());
						//debug_log("mob hurt2\n");
						if(DC->hero->levelup){
							debug_log("hero level up stop update\n");
							return;
						}
					}
				}
			}
		}
	}
}


/*
std::vector<std::unique_ptr<Weapon>> &weapons = DC->hero->weapons;
	static int grid_x = 0, grid_y = 0;
	static const int dx[9] = {1, -1, 0,  0,  0, 1,  1, -1, -1};
	static const int dy[9] = {0,  0, 1, -1,  0, 1, -1,  1, -1};
	for(auto &weapon:weapons){
		for(int i=0;i<9;i++){
			grid_x = weapon->shape->center_x()/DC->cell_width + dx[i];
			grid_y = weapon->shape->center_y()/DC->cell_width + dy[i]; // find the target mobs
			if(DC->grid_inbounds(grid_x, grid_y, DC->grids.size(), DC->grids[0].size())){
				for(auto &mob:DC->grids[grid_y][grid_x].mobs){
					if(mob->shape->overlap(*(weapon->shape))){
						mob->hurt(weapon->get_dmg());
					}
				}
			}
		}
	}
<<<<<<< HEAD
*/

/*void OperationCenter::_update_mob_weapon() {
    DataCenter *DC = DataCenter::get_instance();
    debug_log("weapon\n");

    // 1. 先複製一份當前武器指標（snapshot）
    std::vector<Weapon*> current_weapons;
    current_weapons.reserve(DC->hero->weapons.size());
    for (auto &w : DC->hero->weapons) {
        if (w) current_weapons.push_back(w.get());
    }

    static int grid_x = 0, grid_y = 0;
    static const int dx[8] = {1, -1, 0,  0,  1,  1, -1, -1};
    static const int dy[8] = {0,  0, 1, -1,  1, -1,  1, -1};

    // 2. 用 snapshot 來做碰撞，不再直接遍歷 hero->weapons
    for (Weapon *weapon : current_weapons) {
        if (!weapon || !weapon->shape) continue;

        for (int i = 0; i < 8; ++i) {
            grid_x = weapon->shape->center_x() / DC->cell_width + dx[i];
            grid_y = weapon->shape->center_y() / DC->cell_width + dy[i];

            if (inbounds(grid_x, grid_y, DC->grids.size(), DC->grids[0].size())) {
                for (auto &mob : DC->grids[grid_y][grid_x].mobs) {
                    if (mob && mob->shape &&
                        mob->shape->overlap(*(weapon->shape))) {
                        mob->hurt(weapon->get_dmg());
                    }
                }
            }
        }
    }
=======
>>>>>>> a6edc0fbf093f1443f43641e6ee3140de2a32266
}
*/

void OperationCenter:: _update_mob_spawn(){
	DataCenter *DC = DataCenter::get_instance();
	if(DC->mobs.size() == 200) return;
	const int init_timer = 120;
	static int timer = init_timer;
	if(timer > 0) timer--;
	else {
		int rn = Random::range(0, static_cast<int>(MobType::MOBTYPE_MAX)-1);
		MobType type = static_cast<MobType>(rn); 
		DC->mobs.emplace_back(Mob::create_mob(type));

		timer = init_timer;
	}
}

void OperationCenter::_update_mob(){
	DataCenter *DC = DataCenter::get_instance();
	std::vector<std::unique_ptr<Mob>> &mobs = DataCenter::get_instance()->mobs;
	for(auto &grids:DC->grids){
		for(auto &cell:grids){
			cell.mobs.clear();
		}
	}
	static int gird_x = 0, grid_y = 0;
	for(size_t i=0;i<mobs.size();){
		if(mobs[i]->die){
			std::swap(mobs[i], mobs.back());
			mobs.pop_back();
		}
		else{
			mobs[i]->update();
			gird_x = mobs[i]->shape->center_x()/DC->cell_width;
			grid_y = mobs[i]->shape->center_y()/DC->cell_width;
			DC->grids[grid_y][gird_x].mobs.push_back(mobs[i].get());
			i++;
		}

	}


	/*
	for(auto m=mobs.begin();m!=mobs.end();){
		if((*m)->die){
			m = mobs.erase(m);
		}
		else{
			(*m)->update();
			gird_x = (*m)->shape->center_x()/DC->cell_width;
			grid_y = (*m)->shape->center_y()/DC->cell_width;
			DC->grids[grid_y][gird_x].mobs.push_back((*m).get());
			m++;
		}

	}
	*/
}

void OperationCenter::_update_bump_dmg(){
	DataCenter *DC = DataCenter::get_instance();
	Hero *hero = DC->hero;
	static int grid_x = 0, grid_y = 0;
	static const int dx[9] = {1, -1, 0,  0,  0, 1,  1, -1, -1};
	static const int dy[9] = {0,  0, 1, -1,  0, 1, -1,  1, -1};
	for(int i=0;i<9;i++){
		grid_x = hero->shape->center_x()/DC->cell_width + dx[i];
		grid_y = hero->shape->center_y()/DC->cell_width + dy[i]; // find the target mobs
		if(DC->grid_inbounds(grid_x, grid_y, DC->grids[0].size(), DC->grids.size())){
			for(auto &mob:DC->grids[grid_y][grid_x].mobs){
				if(hero->shape->overlap(*(mob->shape))){
					hero->hurt(mob->atk);
				}
			}
		}
	}
}

void OperationCenter::_update_buffitem_pickup(){
	DataCenter *DC = DataCenter::get_instance();
	Hero *hero = DC->hero;
	auto &items = DC->buff_items;

	for(size_t i = 0; i < items.size();){
		Buffitem *item = items[i].get();
		if(hero->shape->overlap(*(item->shape))){
			BuffType type = item->get_type();
			bool search = false;
			for(auto *b : hero->buffs){
				if(b->get_type() == type){
					b->reset_duration();
					search = true;
					break;
				}
			}
			if(!search){
				Buff *buff = Buff::create_buff(item->get_type());
				if(buff){
					buff->reset_duration();
					hero->buffs.emplace_back(buff);
				}
			}
			debug_log("hero picked buff type=%d\n", (int)type);
			items.erase(items.begin() + i);
		} else{
			++i;
		}
	}
}

void OperationCenter::_update_buffitem_spawn(){
	DataCenter *DC = DataCenter::get_instance();
	static int spawn_counter = 0;
	spawn_counter++;

	if(spawn_counter >= 300){
		spawn_counter = 0;
		
		int t = Random::range(0, 1);
		BuffType type = (t == 0 ? BuffType::SPEED : BuffType::POWER);

		float x = Random::range(0.0, (float)DC->game_field_height);
		float y = Random::range(0.0, (float)DC->game_field_width);

		DC->buff_items.emplace_back(std::make_unique<Buffitem>(type, Point{x, y}));
		debug_log("spawn buff at x=%.1f y=%.1f type=%d\n", x, y, (int)type);
	}
	
}





// draw
void OperationCenter::draw() {
	_draw_buffitem();
	_draw_mob();
}

void OperationCenter::_draw_monster() {
	std::vector<Monster*> &monsters = DataCenter::get_instance()->monsters;
	for(Monster *monster : monsters)
		monster->draw();
}

void OperationCenter::_draw_tower() {
	std::vector<Tower*> &towers = DataCenter::get_instance()->towers;
	for(Tower *tower : towers)
		tower->draw();
}

void OperationCenter::_draw_towerBullet() {
	std::vector<Bullet*> &towerBullets = DataCenter::get_instance()->towerBullets;
	for(Bullet *towerBullet : towerBullets)
		towerBullet->draw();
}

void OperationCenter::_draw_buffitem(){
	DataCenter *DC = DataCenter::get_instance();
	for(auto &item : DC->buff_items){
		item->draw();
	}
}

void OperationCenter::_draw_mob(){
	std::vector<std::unique_ptr<Mob>> &mobs = DataCenter::get_instance()->mobs;
	for(auto &mob:mobs){
		mob->draw();
	}
}





