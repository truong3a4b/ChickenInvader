
#ifndef APPLICATION_USER_SRC_BULLET_HPP_
#define APPLICATION_USER_SRC_BULLET_HPP_
#include "Entity.hpp"
#include <stdint.h>
const uint8_t MAX_BULLET = 10;
const uint8_t MAX_BULLET_ENEMY = 2;
const uint8_t MAX_BULLET_BOSS = 5;

class Bullet: public Entity {
public:
	Bullet();
	const int MOVE_RATE = 400;
	short cur_rate;
	virtual ~Bullet();
	bool update(uint8_t dt);
	bool setSize(short w, short h);
	bool setRate(short s);
};

#endif /* APPLICATION_USER_SRC_BULLET_HPP_ */
