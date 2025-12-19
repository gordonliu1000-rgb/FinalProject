#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro5/allegro.h>
#include "Button/Button.h"
#include "UI.h"

/**
 * @brief Main class that runs the whole game.
 * @details All game procedures must be processed through this class.
 */

class Game
{
public:
	void execute();
public:
	Game(bool testMode = false);
	~Game();
	void game_init();
	bool game_update();
	void game_draw();
	void reset_game();
	bool show_help_menu;
	Button start_btn;
	Button help_btn;
	Button quit_btn;
	Button pulse_btn;
	Button restart_btn;
	Button end_btn;
private:
	/**
	 * @brief States of the game process in game_update.
	 * @see Game::game_update()
	 */
	enum class STATE {
		START, // -> LEVEL
		LEVEL, // -> PAUSE, GAMEOVER
		PAUSE, // -> LEVEL
		GAMEOVER, //->START, END
		END
	};
	STATE state;
	ALLEGRO_EVENT event;
	ALLEGRO_BITMAP *game_icon;
	ALLEGRO_BITMAP *background;
	ALLEGRO_BITMAP *help;
	ALLEGRO_BITMAP *pulse;
private:
	ALLEGRO_DISPLAY *display;
	ALLEGRO_TIMER *timer;
	ALLEGRO_EVENT_QUEUE *event_queue;
	UI *ui;
	bool bgm_played = false;
	// ALLEGRO_SAMPLE_INSTANCE *bgm_instance = nullptr;
	
};

#endif
