#include <gui/gamescreen_screen/GameScreenView.hpp>
#include <D:\MainWork\Space_in\Space_in\STM32CubeIDE\Application\User\src\app.hpp>
#include <BitmapDatabase.hpp>
#include <cmsis_os2.h>
#include <cmsis_os.h>
#include <cstring>
#include "stm32f4xx_hal.h"


extern void gameTask(void *argument);
osThreadId_t gameTaskHandle;
uint8_t hearts = 3;
uint8_t delayCounter = 0;
uint16_t bgY1,bgY2,bgY3;
bool shouldStopScreen;
extern osMessageQueueId_t Queue1Handle;
extern osMessageQueueId_t Queue2Handle;
extern osMessageQueueId_t Queue3Handle;
extern osMessageQueueId_t Queue4Handle;

GameScreenView::GameScreenView()
{
	bgY1 = 0;
	bgY2 = -260;
	bgY3 = -410;
	gameInstance = Game();
	remove(menu_button);
	remove(score_holder);
	// Prepare ship
	shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIP_ID));
	shipImage.setXY(gameInstance.ship.coordinateX, gameInstance.ship.coordinateY);
	add(shipImage);

	for(int i=0;i<MAX_BULLET;i++) {

		shipBulletImage[i].setXY(321, 33);
		shipBulletImage[i].setBitmap(touchgfx::Bitmap(BITMAP_BULLET_DOUBLE_ID));
	}
	for(int i=0;i<MAX_BULLET_ENEMY;i++) {
		enemyBulletImage[i].setXY(321, 33);
		enemyBulletImage[i].setBitmap(touchgfx::Bitmap(BITMAP_EGG_ID));

	}
	for(int i =0; i < MAX_BULLET_BOSS;i++){
		bossBulletImage[i].setXY(321, 33);
		bossBulletImage[i].setBitmap(touchgfx::Bitmap(BITMAP_ENEMY_BULLET_RED_ID));
	}

	for(int i=0;i<MAX_ENEMY;i++) {
		switch(i%2) {
		case 0:
			enemyImage[i].setBitmaps(BITMAP_CHICKENGREEN1_ID, BITMAP_CHICKENGREEN2_ID);
			break;
		case 1:
			enemyImage[i].setBitmaps(BITMAP_CHICKENRED1_ID, BITMAP_CHICKENRED2_ID);
			break;
		default:
			break;
		}
		enemyImage[i].setUpdateTicksInterval(20);
	}

	bossImage.setBitmap(BITMAP_BOSS_ID);

}

void GameScreenView::setupScreen()
{
	GameScreenViewBase::setupScreen();
  osThreadTerminate(gameTaskHandle);
	const osThreadAttr_t gameTask_attributes = {
	  .name = "gameTask",
	  .stack_size = 8192 * 2,
	  .priority = (osPriority_t) osPriorityNormal,
	};
	gameTaskHandle = osThreadNew(gameTask, NULL, &gameTask_attributes);
	shouldEndGame = false;
	shouldStopTask = false;
	shouldStopScreen = false;
}

void GameScreenView::tearDownScreen()
{
    GameScreenViewBase::tearDownScreen();
}



// Render game objects
void GameScreenView::handleTickEvent() {
	GameScreenViewBase::handleTickEvent();
	//check end
	if(shouldEndGame && shouldStopTask && !shouldStopScreen) {
			add(menu_button);
			Unicode::snprintf(score_holderBuffer, SCORE_HOLDER_SIZE, "%d", gameInstance.score);
			add(score_holder);
			menu_button.invalidate();
			score_holder.invalidate();
			shouldStopScreen = true;
			osThreadTerminate(gameTaskHandle);
			return;
	}
	// check if player is out of health and should end game
	if(gameInstance.ship.lives != hearts) {
		shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIPDIE_ID));
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
		hearts = gameInstance.ship.lives;
		delayCounter = 30;
		// reset hearts
		heart_03.setAlpha(80);
		heart_02.setAlpha(80);
		heart_01.setAlpha(80);
		if(hearts >= 1) heart_03.setAlpha(255);
		if(hearts >= 2) heart_02.setAlpha(255);
		if(hearts >= 3) heart_01.setAlpha(255);
		// If player is out of health
		if(hearts < 1) {
			shouldStopTask = true;
			shouldEndGame = true;
			invalidate();
		}
	}

	if(delayCounter > 0){
		delayCounter--;
		return;
	}
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);

	//Set background
	bgY1++;
	bgY2++;
	bgY3++;
	if(bgY1==0){
		bgY2 =-260;
	}
	if(bgY2==0){
		bgY3 =-150;
	}
	if(bgY3 == 0){
		bgY1 = -320;
	}
	bg1.setXY(0, bgY1);
	bg1.invalidate();
	bg2.setXY(0, bgY2);
	bg2.invalidate();
	bg3.setXY(0, bgY3);
	bg3.invalidate();


	// Get input
	uint8_t res = 0;
	uint32_t count = osMessageQueueGetCount(Queue1Handle);
	if(count > 0) {
			osMessageQueueGet(Queue1Handle,&res, NULL, osWaitForever);
			if(res == 'R') {

				gameInstance.ship.updateVelocityX(gameInstance.ship.VELOCITY);
				shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIPR_ID));
				osMessageQueueReset(Queue2Handle);
			} else if(res == 'N'){
				gameInstance.ship.updateVelocityX(0);
				shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIP_ID));
			}
	}
	uint32_t count2 = osMessageQueueGetCount(Queue2Handle);
	if(count2 > 0) {
			osMessageQueueGet(Queue2Handle,&res, NULL, osWaitForever);
			if(res == 'L') {
				gameInstance.ship.updateVelocityX(-gameInstance.ship.VELOCITY);
				shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIPL_ID));
				osMessageQueueReset(Queue1Handle);
			} else if(res == 'N'){
				gameInstance.ship.updateVelocityX(0);
				shipImage.setBitmap(touchgfx::Bitmap(BITMAP_SHIP_ID));
			}
	}

	uint32_t count3 = osMessageQueueGetCount(Queue3Handle);
	if(count3 > 0) {
			osMessageQueueGet(Queue3Handle,&res, NULL, osWaitForever);
			if(res == 'U') {
				gameInstance.ship.updateVelocityY(gameInstance.ship.VELOCITY);
				osMessageQueueReset(Queue4Handle);
			} else if(res == 'N'){
				gameInstance.ship.updateVelocityY(0);
			}
	}

	uint32_t count4 = osMessageQueueGetCount(Queue4Handle);
	if(count4 > 0) {
			osMessageQueueGet(Queue4Handle,&res, NULL, osWaitForever);
			if(res == 'D') {
				gameInstance.ship.updateVelocityY(-gameInstance.ship.VELOCITY);
				osMessageQueueReset(Queue3Handle);
			} else if(res == 'N'){
				gameInstance.ship.updateVelocityY(0);
			}
	}

	// update player position and sprite
	shipImage.moveTo(gameInstance.ship.coordinateX, gameInstance.ship.coordinateY);

	// render ship bullet
	for(int i=0; i<MAX_BULLET;i++) {
		switch(shipBullet[i].displayStatus){
		case IS_HIDDEN:
			break;
		case IS_SHOWN:
			shipBulletImage[i].moveTo(shipBullet[i].coordinateX, shipBullet[i].coordinateY);
			break;
		case SHOULD_SHOW:
			shipBulletImage[i].moveTo(shipBullet[i].coordinateX, shipBullet[i].coordinateY);
			add(shipBulletImage[i]);
			shipBullet[i].updateDisplayStatus(IS_SHOWN);
			break;
		case SHOULD_HIDE:
			remove(shipBulletImage[i]);
			shipBullet[i].updateDisplayStatus(IS_HIDDEN);
			break;
		default:
			break;
		}
	}

	// render enemy bullet
	for(int i=0; i<MAX_BULLET_ENEMY;i++) {
		switch(enemyBullet[i].displayStatus){
		case IS_HIDDEN:
			break;
		case IS_SHOWN:
			enemyBulletImage[i].moveTo(enemyBullet[i].coordinateX, enemyBullet[i].coordinateY);
			break;
		case SHOULD_SHOW:
			enemyBulletImage[i].moveTo(enemyBullet[i].coordinateX, enemyBullet[i].coordinateY);
			add(enemyBulletImage[i]);
			enemyBullet[i].updateDisplayStatus(IS_SHOWN);
			break;
		case SHOULD_HIDE:
			remove(enemyBulletImage[i]);
			enemyBullet[i].updateDisplayStatus(IS_HIDDEN);
			break;
		default:
			break;
		}
	}

	// render enemy
	for(int i=0; i<MAX_ENEMY;i++) {
		switch(enemy[i].displayStatus){
		case IS_SHOWN:
			enemyImage[i].moveTo(enemy[i].coordinateX, enemy[i].coordinateY);
			break;
		case SHOULD_SHOW:
			enemyImage[i].moveTo(enemy[i].coordinateX, enemy[i].coordinateY);
			enemyImage[i].startAnimation(false, true, true);
			add(enemyImage[i]);
			enemy[i].updateDisplayStatus(IS_SHOWN);
			break;
		case SHOULD_HIDE:
			enemyImage[i].stopAnimation();
			remove(enemyImage[i]);
			enemy[i].updateDisplayStatus(IS_HIDDEN);
			break;
		case IS_HIDDEN:
		default:
			break;
		}
	}

	if(bossApear){
		//render boss bullet
		for(int i=0; i<MAX_BULLET_BOSS;i++) {
			switch(bossBullet[i].displayStatus){
				case IS_HIDDEN:
					break;
				case IS_SHOWN:
					bossBulletImage[i].moveTo(bossBullet[i].coordinateX, bossBullet[i].coordinateY);
					break;
				case SHOULD_SHOW:
					bossBulletImage[i].moveTo(bossBullet[i].coordinateX, bossBullet[i].coordinateY);
					add(bossBulletImage[i]);
					bossBullet[i].updateDisplayStatus(IS_SHOWN);
					break;
				case SHOULD_HIDE:
					remove(bossBulletImage[i]);
					bossBullet[i].updateDisplayStatus(IS_HIDDEN);
					break;
				default:
					break;
			}
		}

		//render boss
		switch(boss.displayStatus){
			case IS_SHOWN:
				bossImage.moveTo(boss.coordinateX, boss.coordinateY);
				break;
			case SHOULD_SHOW:
				bossImage.moveTo(boss.coordinateX, boss.coordinateY);
				add(bossImage);
				boss.updateDisplayStatus(IS_SHOWN);
				break;
			case SHOULD_HIDE:
				remove(bossImage);
				boss.updateDisplayStatus(IS_HIDDEN);
				break;
			case IS_HIDDEN:
			default:
				break;
		}
	}


	Unicode::snprintf(score_boardBuffer, SCORE_BOARD_SIZE, "%d", gameInstance.score);
	invalidate();
}
