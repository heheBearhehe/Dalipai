//
//  Player.h
//  dalipoker
//
//  Created by lugan on 7/19/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#ifndef __dalipoker__Player__
#define __dalipoker__Player__

#include <stdio.h>
#include <vector>

using std::vector;
using std::string;

class Card;
class PlayerActionCallBack;
class PlayerChoiceListener;

typedef enum trend{
    NONE = 0,
    UP,
    DOWN,
}PlayerTrend;

class Player {
    
public:
    static const int PLAYER_CHOICE_AUTO    = 0x00;
    static const int PLAYER_CHOICE_KEEP    = 0x01;
    static const int PLAYER_CHOICE_DISCARD = 0x02;
    static const int PLAYER_CHOICE_GIVE    = 0x04;
    
    static const int PLAYER_CHOICE_KEEP_FOR_GIVE    = 0x08;
    static const int PLAYER_CHOICE_REMOVE_FOR_GIVE  = 0x10;
    
    static const int PLAYER_CHOICE_DEAL    = 0x20;
    
    Player();
    ~Player();
    
    void reset();
    string getDumpPrefix();
    
    void setTag(int tag) { mTag = tag; };
    int getTag() { return mTag; }
    void setPlayerActionCallBack(PlayerActionCallBack* callback) { mCallback = callback; }
    void setChoiceListener(PlayerChoiceListener* l) { mChoiceListener = l; }
    
//    void deal(Card* card);
//    void give(Card* card);
    
    void addCard(Card* card, int source);
    Card* removeLastCard();
    
    Card* getLastCard();
    
    int             getTrend();
    vector<Card *>* getCardList() { return mKeepCardList; }
    int             getPoints() { return calcPoints(); };
    int             calcPoints();
    int             getMaxCombo() { return mMaxCombo; };
    
    
    friend class StatePlayerChoice;
    friend class StateOpponentChoice;
    
protected:
//    int makeChoice(Card* card, int availableChoice);
    
    
private:
    vector<Card *>* mKeepCardList;
    int             mMaxCombo;
    int             mTag;
    
    PlayerActionCallBack* mCallback;
    PlayerChoiceListener* mChoiceListener;
    
};

#endif /* defined(__dalipoker__Player__) */
