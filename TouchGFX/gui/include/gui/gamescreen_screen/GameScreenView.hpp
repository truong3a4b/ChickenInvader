#ifndef GAMESCREENVIEW_HPP
#define GAMESCREENVIEW_HPP

#include <gui_generated/gamescreen_screen/GameScreenViewBase.hpp>
#include <gui/gamescreen_screen/GameScreenPresenter.hpp>
#include <D:\MainWork\Space_in\Space_in\STM32CubeIDE\Application\User\src\app.hpp>


class GameScreenView : public GameScreenViewBase
{
public:
    GameScreenView();
    virtual ~GameScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void handleTickEvent();
protected:
    void getInput();

private:
    touchgfx::Image shipImage;
    touchgfx::AnimatedImage enemyImage[MAX_ENEMY];
    touchgfx::Image shipBulletImage[MAX_BULLET];
    touchgfx::Image enemyBulletImage[MAX_BULLET];
    touchgfx::Image bossBulletImage[MAX_BULLET];
    touchgfx::Image bossImage;
};




#endif // GAMESCREENVIEW_HPP
