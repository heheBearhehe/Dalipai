//
//  PlayScene.h
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#ifndef __DaLiPoker__PlayScene__
#define __DaLiPoker__PlayScene__

#include <stdio.h>
#include "cocos2d.h"
#include <CocosGUI.h>

class Game;
class Player;
class Recorder;

class GameLayer;
class PauseLayer;
class UserChoiceLayer;

class PlayScene : public cocos2d::Scene
{
public:
    virtual bool init();
    
    CREATE_FUNC(PlayScene);
    
private:
    
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    cocos2d::ui::Button* addButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    
private:
    Game*   mGame;
    Player* mPlayer1;
    Player* mPlayer2;
    
    GameLayer*       mGameLayer;
    PauseLayer*      mPauseLayer;
    UserChoiceLayer* mUserChoiceLayer;
    
};


#endif /* defined(__DaLiPoker__PlayScene__) */
