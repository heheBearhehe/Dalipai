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
#include "../model/Game.h"

class Game;
class Player;
class Recorder;
class Card;

class GameLayer;
class PauseLayer;
class UserChoiceLayer;
class PlayerActionCallBack;


class PlayScene : public cocos2d::Scene, public PlayerChoiceListener, public GameStateListener
{
public:
    virtual bool init();
    
    CREATE_FUNC(PlayScene);
    
    virtual void onActionExecuted(int action, Player* player, Card* card1, Card* card2);
    void onAction();
private:
    
    virtual int makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback);
    virtual bool onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard);
    virtual void onFinished();
    
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    cocos2d::ui::Button* addButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    
    void startGame();
    void menuRestart(Ref* pSender);
    std::string getChoiceMessage(int action, Player* player);
    std::string getActionExecutedMessage(int action, Player* player);
    
private:
    Game*   mGame;
    Player* mPlayer1;
    Player* mPlayer2;
    
    GameLayer*       mGameLayer;
    PauseLayer*      mPauseLayer;
    UserChoiceLayer* mUserChoiceLayer;
    
    
};


#endif /* defined(__DaLiPoker__PlayScene__) */
