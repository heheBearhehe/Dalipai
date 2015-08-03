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

using std::vector;

class Card;
class Player;
class Recorder;


typedef enum playmode{
    AUTO = 0,
    REPLAY,
}PLAY_MODE;

typedef enum mode{
    NORMAL = 0,
    SMALL,
}GAME_MODE;

typedef enum state{
    NEW,
    INIT,
    PLAY,
    PAUSE,
    FINISH
}STATE;


//class GameListener{
//    virtual void onGameStateChanged(STATE state);
//};


class PlayerActionCallBack{
public:
    virtual void onPlayerAction(Player* player, int action) = 0;
};

class Game : public PlayerActionCallBack{
    
public:
    
    Game(GAME_MODE mode = GAME_MODE::NORMAL);
    ~Game();
    
    bool init();
    bool setPlayer(Player* player1, Player* player2);
    bool start();
    
    virtual void onPlayerAction(Player* player, int action);
    
    void deal();
    void next();

    void pause();
    void stop();
    void onFinished();
    void replay();
    
    int  getResetCardsCount();
    int  getOpponentCardsCount();
    int  getMyPlayerPoints();
    vector<Card *>* getMyPlayerCardList();
    vector<Card *>* getDiscardCardList();
    
    
protected:
    void shuffle();
    void initCards();
    
    void dumpCards();
    
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
};


#endif /* defined(__DaLiPoker_Game__) */
