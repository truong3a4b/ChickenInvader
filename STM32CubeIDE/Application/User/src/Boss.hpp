#ifndef APPLICATION_USER_SRC_BOSS_HPP_
#define APPLICATION_USER_SRC_BOSS_HPP_
#include "Entity.hpp"

class Boss: public Entity {
public:
	int MOVE_RATE = 600;
	int FIRE_RATE = 25000;
	uint8_t hp_boss;
	uint8_t phase;
	Boss();
	virtual ~Boss();
	bool update(uint8_t dt);
	bool updateBullet(uint8_t dt);
	void updateHp(short change);
};

#endif /* APPLICATION_USER_SRC_BOSS_HPP_ */
