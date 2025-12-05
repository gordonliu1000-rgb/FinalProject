#include "Game.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/OperationCenter.h"
#include "data/SoundCenter.h"
#include "data/ImageCenter.h"
#include "data/FontCenter.h"
#include "Player.h"
#include "Level.h"
#include "Hero.h"
#include "Camera.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <cstring>
#include "WorldCoordinate.h"

// fixed settings
constexpr char game_icon_img_path[] = "./assets/image/game_icon.png";
constexpr char game_start_sound_path[] = "./assets/sound/growl.wav";
constexpr char background_img_path[] = "./assets/image/StartBackground.png";
constexpr char background_sound_path[] = "./assets/sound/BackgroundMusic.ogg";

/**
 * @brief Game entry.
 * @details The function processes all allegro events and update the event state to a generic data storage (i.e. DataCenter).
 * For timer event, the game_update and game_draw function will be called if and only if the current is timer.
 */
void
Game::execute() {
	DataCenter *DC = DataCenter::get_instance();
	// main game loop
	bool run = true;
	while(run) {
		// process all events here
		al_wait_for_event(event_queue, &event);
		switch(event.type) {
			case ALLEGRO_EVENT_TIMER: {
				run &= game_update();
				game_draw();
				break;
			} case ALLEGRO_EVENT_DISPLAY_CLOSE: { // stop game
				run = false;
				break;
			} case ALLEGRO_EVENT_KEY_DOWN: {
				DC->key_state[event.keyboard.keycode] = true;
				break;
			} case ALLEGRO_EVENT_KEY_UP: {
				DC->key_state[event.keyboard.keycode] = false;
				break;
			} case ALLEGRO_EVENT_MOUSE_AXES: {
				DC->mouse.x = event.mouse.x;
				DC->mouse.y = event.mouse.y;
				break;
			} case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
				DC->mouse_state[event.mouse.button] = true;
				break;
			} case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
				DC->mouse_state[event.mouse.button] = false;
				break;
			} default: break;
		}
	}
}

/**
 * @brief Initialize all allegro addons and the game body.
 * @details Only one timer is created since a game and all its data should be processed synchronously.
 */
Game::Game(bool testMode) {
	DataCenter *DC = DataCenter::get_instance();
	GAME_ASSERT(al_init(), "failed to initialize allegro.");

	// initialize allegro addons
	bool addon_init = true;
	addon_init &= al_init_primitives_addon();
	addon_init &= al_init_font_addon();
	addon_init &= al_init_ttf_addon();
	addon_init &= al_init_image_addon();
	addon_init &= al_init_acodec_addon();
	GAME_ASSERT(addon_init, "failed to initialize allegro addons.");

	if(testMode) {
		timer = nullptr;
		event_queue = nullptr;
		display = nullptr;
		debug_log("Game initialized in test mode.\n");
		return;
	}

	// initialize events
	bool event_init = true;
	event_init &= al_install_keyboard();
	event_init &= al_install_mouse();
	event_init &= al_install_audio();
	GAME_ASSERT(event_init, "failed to initialize allegro events.");

	// initialize game body
	GAME_ASSERT(
		timer = al_create_timer(1.0 / DC->FPS),
		"failed to create timer.");
	GAME_ASSERT(
		event_queue = al_create_event_queue(),
		"failed to create event queue.");
	GAME_ASSERT(
		display = al_create_display(DC->window_width, DC->window_height),
		"failed to create display.");

	debug_log("Game initialized.\n");
	game_init();
}

/**
 * @brief Initialize all auxiliary resources.
 */
void
Game::game_init() {
	DataCenter *DC = DataCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	ImageCenter *IC = ImageCenter::get_instance();
	FontCenter *FC = FontCenter::get_instance();
	// set window icon
	game_icon = IC->get(game_icon_img_path);
	al_set_display_icon(display, game_icon);

	// register events to event_queue
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

	// init sound setting
	SC->init();

	// init font setting
	FC->init();

	ui = new UI();
	ui->init();

	DC->level->init();
	DC->hero->init();
	DC->camera->init();

	show_help_menu = false;

	float cx = DC->window_width / 2.0f;
	float cy = DC->window_height / 2.0f;
	float bw = 200.0f;
	float bh = 50.0f;
	float gap = 20.0f;

	start_btn = Button{ cx - bw/2, cy - bh - gap, bw, bh, "Start"};
	help_btn = Button{ cx - bw/2, cy, bw, bh, "Help"};
	quit_btn = Button{ cx - bw/2, cy + bh + gap, bw, bh, "Quit"};

	// game start
	background = IC->get(background_img_path);
	debug_log("Game state: change to START\n");
	state = STATE::START;
	al_start_timer(timer);
}

/**
 * @brief The function processes all data update.
 * @details The behavior of the whole game body is determined by its state.
 * @return Whether the game should keep running (true) or reaches the termination criteria (false).
 * @see Game::STATE
 */
bool
Game::game_update() {
	DataCenter *DC = DataCenter::get_instance();
	OperationCenter *OC = OperationCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	static ALLEGRO_SAMPLE_INSTANCE *background = nullptr;

	switch(state) {
		case STATE::START: {
			static bool is_played = false;
			//static ALLEGRO_SAMPLE_INSTANCE *instance = nullptr;
			if(!is_played) {
				/*instance =*/ SC->play(game_start_sound_path, ALLEGRO_PLAYMODE_ONCE);
				DC->level->load_level(1);
				is_played = true;
			}

			bool mouse_down = DC->mouse_state[1];
			bool mouse_prev = DC->prev_key_state[1];
			const Point &mouse = DC->mouse;

			if(show_help_menu){
				if(mouse_down && !mouse_prev){
					show_help_menu = false;
				}
				break;
			}

			if(start_btn.update(mouse, mouse_down, mouse_prev)){
				debug_log("<Game> state: change to LEVEL\n");
				state = STATE::LEVEL;
			}
			else if(help_btn.update(mouse, mouse_down, mouse_prev)){
				show_help_menu = true;
			}
			else if(quit_btn.update(mouse, mouse_down, mouse_prev)){
				debug_log("<Game> state: change to END\n");
				state = STATE::END;
			}
			/*if(!SC->is_playing(instance)) {
				debug_log("<Game> state: change to LEVEL\n");
				state = STATE::LEVEL;
			}*/
			break;
		} case STATE::LEVEL: {
			static bool BGM_played = false;
			if(!BGM_played) {
				background = SC->play(background_sound_path, ALLEGRO_PLAYMODE_LOOP);
				BGM_played = true;
			}

			if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
				SC->toggle_playing(background);
				debug_log("<Game> state: change to PAUSE\n");
				state = STATE::PAUSE;
			}
			if(DC->level->remain_monsters() == 0 && DC->monsters.size() == 0) {
				debug_log("<Game> state: change to END\n");
				state = STATE::END;
			}
			if(DC->player->HP == 0) {
				debug_log("<Game> state: change to END\n");
				state = STATE::END;
			}
			break;
		} case STATE::PAUSE: {
			if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
				SC->toggle_playing(background);
				debug_log("<Game> state: change to LEVEL\n");
				state = STATE::LEVEL;
			}
			break;
		} case STATE::END: {
			return false;
		}
	}
	// If the game is not paused, we should progress update.
	if(state != STATE::PAUSE) {
		DC->player->update();
		SC->update();
		ui->update();
		DC->camera->update();
		if(state != STATE::START) {
			DC->level->update();
			DC->hero->update();
			OC->update();
		}
	}
	// game_update is finished. The states of current frame will be previous states of the next frame.
	memcpy(DC->prev_key_state, DC->key_state, sizeof(DC->key_state));
	memcpy(DC->prev_mouse_state, DC->mouse_state, sizeof(DC->mouse_state));
	return true;
}

/**
 * @brief Draw the whole game and objects.
 */
void
Game::game_draw() {
    DataCenter *DC = DataCenter::get_instance();
    OperationCenter *OC = OperationCenter::get_instance();
    FontCenter *FC = FontCenter::get_instance();

    // 1. 清畫面
    al_clear_to_color(al_map_rgb(100, 100, 100));

    if (state != STATE::END) {
		// 2. 設定「世界座標」攝影機 transform，鏡頭位置用 DC->camera_x/y
		WorldCoordinate::switch_to_world_coordinate();

		// 3. 在世界座標下畫「會跟著鏡頭動」的東西：背景、地圖、hero、OC
		// 背景（放大 2 倍）
		al_draw_scaled_bitmap(background, 
			0, 0,
			DC->game_field_length, DC->game_field_length, 
			0, 0,
			DC->game_field_length * 2.0, DC->game_field_length * 2.0, 
			0);

		if (state != STATE::START) {
			DC->level->draw();   // 這裡的座標都當「世界座標」
			DC->hero->draw();    // Hero::draw 用 shape->center_x()/y() 就好
			OC->draw();          // 若是遊戲物件（不是 UI），放這裡
		}

		// 4. 畫 UI：改回「螢幕座標」，不跟鏡頭動
		WorldCoordinate::switch_to_camera_coordinate();

		if (state != STATE::START) {
			ui->draw();          // UI 用視窗座標，例如 (10,10)
		}
    }

    // 5. 疊加狀態畫面（此時已是螢幕座標）
    switch (state) {
        case STATE::START: {
			//button color
			auto btn_color  = al_map_rgb(50, 50, 50);
			auto btn_hover = al_map_rgb(80, 80, 80);
			auto border = al_map_rgb(255, 255, 255);
			auto text_color = al_map_rgb(255, 255, 255);

			DataCenter *DC = DataCenter::get_instance();
			const Point &mouse = DC->mouse;
			ALLEGRO_FONT *font = FC->caviar_dreams[FontSize::MEDIUM];
			start_btn.draw(font, btn_color, btn_hover, border, text_color, mouse);
			help_btn.draw(font, btn_color, btn_hover, border, text_color, mouse);
			quit_btn.draw(font, btn_color, btn_hover, border, text_color, mouse);

			if(show_help_menu){
				al_draw_filled_rectangle(0, 0, DC->window_width, DC->window_height, al_map_rgba(0, 0, 0, 150));

				float w = DC->window_width * 0.6f;
				float h = DC->window_height * 0.6f;
				//半透明左上角標
				float x = (DC->window_width - w) / 2.0f; 
				float y = (DC->window_height - h) / 2.0f;
				al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgb(230, 230, 230));
				al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(255, 255, 255), 2);

				al_draw_text(FC->caviar_dreams[FontSize::LARGE], al_map_rgb(0, 0, 0), x + w/2.0f, y + 20.0f,
					ALLEGRO_ALIGN_CENTRE, "HELP");

			}
            break;
        }
        case STATE::LEVEL: {
            break;
        }
        case STATE::PAUSE: {
            // 半透明遮罩
            al_draw_filled_rectangle(
                0, 0, DC->window_width, DC->window_height,
                al_map_rgba(50, 50, 50, 64)
            );
            // 中央顯示 GAME PAUSED
            al_draw_text(
                FC->caviar_dreams[FontSize::LARGE],
                al_map_rgb(255, 255, 255),
                DC->window_width / 2.0, DC->window_height / 2.0,
                ALLEGRO_ALIGN_CENTRE,
                "GAME PAUSED"
            );
            break;
        }
        case STATE::END: {
            break;
        }
    }

    al_flip_display();
}

Game::~Game() {
	if(display) al_destroy_display(display);
	if(timer) al_destroy_timer(timer);
	if(event_queue) al_destroy_event_queue(event_queue);
}
