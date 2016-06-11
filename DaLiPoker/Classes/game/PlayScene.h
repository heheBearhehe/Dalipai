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
class AIPlayer;
class ReplayPlayer;

class GameLayer;
class PauseLayer;
class UserChoiceLayer;
class CalcScoreLayer;
class ReplayLayer;
class PlayerActionCallBack;
class CalcScoreActionCallBack;


class PlayScene : public cocos2d::Scene, public PlayerChoiceListener, public GameStateListener, public GameActionCallBack
{
public:
    virtual bool init();
    
    CREATE_FUNC(PlayScene);
    
    virtual void onActionExecuted(int action, Player* player, Card* card1, Card* card2);
    void onAction();
    void onMakeChoice();
    virtual void onGameAction(int action);
private:
    virtual void reset() {};
    virtual int makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback);
    virtual bool onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard);
    void hideOppenentCardAndOnAction();
    virtual void onFinished();
    
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    cocos2d::ui::Button* addButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    
    
    void initBG();
    void startGame();
    void replayGame();
    void replayGame(int startCardIndex);
    void finishReplay();

    void menuRestart(Ref* pSender);
    std::string getChoiceMessage(int action, Player* player);
    std::string getActionExecutedMessage(int action, Player* player);
    
private:
    Game*   mGame;
    Player* mPlayer1;
    Player* mPlayer2;
    AIPlayer* mAi2;
    ReplayPlayer* mReplayerPlayer;
    
    GameLayer*       mGameLayer;
    PauseLayer*      mPauseLayer;
    UserChoiceLayer* mUserChoiceLayer;
    CalcScoreLayer*  mCalcScoreLayer;
    ReplayLayer*     mReplayLayer;
    
    bool                  mChoiceSkipped;
    bool                  mReplayMode;
    bool                  mReplayPaused;
    int                   mReplayStartCardIndex;
    float                 mReplayInterval;
    PlayerActionCallBack* mReplayCallback;
    Player*               mCurrentReplayPlayer;
    int                   mCurrentReplayAction;

    
};


#endif /* defined(__DaLiPoker__PlayScene__) */
