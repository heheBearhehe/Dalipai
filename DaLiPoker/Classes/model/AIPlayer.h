//
//  AIPlayer.h
//  dalipoker
//
//  Created by lugan on 7/19/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#ifndef __dalipoker__AIPlayer__
#define __dalipoker__AIPlayer__

#include "Game.h"
class Player;

using std::vector;

static const int CARD_RANK_UNKNOWN = -1;
static const int CARD_MAX_PROB = 100;

struct CountCard{
    int rank;
    int prob;
    CountCard(int rank = CARD_RANK_UNKNOWN, int prob = 0) { this->rank = rank; this->prob = prob; }
};


class AIPlayer : public PlayerChoiceListener
{
public:
    AIPlayer(int rankMin = 0, int rankMax = 12);
    virtual ~AIPlayer();
    virtual void reset();
    virtual int makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback);
    virtual bool onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard);
    
    void setStrategy(int strategy) { mStrategy = strategy; };
    int getStategy() { return mStrategy; }
    void setKeepStrategyWeight(int weight[]) { mKeepStrategyWeight = weight; }
    int* getKeepStrategyWeight() { return mKeepStrategyWeight; }
    
    void setGiveProb(int prob){ mGiveProb = prob; }
    int getGiveProb() { return mGiveProb; }
    void setGiveStrategy(int strategy){ mGiveStrategy = strategy; }
    int getGiveStrategy() { return mGiveStrategy; }
    void setGiveStrategyOffset(int offset){ mGiveStrategyOffset = offset; }
    int getGiveStrategyOffset() { return mGiveStrategyOffset; }
    int getGiveCount() { return mGiveCount; }
    void setTag(int tag) { mTag = tag; };
    int getTag() { return mTag; }
    
    int getGuessedUpProb() { return mOpponentUpProb; }
    vector<CountCard *>* getGuessedOpponentCardList() { return mOpponentCardList; };
    void dumpStat();
    
protected:
    int makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback, int strategy);
    
    int guessUpProb(int choice, Card* currentCard, Card* lastCard);
    bool shouldKeepCard(Card* card, Card* last, Card* last2);
    bool shouldGiveCard(Card* card);
private:
    int mCardRandMin;
    int mCardRandMax;
    
    vector<Card *>* mDiscardCardList;
    vector<Card *>* mMyCardList;
    vector<CountCard *>* mOpponentCardList;
    
    int mStrategy;
    int* mKeepStrategyWeight;
    int mGiveStrategy;
    int mGiveStrategyOffset;
    int mGiveCount;
    int mGiveProb;
    int mTag;
    
    Card* mMyLastGiveCard;
    Card* mLastOpponentDiscardCard;
    int mOpponentUpProb;
    int mOpponentDownProb;
    
    
    int mStatCount[13*13];
    int mStatKeep[13*13];
};

#endif /* defined(__dalipoker__AIPlayer__) */
