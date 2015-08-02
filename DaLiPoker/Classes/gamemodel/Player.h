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

class Player {
    
public:
    static const int PLAYER_CHOICE_KEEP    = 0x01;
    static const int PLAYER_CHOICE_DISCARD = 0x02;
    static const int PLAYER_CHOICE_GIVE    = 0x04;
    static const int PLAYER_CHOICE_REMOVE  = 0x08;
    
    Player();
    ~Player();
    
    string getDumpPrefix();
    
    void setTag(int tag) { mTag = tag; };
    int getTag() { return mTag; }
    void setPlayerActionCallBack(PlayerActionCallBack* callback) { mCallback = callback; }
    
    void deal(Card* card);
    void give(Card* card);
    
    void addCard(Card* card);
    void removeLastCard();
    
    Card* getLastCard();
    
    vector<Card *>* getCardList() { return mKeepCardList; }
    int             getPoints() { return calcPoints(); };
    int             calcPoints();
    
protected:
    int makeChoice(Card* card, int availableChoice);
    
    
private:
    vector<Card *>* mKeepCardList;
    int             mPoints;
    int             mTag;
    
    PlayerActionCallBack* mCallback;
    
};

#endif /* defined(__dalipoker__Player__) */
