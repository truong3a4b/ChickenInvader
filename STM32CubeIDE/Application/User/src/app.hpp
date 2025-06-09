#ifndef APPLICATION_USER_SRC_APP_HPP_
#define APPLICATION_USER_SRC_APP_HPP_
#include "Game.hpp"
#include "Entity.hpp"
#include "Boss.hpp"

extern Game gameInstance;
extern bool shouldEndGame;
extern bool shouldStopTask;
extern bool bossApear;
extern Enemy enemy[MAX_ENEMY];
extern Bullet shipBullet[MAX_BULLET];
extern Bullet enemyBullet[MAX_BULLET_ENEMY];
extern Bullet bossBullet[MAX_BULLET_BOSS];
extern Boss boss;
extern void gameTask(void *argument);

#endif /* APPLICATION_USER_SRC_APP_HPP_ */
