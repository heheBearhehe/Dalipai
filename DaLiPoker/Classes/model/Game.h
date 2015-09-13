//
//  Game.h
//  DaLiPoker
//
//  Created by lugan on 7/12/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#ifndef __DaLiPoker_Game__
#define __DaLiPoker_Game__

#include <stdio.h>
#include <vector>
#include "Utils.h"

using std::vector;

class Card;
class Player;
class Recorder;
class StateBase;


typedef enum playmode{
    AUTO = 0,
    REPLAY,
}PLAY_MODE;

typedef enum mode{
    NORMAL = 0,
    SMALL,
}GAME_MODE;

typedef enum state{
    INIT,
    PLAYER_DEAL,  // deal
    PLAYER_CHOICE,   // keep/discard/give
    PLAYER_OPPENENT_CHOICE, // auto keep/discard
    PLAYER_SWITCH,    // switch to next player
    FINISH,
    PAUSE,
}STATE;

class PlayerActionCallBack{
public:
    virtual void onPlayerAction(Player* player, int action) = 0;
    virtual void execute() = 0;
};

class PlayerChoiceListener{
public:
    virtual int makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback) = 0;
    virtual bool onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard) = 0;
};


class GameStateListener{
public:
    virtual void onActionExecuted(int action, Player* player, Card* card1, Card* card2) = 0;
    virtual void onFinished() = 0;
};



class Game : public PlayerActionCallBack{
    
public:
    
    Game(GAME_MODE mode = GAME_MODE::NORMAL);
    ~Game();
    
    bool init();
    bool setPlayer(Player* player1, Player* player2);
    bool start();
    
public:
    Player* currentPlayer() { return mCurrentPlayer; }
    Player* oppenentPlayer() { return mNextPlayer; }
    void nextState(STATE nextState);
    void onActionExecuted(int action, Player* player, Card* card1, Card* card2);
    Card* currentCard();
    void makeChoice(Player* player, Card* card, int availableChoice);
    

    virtual void onPlayerAction(Player* player, int action);
    
    void next();
    void execute();

    void pause();
    void stop();
    void onFinished();
    void replay();
    
    int  getResetCardsCount();
    vector<Card *>* getOpponentCardsList();
    int  getMyPlayerPoints();
    int  getOpponentPoints();
    vector<Card *>* getMyPlayerCardList();
    vector<Card *>* getDiscardCardList();
    
    void setPlayer1ChoiceListener(PlayerChoiceListener* l);
    void setPlayer2ChoiceListener(PlayerChoiceListener* l);
    void setGameStateListener(GameStateListener* l);
    
    
protected:
    void shuffle();
    void initCards();
    void switchPlayer();
    
    void dumpCards();
    
    StateBase* getState(STATE state);
    
    friend class StateInit;
    friend class StateDeal;
    friend class StatePlayerChoice;
    friend class StateSwitchPlayer;
    friend class StateOpponentChoice;
    
private:
    PLAY_MODE       mPlayMode;
    GAME_MODE       mGameMode;
    STATE           mState;
    vector<Card *>* mCardList;
    int             mCurrentCardIndex;
    
    vector<Card *>* mDiscardCardList;
    
    Player*         mPlayer1;
    Player*         mPlayer2;
    Player*         mCurrentPlayer;
    Player*         mNextPlayer;
    
    Recorder*       mRecorder;
    PlayerChoiceListener* mPlayerChoiceListener;
    PlayerChoiceListener* mPlayer2ChoiceListener;
    GameStateListener*    mGameStateListener;
    
    StateBase*      mCurrentState;
    vector<StateBase *>* mStateList;
    int             mPlayerAction;
};


#endif /* defined(__DaLiPoker_Game__) */
