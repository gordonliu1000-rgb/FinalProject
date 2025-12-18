#include "UI.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/ImageCenter.h"
#include "data/FontCenter.h"
#include <algorithm>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include "shapes/Point.h"
#include "shapes/Rectangle.h"
#include "Player.h"
#include "Level.h"
#include "buffs/Buff.h"
#include "Hero.h"

// fixed settings
constexpr int love_img_padding = 5;
constexpr int tower_img_left_padding = 20;
constexpr int tower_img_top_padding = 30;


constexpr int buff_icon_top_padding = 80;
constexpr int buff_icon_left_padding = 20;

void
UI::init() {
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
		
	sprintf(buf, "LV:%d   EXP:%.0f / %.0f", hero->level, hero->exp, hero->exp_to_next);
	al_draw_text(FC->caviar_dreams[FontSize::SMALL], al_map_rgb(255, 255, 255),
			ExpBar_x, ExpBar_y - 20.0, 0, buf);

	//shield
	if(hero->shield != 0.0){
		float shield_ratio = hero->shield / hero->max_shield;
		if(shield_ratio < 0) shield_ratio = 0;
		if(shield_ratio > 1) shield_ratio = 1;

		float Shildbar_x = 280;
		float Shildbar_y = 40;
		float Shildbar_w = 100.0;
		float Shildbar_h = 20.0;
		
		al_draw_filled_rectangle(Shildbar_x, Shildbar_y, 
				Shildbar_x + Shildbar_w, Shildbar_y + Shildbar_h, al_map_rgb(60, 60, 60));
		al_draw_filled_rectangle(Shildbar_x, Shildbar_y, 
				Shildbar_x + Shildbar_w * shield_ratio, Shildbar_y + Shildbar_h, al_map_rgb(255, 255, 0));
		al_draw_rectangle(Shildbar_x, Shildbar_y, 
				Shildbar_x + Shildbar_w, Shildbar_y + Shildbar_h, al_map_rgb(255, 255, 255), 2);
	}

	//score
	sprintf(buf, "Kills:%d", DC->hero->score);
	al_draw_text(FC->caviar_dreams[FontSize::MEDIUM], al_map_rgb(255, 255, 255),
					DC->window_width / 2.0, 10.0, ALLEGRO_ALIGN_CENTRE, buf);

	//Hero damage
	if(hero->damage_hp > 0) {
		float t = hero->damage_hp / 18.0;
		float a = 0.25 * t;
		al_draw_filled_rectangle(0, 0, DC->window_width, DC->window_height, 
							al_premul_rgba_f(1.0, 0.0, 0.0, a));
		hero->damage_hp--;
	}

	//Hero Hp Low
	if(hero->hp <= hero->max_hp * 0.2) {
		float a = 0.35;
		float b = 20;
		ALLEGRO_COLOR c = al_premul_rgba_f(1, 0, 0, a);
		al_draw_filled_rectangle(0, 0, DC->window_width, b, c);
		al_draw_filled_rectangle(0, DC->window_height - b, DC->window_width, DC->window_height, c);
		al_draw_filled_rectangle(0, 0, b, DC->window_height, c);
		al_draw_filled_rectangle(DC->window_width - b, 0, DC->window_width, DC->window_height, c);
	}
	
}
