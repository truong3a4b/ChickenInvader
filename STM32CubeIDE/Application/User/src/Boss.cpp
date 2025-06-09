#include "Boss.hpp"
Boss::Boss() {
	this->width = 100;
	this->height = 80;
	this->displayStatus = IS_HIDDEN;
	this->velocityX = 0;
	this->velocityY = 0;
	this->hp_boss = 20;
	this->phase = 1;
  this->moveRate = MOVE_RATE;
  this->fireRate = FIRE_RATE;
}

Boss::~Boss() {

}

bool Boss::update(uint8_t dt) {
	if(this->coordinateX < -100 || this->coordinateX > 290 ||
			this->coordinateY < -100 || this->coordinateY > 370) {
		this->coordinateX = -32;
		this-> coordinateY = 80;
	}
	this->moveRate -= dt;
	if(this->moveRate > 0) return false;
	this->coordinateX += this->velocityX;
	this->coordinateY += this->velocityY;
	this->moveRate = MOVE_RATE;
	return true;
}

bool Boss::updateBullet(uint8_t dt) {
	this->fireRate -= dt;
	if(this->fireRate > 0) return false;
	this->fireRate = FIRE_RATE;
	return true;
}
void Boss::updateHp(short change) {
 this->hp_boss += change;
}

