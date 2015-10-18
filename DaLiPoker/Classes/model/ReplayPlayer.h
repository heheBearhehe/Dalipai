//
//  ReplayPlayer.h
//  DaLiPoker
//
//  Created by lugan on 10/18/15.
//
//

#ifndef ReplayPlayer_h
#define ReplayPlayer_h

#include "Game.h"
class Game;
class Player;
class Recorder;

class ReplayPlayer : public PlayerChoiceListener
{
public:
    ReplayPlayer(Game* game);
    virtual ~ReplayPlayer();
    virtual int makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback);
    virtual bool onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard);
    
    void setGame(Game* game) { mGame = game;};
    void setRecorder(Recorder* recorder) { mRecorder = recorder; }
    
protected:
    int makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback, int strategy);
private:
    Game* mGame;
    Recorder* mRecorder;
    
};
#endif /* ReplayPlayer_h */
