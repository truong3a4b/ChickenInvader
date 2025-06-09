#include "app.hpp"
#include <cmsis_os2.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "stack_macros.h"

Game gameInstance;
Bullet shipBullet[MAX_BULLET];
Bullet enemyBullet[MAX_BULLET_ENEMY];
Enemy enemy[MAX_ENEMY];
Boss boss;
Bullet bossBullet[MAX_BULLET_BOSS];
#define MAX_SPAWN_RATE 10000
int spawnRate = MAX_SPAWN_RATE;
int spawnSeed = 0;
uint16_t sx, sy;
bool shouldEndGame;
bool shouldStopTask;
bool bossApear;

void updateEnemy(uint8_t dt);
void updateShipBullet(uint8_t dt);
void updateEnemyBullet(uint8_t dt);
void updateBoss(uint8_t dt);
void updateBossBullet(uint8_t dt);

uint8_t dt = 0;
void gameTask(void *argument) {
	int loopCount = 0;
	bossApear = false;

	for(int i=0; i<MAX_BULLET;i++) {
		shipBullet[i].updateCoordinate(-50, -50);
		shipBullet[i].updateVelocity(0, -5);
		shipBullet[i].updateDisplayStatus(IS_HIDDEN);
	}
	for(int i=0; i<MAX_BULLET_ENEMY;i++) {
		enemyBullet[i].updateCoordinate(-50, -50);
		enemyBullet[i].updateVelocity(0, 5);
		enemyBullet[i].updateDisplayStatus(IS_HIDDEN);
		enemyBullet[i].setRate(1000);
		enemyBullet[i].setSize(10, 6);

	}
	for(int i = 0; i < MAX_BULLET_BOSS;i++){
		bossBullet[i].updateCoordinate(-50, -50);
		bossBullet[i].updateVelocity(0, 5);
		bossBullet[i].updateDisplayStatus(IS_HIDDEN);
		bossBullet[i].setRate(1000);
		bossBullet[i].setSize(20, 20);
	}
	for(int i=0; i<MAX_ENEMY;i++) {
		enemy[i].updateCoordinate(-50, -50);
		enemy[i].updateDisplayStatus(IS_HIDDEN);
	}

	boss.updateCoordinate(-50, -50);
	boss.updateDisplayStatus(IS_HIDDEN);
	boss.hp_boss = 20;
//	uint32_t previousTick = 0, currentTick = 0;
	for(;;) {

		//check next stage
		if(gameInstance.score > 100){
			int tmp = 0;
			for(int i=0;i<MAX_ENEMY;i++) {
				if(enemy[i].displayStatus != IS_HIDDEN) tmp++;
			}
			for(int i = 0; i < MAX_BULLET_ENEMY; i++){
				if(enemyBullet[i].displayStatus != IS_HIDDEN) tmp++;
			}
			if(tmp == 0){
				bossApear = true;
				break;
			}
		}


		loopCount++;

		if(shouldEndGame) continue;

		if(loopCount >= 2) {
			dt = 1;
			loopCount = 0;
		} else {
			dt = 0;
		}
//		currentTick = xTaskGetTickCount();
//		dt = currentTick - previousTick;
//		dt = dt > 0 ? dt : 1;
//		previousTick = currentTick;
		/*  UPDATE POSITIONS */
		sx = gameInstance.ship.coordinateX;
		sy = gameInstance.ship.coordinateY;

		// update enemy
		updateEnemy(dt);

		// update ship position
		gameInstance.ship.update(dt);

		// update ship bullet position
		updateShipBullet(dt);

		// update enemy bullet position
		updateEnemyBullet(dt);


		/*  CHECK COLLISIONS */

		// check enemy and ship collision
		for(int i=0;i<MAX_ENEMY;i++) {
			if(enemy[i].displayStatus != IS_SHOWN) continue;
			if(Entity::isCollide(enemy[i], gameInstance.ship)) {
				gameInstance.ship.updateShipHp(-1);
				enemy[i].updateDisplayStatus(SHOULD_HIDE);
				gameInstance.ship.updateCoordinate(104, 260);
				break;
			}
		}

		// check enemy and ship bullet collision
		for(int i=0;i<MAX_ENEMY;i++) {
			if(enemy[i].displayStatus != IS_SHOWN) continue;
			for(int j=0;j<MAX_BULLET;j++) {
				if(shipBullet[j].displayStatus != IS_SHOWN) continue;
				if(Entity::isCollide(enemy[i], shipBullet[j])) {
					enemy[i].updateDisplayStatus(SHOULD_HIDE);
					shipBullet[j].updateDisplayStatus(SHOULD_HIDE);
					gameInstance.updateScore(5);
					break;
				}
			}
		}

		// check ship and enemy bullet collision
		for(int i=0;i<MAX_BULLET_ENEMY;i++) {
			if(enemyBullet[i].displayStatus != IS_SHOWN) continue;
			if(Entity::isCollide(enemyBullet[i], gameInstance.ship)) {
				gameInstance.ship.updateShipHp(-1);
				enemyBullet[i].updateDisplayStatus(SHOULD_HIDE);
				gameInstance.ship.updateCoordinate(104, 260);
				break;
			}
		}

		if(gameInstance.ship.lives <= 0) {
			shouldEndGame = true;
			  shouldStopTask = true;
		}

	}

	//Boss
	if(bossApear){
		for(;;){
			loopCount++;

			if(shouldEndGame) continue;

			if(loopCount >= 10) {
				dt = 1;
				loopCount = 0;
			} else {
				dt = 0;
			}

			sx = gameInstance.ship.coordinateX;
			sy = gameInstance.ship.coordinateY;

			updateBoss(dt);

			gameInstance.ship.update(dt);

			// update ship bullet position
			updateShipBullet(dt);

			// update enemy bullet position
			updateBossBullet(dt);


			/*  CHECK COLLISIONS */

			// check boss and ship collision
			if(Entity::isCollide(boss, gameInstance.ship)) {
				gameInstance.ship.updateShipHp(-1);
				gameInstance.ship.updateCoordinate(104, 260);
			}


			// check boss and ship bullet collision
			for(int j=0;j<MAX_BULLET;j++) {
				if(shipBullet[j].displayStatus != IS_SHOWN) continue;
				if(Entity::isCollide(boss, shipBullet[j])) {
					boss.updateHp(-1);
					shipBullet[j].updateDisplayStatus(SHOULD_HIDE);
					if(boss.hp_boss <= 0){
						boss.updateDisplayStatus(SHOULD_HIDE);
						shouldEndGame = true;
						shouldStopTask = true;

						break;
					}
				}
			}


			// check ship and enemy bullet collision
			for(int i=0;i<MAX_BULLET_BOSS;i++) {
				if(bossBullet[i].displayStatus != IS_SHOWN) continue;
				if(Entity::isCollide(bossBullet[i], gameInstance.ship)) {
					gameInstance.ship.updateShipHp(-1);
					bossBullet[i].updateDisplayStatus(SHOULD_HIDE);
					gameInstance.ship.updateCoordinate(104, 260);
					break;
				}
			}

			if(gameInstance.ship.lives <= 0) {
				shouldEndGame = true;

			}
		}
	}
}

void updateEnemy(uint8_t dt) {
	spawnRate -=dt;
	for(int i=0;i<MAX_ENEMY;i++) {
		if(enemy[i].displayStatus != IS_HIDDEN) {
			enemy[i].update(dt);
			continue;
		}

		if(gameInstance.score > 100) continue;
		if(spawnRate > 0) continue;

		uint16_t ex = 0;
		uint16_t ey = 0;
		uint16_t vx;
		uint16_t vy = 0;

		if(spawnSeed%2) {
			ex = -32;
			vx = 1;
		} else {
			ex = 240;
			vx = -1;
		}

		switch(spawnSeed) {
		case 0:
			ey = 42;
			break;
		case 1:
			ey = 80;
			break;
		case 2:
			ey = 120;
			break;
		case 3:
			ey = 160;
			break;
		default:
			break;
		}

		if(spawnSeed >= 3) {
			spawnSeed = 0;
		} else {
			spawnSeed++;
		}

		enemy[i].updateCoordinate(ex, ey);
		enemy[i].updateVelocity(vx, vy);
		enemy[i].updateDisplayStatus(SHOULD_SHOW);

		spawnRate = MAX_SPAWN_RATE;
	}
}

void updateShipBullet(uint8_t dt) {
	bool shouldFire = gameInstance.ship.updateBullet(dt);
	for(int i=0; i<MAX_BULLET;i++) {
			if(shipBullet[i].displayStatus != IS_HIDDEN) {
				shipBullet[i].update(dt);
				continue;
			}

			if(shouldFire) {
				shipBullet[i].updateCoordinate(sx, sy - 16);
				shipBullet[i].updateDisplayStatus(SHOULD_SHOW);
				shouldFire = false;
			}
		}
}

void updateEnemyBullet(uint8_t dt) {
	for(int j=0; j<MAX_ENEMY;j++) {
		if(enemy[j].displayStatus != IS_SHOWN) continue;
		if(enemy[j].updateBullet(dt) == false) continue;
		for(int i=0; i<MAX_BULLET_ENEMY;i++) {
			if(enemyBullet[i].displayStatus != IS_HIDDEN) continue;
			enemyBullet[i].updateCoordinate(enemy[j].coordinateX, enemy[j].coordinateY + 16);
			enemyBullet[i].updateDisplayStatus(SHOULD_SHOW);
			break;
		}
	}

	for(int i=0; i<MAX_BULLET_ENEMY;i++) {
		if(enemyBullet[i].displayStatus != IS_SHOWN) continue;
		enemyBullet[i].update(dt);
	}

}
void updateBoss(uint8_t dt){
	if(boss.displayStatus != IS_HIDDEN) {
		boss.update(dt);
		return;
	}
	if(bossApear){
		boss.updateCoordinate(-32, 40);
		boss.updateVelocity(2, 0);
		boss.updateDisplayStatus(SHOULD_SHOW);
	}
}
void updateBossBullet(uint8_t dt) {

	if(boss.displayStatus != IS_SHOWN) return;
	if(boss.updateBullet(dt) == true){
		for(int i=0; i<MAX_BULLET_BOSS;i++) {
			if(bossBullet[i].displayStatus != IS_HIDDEN) continue;
			bossBullet[i].updateCoordinate(boss.coordinateX+50, boss.coordinateY + 40);
			bossBullet[i].updateDisplayStatus(SHOULD_SHOW);
			break;
		}
	}


	for(int i=0; i<MAX_BULLET_BOSS;i++) {
		if(bossBullet[i].displayStatus != IS_SHOWN) continue;
		bossBullet[i].update(dt);
	}

}
