#include "UI.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include "data/FontCenter.h"
#include <algorithm>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "shapes/Point.h"
#include "shapes/Rectangle.h"
#include "Player.h"
#include "towers/Tower.h"
#include "Level.h"
#include "buffs/Buff.h"
#include "Hero.h"

// fixed settings
constexpr char love_img_path[] = "./assets/image/love.png";
constexpr int love_img_padding = 5;
constexpr int tower_img_left_padding = 20;
constexpr int tower_img_top_padding = 30;


constexpr int buff_icon_top_padding = 80;
constexpr int buff_icon_left_padding = 20;

void
UI::init() {
	DataCenter *DC = DataCenter::get_instance();
	ImageCenter *IC = ImageCenter::get_instance();
	love = IC->get(love_img_path);
	int tl_x = DC->game_field_height + tower_img_left_padding;
	int tl_y = tower_img_top_padding;
	int max_height = 0;
	// arrange tower shop
	for(size_t i = 0; i < (size_t)(TowerType::TOWERTYPE_MAX); ++i) {
		ALLEGRO_BITMAP *bitmap = IC->get(TowerSetting::tower_menu_img_path[i]);
		int w = al_get_bitmap_width(bitmap);
		int h = al_get_bitmap_height(bitmap);
		if(tl_x + w > DC->window_width) {
			tl_x = DC->game_field_height + tower_img_left_padding;
			tl_y += max_height + tower_img_top_padding;
			max_height = 0;
		}
		tower_items.emplace_back(bitmap, Point{tl_x, tl_y}, TowerSetting::tower_price[i]);
		tl_x += w + tower_img_left_padding;
		max_height = std::max(max_height, h);
	}
	debug_log("<UI> state: change to HALT\n");
	state = STATE::HALT;
	on_item = -1;
}

void
UI::update() {}

void
UI::draw() {
	DataCenter *DC = DataCenter::get_instance();
	FontCenter *FC = FontCenter::get_instance();
	Hero *hero = DC->hero;
	//const Point &mouse = DC->mouse;
	
	// draw buff icons
	int num_of_drew_icon = 0;
	for(auto &buff : DC->hero->buffs){
		ALLEGRO_BITMAP *icon = buff->get_icon();
		if(icon==nullptr) continue;
		int icon_width = al_get_bitmap_width(icon);
		al_draw_bitmap(icon, 
			10 + (icon_width + buff_icon_left_padding) * (num_of_drew_icon%5),
			110 + 110* (num_of_drew_icon/5), 
			0);
		num_of_drew_icon++;
	}

	//Hp 
	float Hp_ratio = hero->hp / hero->max_hp;
	if(Hp_ratio < 0) Hp_ratio = 0;
	if(Hp_ratio > 1) Hp_ratio = 1;

	float HpBar_x = 10.0;
	float HpBar_y = 40.0;
	float HpBar_w = 250.0;
	float HpBar_h = 20.0;

	al_draw_filled_rectangle(HpBar_x, HpBar_y, HpBar_x + HpBar_w, 
			HpBar_y + HpBar_h, al_map_rgb(60, 60, 60));
	al_draw_filled_rectangle(HpBar_x, HpBar_y, HpBar_x + HpBar_w * Hp_ratio,
			HpBar_y + HpBar_h, al_map_rgb(200, 50, 50));
	al_draw_rectangle(HpBar_x, HpBar_y, HpBar_x + HpBar_w,
			HpBar_y + HpBar_h, al_map_rgb(255, 255, 255), 2);

	char buf[64];
	sprintf(buf, "HP:%.0f / %.0f", hero->hp, hero->max_hp);
	al_draw_text(FC->caviar_dreams[FontSize::SMALL], al_map_rgb(255, 255, 255), 
				HpBar_x, HpBar_y -22.0, 0, buf);
	//Exp
	float Exp_ratio = hero->exp / hero->exp_to_next;
	if(Exp_ratio < 0) Exp_ratio = 0;
	if(Exp_ratio > 1) Exp_ratio = 1;

	float ExpBar_x = 10.0;
	float ExpBar_y = HpBar_y + HpBar_h + 30.0;
	float ExpBar_w = 250.0;
	float ExpBar_h = 14.0;

	al_draw_filled_rectangle(ExpBar_x, ExpBar_y, 
			ExpBar_x + ExpBar_w, ExpBar_y + ExpBar_h, al_map_rgb(60, 60, 60));
	al_draw_filled_rectangle(ExpBar_x, ExpBar_y, 
			ExpBar_x + ExpBar_w * Exp_ratio, ExpBar_y + ExpBar_h, al_map_rgb(0, 191, 255));
	al_draw_rectangle(ExpBar_x, ExpBar_y, 
			ExpBar_x + ExpBar_w, ExpBar_y + ExpBar_h, al_map_rgb(255, 255, 255), 2);
		
	sprintf(buf, "LV:%.0f   EXP:%.0f / %.0f", hero->level, hero->exp, hero->exp_to_next);
	al_draw_text(FC->caviar_dreams[FontSize::SMALL], al_map_rgb(255, 255, 255),
			ExpBar_x, ExpBar_y - 20.0, 0, buf);
}
