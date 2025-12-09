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

void OperationCenter::update() {
	// Update monsters.
	_update_monster();
	// Update towers.
	_update_tower();
	// Update tower bullets.
	_update_towerBullet();
	// If any bullet overlaps with any monster, we delete the bullet, reduce the HP of the monster, and delete the monster if necessary.
	_update_monster_towerBullet();
	// If any monster reaches the end, hurt the player and delete the monster.
	_update_monster_player();

	//_update_monster_hero();

	_update_monster_weapon();

	_update_buffitem_pickup();

	_update_buffitem_spawn();

	
	_update_mob_spawn();

	_update_mob();

	_update_mob_weapon();
	
}

void OperationCenter::_update_monster() {
	std::vector<Monster*> &monsters = DataCenter::get_instance()->monsters;
	for(Monster *monster : monsters)
		monster->update();
}

void OperationCenter::_update_tower() {
	std::vector<Tower*> &towers = DataCenter::get_instance()->towers;
	for(Tower *tower : towers)
		tower->update();
}

void OperationCenter::_update_towerBullet() {
	std::vector<Bullet*> &towerBullets = DataCenter::get_instance()->towerBullets;
	for(Bullet *towerBullet : towerBullets)
		towerBullet->update();
	// Detect if a bullet flies too far (exceeds its fly distance limit), which means the bullet lifecycle has ended.
	for(size_t i = 0; i < towerBullets.size(); ++i) {
		if(towerBullets[i]->get_fly_dist() <= 0) {
			delete towerBullets[i];
			towerBullets.erase(towerBullets.begin() + i);
			--i;
		}
	}
}

void OperationCenter::_update_monster_towerBullet() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	std::vector<Bullet*> &towerBullets = DC->towerBullets;
	for(size_t i = 0; i < monsters.size(); ++i) {
		for(size_t j = 0; j < towerBullets.size(); ++j) {
			// Check if the bullet overlaps with the monster.
			if(monsters[i]->shape->overlap(*(towerBullets[j]->shape))) {
				// Reduce the HP of the monster. Delete the bullet.
				monsters[i]->HP -= towerBullets[j]->get_dmg();
				delete towerBullets[j];
				towerBullets.erase(towerBullets.begin() + j);
				--j;
			}
		}
	}
}

constexpr char sword_hit_sound_path[] = "./assets/sound/Hit.ogg";
void OperationCenter::_update_monster_weapon(){
	DataCenter *DC = DataCenter::get_instance();
	std::vector<std::unique_ptr<Weapon>> &weapons = DC->hero->weapons;
	std::vector<Monster*> &monsters = DC->monsters;
	for(size_t i = 0; i < monsters.size(); ++i){
		for(size_t j=0; j < weapons.size(); ++j){
			if(!(weapons[j] ->can_hit())) continue;

			if(monsters[i]->shape->overlap(*(weapons[j]->shape))) {
				//debug_log("cwd = %s\n", std::filesystem::current_path().string().c_str());
				SoundCenter *SC = SoundCenter::get_instance();
				SC->play(sword_hit_sound_path, ALLEGRO_PLAYMODE_ONCE);
				monsters[i]->HP -= weapons[j] ->get_dmg();
				weapons[j] -> reset_cooldown();
			}
		}
	}
}

void OperationCenter::_update_mob_weapon(){
	DataCenter *DC = DataCenter::get_instance();
	std::vector<std::unique_ptr<Weapon>> &weapons = DC->hero->weapons;
	std::vector<std::unique_ptr<Mob>> &mobs = DC->mobs;

	for(auto &mob:mobs){
		for(auto &weapon:weapons){
			if(!weapon->can_hit()) continue;
			if(mob->shape->overlap(*(weapon->shape))){
				mob->hurt(weapon->get_dmg());
				weapon->reset_cooldown();
			}
		}
	}

}

void OperationCenter:: _update_mob_spawn(){
	static int one = 1;
	if(one==0) return ;//debug

	const int init_timer = 0;
	static int timer = init_timer;
	if(timer > 0) timer--;
	else {
		int rn = Random::range(0, static_cast<int>(MobType::MOBTYPE_MAX)-1);
		MobType type = static_cast<MobType>(rn); 
		DataCenter *DC = DataCenter::get_instance();
		DC->mobs.emplace_back(Mob::create_mob(type));
		
		timer = init_timer;
	}
	one--;
}

void OperationCenter::_update_mob(){
	std::vector<std::unique_ptr<Mob>> &mobs = DataCenter::get_instance()->mobs;
	for(auto m=mobs.begin();m!=mobs.end();){
		if((*m)->die){
			mobs.erase(m);
		}
		else{
			(*m)->update();
			m++;
		}
	}
}





void OperationCenter::_update_monster_player() {
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster*> &monsters = DC->monsters;
	Player *&player = DC->player;
	for(size_t i = 0; i < monsters.size(); ++i) {
		// Check if the monster is killed.
		if(monsters[i]->HP <= 0) {
			// Monster gets killed. Player receives money.
			player->coin += monsters[i]->get_money();
			delete monsters[i];
			monsters.erase(monsters.begin() + i);
			--i;
			// Since the current monsster is killed, we can directly proceed to next monster.
			break;
		}
		// Check if the monster reaches the end.
		if(monsters[i]->get_path().empty()) {
			delete monsters[i];
			monsters.erase(monsters.begin() + i);
			player->HP--;
			--i;
		}
	}
}

void OperationCenter::_update_monster_hero(){
	DataCenter *DC = DataCenter::get_instance();
	std::vector<Monster *> &monsters = DC->monsters;
	for(size_t i=0; i<monsters.size(); i++){
		if(monsters[i]->shape->overlap(*(DC->hero->shape))){
			monsters[i]->HP = 0;
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

		float x = Random::range(0.0, (float)DC->game_field_length);
		float y = Random::range(0.0, (float)DC->game_field_width);

		DC->buff_items.emplace_back(std::make_unique<Buffitem>(type, Point{x, y}));
		debug_log("spawn buff at x=%.1f y=%.1f type=%d\n", x, y, (int)type);
	}
	
}





// draw
void OperationCenter::draw() {
	_draw_monster();
	_draw_tower();
	_draw_towerBullet();
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





