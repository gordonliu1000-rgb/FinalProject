#include "DataCenter.h"
#include <cstring>
#include "../Level.h"
#include "../Hero.h"
#include "../Camera.h"

// fixed settings
namespace DataSetting {
	constexpr double FPS = 60;
	constexpr int window_width = 1200;
	constexpr int window_height = 800;
	constexpr int game_field_height = 2048;
	constexpr int game_field_width = 2048;
	constexpr int wall_width = 32;
	constexpr int cell_width = 64;
}

DataCenter::DataCenter() {
	this->FPS = DataSetting::FPS;
	this->cell_width = DataSetting::cell_width;
	this->window_width = DataSetting::window_width;
	this->wall_width = DataSetting::wall_width;
	this->window_height = DataSetting::window_height;
	this->game_field_height = DataSetting::game_field_height;
	this->game_field_width = DataSetting::game_field_width;
	
	memset(key_state, false, sizeof(key_state));
	memset(prev_key_state, false, sizeof(prev_key_state));
	mouse = Point(0, 0);
	memset(mouse_state, false, sizeof(mouse_state));
	memset(prev_mouse_state, false, sizeof(prev_mouse_state));
	grids.assign(this->game_field_height/DataSetting::cell_width,
		std::vector<Cell>(this->game_field_width/DataSetting::cell_width, Cell()));
	level = new Level();
	hero = new Hero();
	camera = Camera::get_instance();
	mobs.reserve(200);
}

void DataCenter::reset_object() { 
	buff_items.clear();
	next_mob_idx = 0;
	mobs.clear();
	enemy_spells.clear();
	next_exp_idx = 0;
	exps.clear();
	for(auto &g : grids) {
		for(auto &c : g) {
			c.mobs.clear();
		}
	}
}

DataCenter::~DataCenter() {
	delete level;
}
