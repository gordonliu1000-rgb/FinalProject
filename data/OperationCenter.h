#ifndef OPERATIONCENTER_H_INCLUDED
#define OPERATIONCENTER_H_INCLUDED

/**
 * @brief Class that defines functions for all object operations.
 * @details Object self-update, draw, and object-to-object interact functions are defined here.
 */
class OperationCenter
{
public:
	static OperationCenter *get_instance() {
		static OperationCenter OC;
		return &OC;
	}
	/**
	 * @brief Highest level update function.
	 * @details Calls all other update functions.
	 */
	void update();
	/**
	 * @brief Highest level draw function.
	 * @details Calls all other draw functions.
	 */
	void draw();
private:
	OperationCenter() {}
private:
	void _update_buffitem_pickup();
	void _update_mob_spawn();
	void _update_mob();
	void _update_mob_weapon();
	void _update_bump_dmg();
	void _update_enemy_spell();
	void _update_exp_pickup();
	void _update_exp_to_grid();
public:
	void drop_exp(const double &x, const double &y);
private:
	void _draw_exp();
	void _draw_buffitem();
	void _draw_mob();
	void _draw_enemy_spell();
};

#endif
