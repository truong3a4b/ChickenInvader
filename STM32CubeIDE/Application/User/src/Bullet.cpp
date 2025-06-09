#include "Bullet.hpp"
#include <stdint.h>
Bullet::Bullet() {
	this->width = 24;
	this->height = 24;
	this->displayStatus = IS_HIDDEN;
	this->velocityX = 0;
	this->velocityY = 0;
	this->moveRate = MOVE_RATE;
	this->cur_rate = MOVE_RATE;
}

Bullet::~Bullet() {

}

bool Bullet::update(uint8_t dt) {
	if(this->coordinateX < -this->width || this->coordinateX > 240 ||
			this->coordinateY < -this->height || this->coordinateY > 320) {
		this->displayStatus = SHOULD_HIDE;
		this->moveRate = MOVE_RATE;
		return false;
	}
	this->moveRate -= dt;
	if(this->moveRate > 0) return false;
	this->coordinateX += this->velocityX;
	this->coordinateY += this->velocityY;
	this->moveRate = cur_rate;
	return true;
}
bool Bullet::setSize(short w, short h){
	this->width = w;
	this->height = h;
	return true;
}
bool Bullet::setRate(short s){
	this->cur_rate = s;
}
