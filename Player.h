#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include <vector>
#include "buffs/Buff.h"

class Player
{
public:
	Player();
	void update();
	int HP;
	int coin;
	std::vector<Buff*> buffs;

private:
	int coin_freq;
	int coin_increase;
	int coin_counter;
};

#endif
