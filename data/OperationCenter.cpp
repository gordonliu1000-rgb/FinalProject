#include "OperationCenter.h"
#include "DataCenter.h"
#include "SoundCenter.h"
#include "../weapon/Weapon.h"
#include "../buffs/Buffitem.h"
#include "../Random.h"
#include "../Hero.h"
#include "../Utils.h"
#include "../mobs/Mob.h"
#include <algorithm>

void OperationCenter::update() {
	_update_buffitem_pickup();
	//_update_buffitem_spawn();
	_update_mob_spawn();
	_update_mob();
	_update_mob_weapon();
	_update_bump_dmg();
	_update_enemy_spell();

	_update_exp_pickup();
	_update_exp_to_grid();
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
	static const int dx[9] = {0, 1, -1, 0,  0,  1,  1, -1, -1};
	static const int dy[9] = {0, 0,  0, 1, -1,  1, -1,  1, -1};
	for(auto *weapon : current_weapons){
		for(int i=0;i<9;i++){
			grid_x = weapon->shape->center_x()/DC->cell_width + dx[i];
			grid_y = weapon->shape->center_y()/DC->cell_width + dy[i]; // find the target mobs
			if(DataCenter::grid_inbounds(grid_x, grid_y, DC->grids.size(), DC->grids[0].size())){
				for(auto &idx:DC->grids[grid_y][grid_x].mobs){
					if(DC->mobs[idx].get()->shape->overlap(*(weapon->shape))){
						//debug_log("mob hurt1\n");
						DC->mobs[idx].get()->hurt(weapon->get_dmg());
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

void OperationCenter:: _update_mob_spawn(){
	DataCenter *DC = DataCenter::get_instance();
	if(DC->mobs.size() >= 300) return;
	const int init_timer = 30;
	static int timer = init_timer;
	if(timer > 0) timer--;
	else {
		timer = init_timer;
		int rn = Random::range(0, static_cast<int>(MobType::MOBTYPE_MAX)-1);
		MobType type = static_cast<MobType>(rn);
		if(DC->next_mob_idx >= DC->mobs.size()) DC->mobs.emplace_back(Mob::create_mob(type));
		else {
			DC->mobs[DC->next_mob_idx] = Mob::create_mob(type);
		}
		DC->next_mob_idx++;
		// for(size_t i=0;i<DC->mobs.size();i++){
		// 	if(DC->mobs[i]->die){
		// 		DC->mobs[i] = Mob::create_mob(type);
		// 		return;
		// 	}
		// }
		
	}
}

void OperationCenter::_update_mob(){
	DataCenter *DC = DataCenter::get_instance();
	if((int)DC->play_time >= 1800 && (int)(DC->play_time) % 1800 == 0) {Mob::mob_level_up();debug_log("mob lvl up!\n");}
	std::vector<std::unique_ptr<Mob>> &mobs = DataCenter::get_instance()->mobs;
	
	for (auto& m : mobs) m->update();

	for(auto &grids:DC->grids){
		for(auto &cell:grids){
			cell.mobs.clear();
		}
	}

	for(size_t i=0;i<DC->next_mob_idx;){
		if(mobs[i]->die){
			if(!mobs[i]->swapped){
				mobs[i]->swapped = true;
				std::swap(mobs[DC->next_mob_idx-1], mobs[i]);
				DC->next_mob_idx--;
			}
			continue;
		}
		static int grid_x = 0, grid_y = 0;
		grid_x = mobs[i]->shape->center_x()/DC->cell_width;
		grid_y = mobs[i]->shape->center_y()/DC->cell_width;
		if(grid_x < 0) grid_x = 0;
		if(grid_y < 0) grid_y = 0;
		DC->grids[grid_y][grid_x].mobs.push_back(i);
		i++;
	}

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
			for(auto &idx:DC->grids[grid_y][grid_x].mobs){
				if(hero->shape->overlap(*(DC->mobs[idx].get()->shape)) && !DC->mobs[idx].get()->explosive){
					hero->hurt(DC->mobs[idx].get()->atk);
				}
			}
		}
	}
}

void OperationCenter::_update_enemy_spell(){
	std::vector<std::unique_ptr<EnemySpell>> &spells = DataCenter::get_instance()->enemy_spells;
	for(size_t i=0;i<spells.size();){
		if(spells[i]->end){
			std::swap(spells[i], spells.back());
			spells.pop_back();
		}
		else{
			spells[i]->update();
			i++;
		}
	}
}

void OperationCenter::drop_exp(const double &x, const double &y){
	DataCenter *DC = DataCenter::get_instance();
	if(DC->next_exp_idx >= 300) return;
	std::size_t &next_idx = DC->next_exp_idx;
	if(next_idx >= DC->exps.size()){
		DC->exps.emplace_back(std::make_unique<Exp>(x, y));
		next_idx ++;
		return;
	}
	while(!DC->exps[next_idx].get()->picked){
		next_idx++;
	}
	DC->exps[next_idx] = std::make_unique<Exp>(x, y);
	next_idx++;
}	

void OperationCenter::_update_exp_to_grid(){
	DataCenter *DC = DataCenter::get_instance();
	for(auto &grids:DC->grids){
		for(auto &cell:grids){
			cell.exps.clear();
		}
	}

	for(std::size_t i=0;i<DC->next_exp_idx;i++){
		if(DC->exps[i]->picked) continue;
		static int grid_x = 0, grid_y = 0;
		grid_x = DC->exps[i]->shape->center_x()/DC->cell_width;
		grid_y = DC->exps[i]->shape->center_y()/DC->cell_width;
		if(grid_x < 0) grid_x = 0;
		if(grid_y < 0) grid_y = 0;
		DC->grids[grid_y][grid_x].exps.push_back(i);
	}
}

void OperationCenter::_update_exp_pickup(){
	DataCenter *DC = DataCenter::get_instance();
	std::vector<std::unique_ptr<Exp>> &exps = DC->exps;
	Hero *hero = DC->hero;
	const int &cell_width = DC->cell_width;
    int min_grid_x = std::max((int)(hero->shape->center_x() - (int)hero->exp_pickup_radius)/cell_width, 0);
    int min_grid_y = std::max((int)(hero->shape->center_y() - (int)hero->exp_pickup_radius)/cell_width, 0);
    int max_grid_x = std::min((int)(hero->shape->center_x() + (int)hero->exp_pickup_radius)/cell_width, (int)DC->grids[0].size()-1);
    int max_grid_y = std::min((int)(hero->shape->center_y() + (int)hero->exp_pickup_radius)/cell_width, (int)DC->grids.size()-1);
	
	for(int grid_y=min_grid_y;grid_y<=max_grid_y;grid_y++){
		for(int grid_x=min_grid_x;grid_x<=max_grid_x;grid_x++){
			if(DC->grid_inbounds(grid_x, grid_y, DC->grids[0].size(), DC->grids.size())){
				for(auto &idx:DC->grids[grid_y][grid_x].exps){
					if(hero->exp_pickup_range->overlap(*(DC->exps[idx].get()->shape))){
						exps[idx].get()->chasing = true;
					}
				}
			}
		}
	}

	static const float exp_speed_factor = 0.2;
	for(std::size_t idx=0;idx < DC->next_exp_idx;idx++){
		if(exps[idx].get()->picked) continue;
		if(!exps[idx].get()->chasing) continue;
		if(hero->shape->overlap(*(exps[idx].get()->shape))) {
			hero->gain_exp(exps[idx].get()->get_val());
			exps[idx].get()->picked = true;
			exps[idx].get()->chasing = false;
			std::swap(exps[idx], exps[DC->next_exp_idx-1]);
			DC->next_exp_idx--;
			continue;
		}
		float dx = hero->shape->center_x() - exps[idx].get()->shape->center_x();
		float dy = hero->shape->center_y() - exps[idx].get()->shape->center_y();
		exps[idx].get()->shape->update_center_x(exps[idx].get()->shape->center_x() + dx*exp_speed_factor);
		exps[idx].get()->shape->update_center_y(exps[idx].get()->shape->center_y() + dy*exp_speed_factor);
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
			std::swap(items.back(), items[i]);
			items.pop_back();
		} else{
			++i;
		}
	}
}





// draw
void OperationCenter::draw() {
	_draw_buffitem();
	_draw_mob();
	_draw_enemy_spell();
	_draw_exp();
}

void OperationCenter::_draw_exp(){
	DataCenter *DC = DataCenter::get_instance();
	for(auto &exp:DC->exps){
		exp->draw();
	}
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

void OperationCenter::_draw_enemy_spell(){
	std::vector<std::unique_ptr<EnemySpell>> &spells = DataCenter::get_instance()->enemy_spells;
	for(auto &spell:spells){
		spell->draw();
	}
}





