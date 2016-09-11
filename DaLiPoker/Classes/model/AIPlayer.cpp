//
//  AIPlayer.cpp
//  dalipoker
//
//  Created by lugan on 7/19/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#include "AIPlayer.h"
#include "Player.h"
#include "Card.h"


AIPlayer::AIPlayer(int rankMin, int rankMax):mCardRandMin(rankMin),mCardRandMax(rankMax){
    mStrategy = 3;
    mGiveStrategy = 4;
    mGiveProb = 100;
    mGiveStrategyOffset = 3;
    mKeepMistakeProb = 0;
    mDetect = true;
    mAttack = true;
    mAttackM = 50;
    mAttackL = 90;
    mGiveMid = false;
    mNeverGiveMid = false;
    mKeepOnceScored = false;
    
    mKeepStrategyWeight = new int[3] {100, 200, 100};
    mGiveCount = 0;
    mTag = 0;
    mCardRandMax = 12;
    mCardRandMin = 0;
    mDiscardCardList = new vector<Card *>();
    mMyCardList = new vector<Card *>();
    mOpponentCardList = new vector<CountCard *>();
    reset();
    
    for(int i = 0; i < 13*13; i++){
        mStatCount[i] = 0;
        mStatKeep[i] = 0;
    }
}

AIPlayer::~AIPlayer(){
    delete mDiscardCardList;
    delete mMyCardList;
    vector<CountCard *>::iterator iter;
    for (iter = mOpponentCardList->begin(); iter != mOpponentCardList->end(); iter++){
        delete (*iter);
    }
    delete mOpponentCardList;
}

void AIPlayer::reset(){
    mGiveCount = 0;
    mDiscardCardList->clear();
    mMyCardList->clear();
    mOpponentCardList->clear();
    mOpponentUpProb = 50;
    mOpponentDownProb = 50;
    mMyLastGiveCard = NULL;
    
    if (mGiveProb < 0) {
        mGiveProb = 20;
    }
}

int AIPlayer::makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback){
    return makeChoice(player, card, availableChoice, callback, mStrategy);
}

int AIPlayer::makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback, int strategy){
    
    if ((availableChoice & Player::PLAYER_CHOICE_REMOVE_FOR_GIVE) > 0){
        return Player::PLAYER_CHOICE_REMOVE_FOR_GIVE;
    }else if ((availableChoice & Player::PLAYER_CHOICE_KEEP_FOR_GIVE) > 0){
        return Player::PLAYER_CHOICE_KEEP_FOR_GIVE;
    }
    
    vector<int> choiceList = vector<int>();
    switch (strategy) {
        case 0: // 不动
            if ((availableChoice & Player::PLAYER_CHOICE_KEEP) > 0) {
                return Player::PLAYER_CHOICE_KEEP;
            }else if ((availableChoice & Player::PLAYER_CHOICE_GIVE) > 0){
                return Player::PLAYER_CHOICE_GIVE;
            }
            break;
            
            
        case 1: // 可选项等比例随机选
            if ((availableChoice & Player::PLAYER_CHOICE_KEEP) > 0) {
                choiceList.push_back((int)Player::PLAYER_CHOICE_KEEP);
            }
            if (mStrategy > 0 && (availableChoice & Player::PLAYER_CHOICE_GIVE) > 0){
                choiceList.push_back((int)Player::PLAYER_CHOICE_GIVE);
            }
            if (mStrategy > 0 && (availableChoice & Player::PLAYER_CHOICE_DISCARD) > 0){
                choiceList.push_back((int)Player::PLAYER_CHOICE_DISCARD);
            }
            
            if (choiceList.size() > 0) {
                return choiceList.at(getRandom((int)choiceList.size()));
            }
            break;
            
        case 2: // 优先留牌，其次随机
            if ((availableChoice & Player::PLAYER_CHOICE_KEEP) > 0) {
                return Player::PLAYER_CHOICE_KEEP;
            }
            if (mStrategy > 0 && (availableChoice & Player::PLAYER_CHOICE_GIVE) > 0){
                choiceList.push_back((int)Player::PLAYER_CHOICE_GIVE);
            }
            if (mStrategy > 0 && (availableChoice & Player::PLAYER_CHOICE_DISCARD) > 0){
                choiceList.push_back((int)Player::PLAYER_CHOICE_DISCARD);
            }
            
            if (choiceList.size() > 0) {
                return choiceList.at(getRandom((int)choiceList.size()));
            }
            break;
            
        case 3:
        {
            vector<Card *>* cardList = mMyCardList;
            if ((availableChoice & Player::PLAYER_CHOICE_KEEP) > 0) {
                if (cardList->size() <= 2) {
                    return Player::PLAYER_CHOICE_KEEP;
                }
                Card* last = cardList->at(cardList->size() - 1);
                Card* last2 = cardList->at(cardList->size() - 2);
                int index = last->getRank() * 13 + card->getRank();
                mStatCount[index]++;
//                LOGF("last=[%d] current=[%d]", last->getRank(), card->getRank());
//                if (last->getRank() == 0) {
//                    LOGI("** last=[%d] current=[%d]", last->getRank(), card->getRank());
//                }
                
                bool shouldKeep = shouldKeepCard(card, last, last2);
                
                if (mKeepMistakeProb > 0 && getRandomSelect(mKeepMistakeProb)) {
                    shouldKeep = !shouldKeep;
                }
                if (shouldKeep) {
                    mStatKeep[index]++;
                    return Player::PLAYER_CHOICE_KEEP;
                }
            }
            
            if (mGiveStrategy > 0 && mStrategy > 0 && (availableChoice & Player::PLAYER_CHOICE_GIVE) > 0
                && !(mNeverGiveMid && card->getRank() >= 5 && card->getRank() <= 7 )){
                if (shouldGiveCard(card)) {
                    mGiveCount++;
                    return Player::PLAYER_CHOICE_GIVE;
                }
            }
            
            if (mStrategy > 0 && (availableChoice & Player::PLAYER_CHOICE_DISCARD) > 0){
                choiceList.push_back((int)Player::PLAYER_CHOICE_DISCARD);
            }
            
            if (choiceList.size() > 0) {
                return choiceList.at(getRandom((int)choiceList.size()));
            }else{
                if ((availableChoice & Player::PLAYER_CHOICE_DISCARD) > 0) {
                    return Player::PLAYER_CHOICE_DISCARD;
                }else if ((availableChoice & Player::PLAYER_CHOICE_GIVE) > 0) {
                    return Player::PLAYER_CHOICE_GIVE;
                }else if ((availableChoice & Player::PLAYER_CHOICE_KEEP) > 0) {
                    return Player::PLAYER_CHOICE_KEEP;
                }
            }
        }
            break;
            
        default:
            break;
    }
    
    return Player::PLAYER_CHOICE_AUTO;
}

bool AIPlayer::onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard){
    LOGI("AIPlayer.onChoiceMade AI.tag=[%d] player=[%d] choice=[%d] current=[%s] last=[%s]",
         mTag, player->getTag(), choice, currentCard == NULL? "" : currentCard->getDisplay().c_str(), lastCard == NULL? "" : lastCard->getDisplay().c_str());
    
    if (player == NULL) {
        return true;
    }
    
    if (player->getTag() != mTag) {
        mOpponentUpProb = guessUpProb(mOpponentUpProb, choice, currentCard, lastCard);
    }
    
    if (choice == Player::PLAYER_CHOICE_DISCARD) {
        mDiscardCardList->push_back(currentCard);
    }
    if (choice == Player::PLAYER_CHOICE_REMOVE_FOR_GIVE) {
        if (lastCard != NULL) {
            mDiscardCardList->push_back(lastCard);
        }
        if (currentCard != NULL) {
            mDiscardCardList->push_back(currentCard);
        }
    }
    
    if (player->getTag() == mTag && currentCard != NULL) {
        if (choice == Player::PLAYER_CHOICE_KEEP || choice == Player::PLAYER_CHOICE_KEEP_FOR_GIVE) {
            mMyCardList->push_back(currentCard);
        }else if (choice == Player::PLAYER_CHOICE_REMOVE_FOR_GIVE) {
            if (mMyCardList->size() > 1) {
                mMyCardList->pop_back();
            }
        }else if(choice == Player::PLAYER_CHOICE_GIVE){
            mMyLastGiveCard = currentCard;
        }
    }else if(player->getTag() != mTag){
        if (choice == Player::PLAYER_CHOICE_KEEP) {
            mOpponentCardList->push_back(new CountCard());
            mLastOpponentDiscardCard = NULL;
        }else if(choice == Player::PLAYER_CHOICE_KEEP_FOR_GIVE){
            CountCard* card = new CountCard();
            if (mMyLastGiveCard != NULL) {
                card->rank = mMyLastGiveCard->getRank();
                card->prob = 100;
                mMyLastGiveCard = NULL;
            }
            mOpponentCardList->push_back(card);
            mLastOpponentDiscardCard = NULL;
        }
        else if (choice == Player::PLAYER_CHOICE_REMOVE_FOR_GIVE) {
            if (mOpponentCardList->size() > 1) {
                mOpponentCardList->pop_back();
            }
            mLastOpponentDiscardCard = NULL;
        }
        else if(choice == Player::PLAYER_CHOICE_DISCARD){
            mLastOpponentDiscardCard = currentCard;
        }else if (choice == Player::PLAYER_CHOICE_GIVE){
            mLastOpponentDiscardCard = currentCard;
            
        }
    }
    
    return true;
}


bool AIPlayer::shouldKeepCard(Card* card, Card* last, Card* last2){
    vector<Card *>* cardList = mMyCardList;
    int size = (int)cardList->size();
    if (size < 2) {
        return Player::PLAYER_CHOICE_KEEP;
    }
    
    int weight[3];
    weight[0] = mKeepStrategyWeight[0];
    weight[1] = mKeepStrategyWeight[1];
    weight[2] = mKeepStrategyWeight[2];
    
    int currentRank = card->getRank();
    int lastRank = last->getRank();
    int last2Rank = last2->getRank();
    int minRank = mCardRandMin;
    int maxRank = mCardRandMax;
    int rankCount = maxRank - minRank + 1;
    int* totalScore = new int[rankCount];
    bool scoreIfKeep = false;
    
    
    if ((currentRank < lastRank && lastRank < last2Rank)
        || (currentRank > lastRank && lastRank > last2Rank)) {
        scoreIfKeep = true;
    }
    
    if (mKeepOnceScored && scoreIfKeep) {
        return true;
    }
    
    for(int rank = minRank; rank <= maxRank; rank++){
        float score[3] = {0};
        if(abs(rank - lastRank) <= 1){
            score[2] = 0;
        }else{
            float gapMin = std::min(rank - minRank - 3, maxRank - rank - 3);
            float gapMax = std::max(rank - minRank - 3, maxRank - rank - 3);
            score[2] = ((gapMax + 1)* gapMax / 2 + (gapMin > 0? (gapMin + 1) * gapMin / 2 : 0))
            / (rankCount * rankCount);
        }
        
        if (rank > lastRank + 1) {
            score[1] = ((float)maxRank - rank - 1) / rankCount;
            if(lastRank > last2Rank){
                score[0] = 1;
            }
        }else if(rank < lastRank - 1){
            score[1] = ((float)rank - minRank - 1) / rankCount;
            if(lastRank < last2Rank){
                score[0] = 1;
            }
        }
        
        if (score[1] < 0) {
            score[1] = 0;
        }
        
        int cardScore = 0;
        for (int i = 0; i < 3; i++) {
            cardScore += score[i] * weight[i];
        }
        totalScore[rank - minRank] = cardScore;
        LOGI("rank=[%d] cardScore=[%d] rankCount=[%d] minRank=[%d] score=[%5.2f %5.2f %5.2f]", rank, cardScore, rankCount, minRank, score[0], score[1], score[2]);
    }
    
    int currentScore = totalScore[currentRank - minRank];
    int topRank = 0;
    for (int i = 0; i < rankCount; i++) {
        if(totalScore[i] > currentScore){
            topRank++;
        }
    }
    
    LOGI("card=[%d %d %d] curScore=[%d] topRank=[%d]", last2Rank, lastRank, currentRank, currentScore, topRank);
    delete[] totalScore;
    
    if (scoreIfKeep) {
        if (topRank > (rankCount - 3) / 2) {
            LOGI("discard scored card!!!");
        }
    }
    
    if (topRank <= (rankCount - 3) / 2) {
        return true;
    }

    return false;
}

bool AIPlayer::shouldGiveCard(Card* card){
    int curSeq = card->getSeq();
    vector<Card *>* discardList = mDiscardCardList;
    
    Card* lastOpponentDiscardCard = NULL;
    CountCard* last = NULL;
    CountCard* last2 = NULL;
    if (mOpponentCardList->size() > 0) {
        last = mOpponentCardList->at(mOpponentCardList->size() - 1);
        if (mOpponentCardList->size() > 1) {
            last2 = mOpponentCardList->at(mOpponentCardList->size() - 2);
        }
    }
    
    if (discardList != NULL && !discardList->empty()) {
        vector<Card *>::iterator iter;
        for (iter = discardList->end() - 1; iter != discardList->begin(); iter--){
            Card* discardCard = *iter;
            if (discardCard->getTag() != this->getTag() && discardCard->getTag() > 0) {
                lastOpponentDiscardCard = (*iter);
                break;
            }
        }
    }
    
    lastOpponentDiscardCard = mLastOpponentDiscardCard;
    if (mGiveStrategy >= 1 && last2 != NULL) {
        if (last->rank == CARD_RANK_UNKNOWN && last2->rank == CARD_RANK_UNKNOWN && mLastOpponentDiscardCard == NULL) {
            int totalCardCount = (mCardRandMax - mCardRandMin + 1) * 4;
            int usedCardCount = mMyCardList->size() + mOpponentCardList->size() + mDiscardCardList->size();
            if(usedCardCount <= totalCardCount * 0.9 && getRandomSelect(mGiveProb)){
                return true;
            }
        }
    }
    
    if (mGiveStrategy > 2) {
        
        if (last2 != NULL) {
            
            if(mGiveStrategy == 3 && last->rank >= 0 && last->prob == 100){
                int offset = 1;
//                if ((abs(last->rank - card->getRank()) <= 1)) {
//                    return true;
//                }
                if ((abs(last->rank - card->getRank()) <= 1)
                    && (card->getRank() <= 1 || card->getRank() >= 11)) {
                    return true;
                }
                if ((abs(last->rank - card->getRank()) <= 1)
                    || (card->getRank() <= 1 || card->getRank() >= 11)) {
                    return false;
                }
                
                if ((last->rank == 0 && card->getRank() >= 8 + offset)
                    || (last->rank == 1 && card->getRank() >= 9 + offset)
                    || (last->rank == 2 && card->getRank() >= 10 + offset)
                    || (last->rank == 3 && card->getRank() >= 11 + offset)
                    || (last->rank == 4 && card->getRank() == 11)
                    || (last->rank == 8 && card->getRank() == 1)
                    || (last->rank == 9 && card->getRank() <= 1 - offset)
                    || (last->rank == 10 && card->getRank() <= 2 - offset)
                    || (last->rank == 11 && card->getRank() <= 3 - offset)
                    || (last->rank == 12 && card->getRank() <= 4 - offset)) {
                    return true;
                }
            }
            
            if(mGiveStrategy > 1 && last->rank >= 0 && last->prob == 100){
                if (mOpponentUpProb >= 80) {
                    if (card->getRank() > last->rank + 1) {
                        return false;
                    }
                }else if(mOpponentUpProb <= 20){
                    if (card->getRank() < last->rank - 1) {
                        return false;
                    }
                }
                
                if ((last->rank >= 11 && (last->rank - card->getRank() <= 5))
                    || (last->rank <= 1  && (card->getRank() - last->rank ) <= 5)) {
                    return false;
                }
            }
            
            
            if(mGiveStrategy > 3 && last2 != NULL && last2->rank > 0 && last2->prob == 100){
                if (abs(last2->rank - card->getRank()) <= 3) {
                    return true;
                }
            }
        }
        
        int upProbIfGive = guessUpProb(mOpponentUpProb, Player::PLAYER_CHOICE_KEEP_FOR_GIVE, card, NULL);
        if (mDetect && abs(mOpponentUpProb - 50) <= 5) {
            if (abs(upProbIfGive - 100) <= 20) {
                if (card->getRank() >= 1 && card->getRank() <= 11) {
                    return true;
                }
            }
            
            if (mGiveMid && card->getRank() >= 5 && card->getRank() <= 7) {
                return true;
            }
        }
        
        if(mAttack){
            int opponentCardSize = (int)mOpponentCardList->size();
            int last2 = 0;
            if(opponentCardSize > 1){
                CountCard* last2Card = mOpponentCardList->at(opponentCardSize - 2);
                if (last2Card->prob == 100) {
                    last2 = last2Card->rank + 1;
                }
            }
            
            if (last2 > 0) {
                if (abs(card->getRank() - (last2 - 1)) <= 2) {
                    return true;
                }
            }else if(last2 == 0){
                if (mOpponentUpProb > 50) {
                    if (mOpponentUpProb > mAttackM && upProbIfGive < mAttackL) {
                        return true;
                    }
                    
                }else if(mOpponentUpProb < 50){
                    if (mOpponentUpProb < 100 - mAttackM && upProbIfGive > 100 - mAttackL) {
                        return true;
                    }
                }
                
//                if ((mOpponentUpProb > mAttackM && upProbIfGive < mAttackL)
//                    || (mOpponentUpProb < 100 - mAttackM && upProbIfGive > 100 - mAttackL)) {
//                    return true;
//                }
            }
        }
    }
    
    if (mGiveStrategy >= 2 && lastOpponentDiscardCard != NULL && lastOpponentDiscardCard->getSeq() == curSeq - 1) {
        if (abs(card->getRank() - lastOpponentDiscardCard->getRank()) <= 2) {
            return true;
        }
    }

    
    if(mGiveStrategy == 2 && getRandomSelect(mGiveProb)){
        return true;
    }

    return false;
}


int AIPlayer::guessUpProb(int originProb, int choice, Card* currentCard, Card* lastCard){
    int upProb = 50;
    
    int current = 0;
    int last = 0;
    int last2 = 0;
    if (currentCard != NULL) {
        current = currentCard->getRank() + 1;
    }
    
    int opponentCardSize = (int)mOpponentCardList->size();
    if (opponentCardSize > 0) {
        CountCard* lastCard = mOpponentCardList->at(opponentCardSize - 1);
        if (lastCard->prob == 100) {
            last = lastCard->rank + 1;
        }
        
        if(opponentCardSize > 1){
            CountCard* last2Card = mOpponentCardList->at(opponentCardSize - 2);
            if (last2Card->prob == 100) {
                last2 = last2Card->rank + 1;
            }
        }
    }
    
    upProb = guessUpProb(originProb, choice, current, last, last2);

    if (upProb != 50) {
        LOGI("* ai.%d upProb=[%d]", this->getTag(), upProb);
    }
    
    return upProb;
}

int AIPlayer::getValueForLimit(int originValue, int limitValue, bool greater){
    if (greater) {
        return std::max(originValue, limitValue);
    }else{
        return std::min(originValue, limitValue);
    }
}

int AIPlayer::guessUpProb(int originProb, int choice, int x, int a, int b){
    int prob = -1;
    int probDelta = 0;
    if (choice == Player::PLAYER_CHOICE_KEEP_FOR_GIVE) { // 给对方牌
        if (a > 0) {
            if (x > a) {
                prob = 100;
            }else{
                prob = 0;
            }
        }else if(a == 0){
            if (b == 0) {
                prob = 100 * getValueForLimit(x - 2, 0, true) / 10;
            }else if(x < b){
                prob = 100 * getValueForLimit(x - 2, 0, true)
                / (13 - getValueForLimit(
                                         (getValueForLimit(b + 1, 13, false))
                                         - getValueForLimit(x - 1, 0, true)
                                         + 1,
                                         6, false));
                
            }else{ // x >= b
                
                prob = 100 - 100 * getValueForLimit(13 - x - 1, 0, true)
                / (13 - getValueForLimit(
                                         (getValueForLimit(x + 1, 13, false))
                                         - getValueForLimit(b - 1, 0, true)
                                         + 1,
                                         6, false));
            }
        }
        
    }else if(choice == Player::PLAYER_CHOICE_DISCARD || choice == Player::PLAYER_CHOICE_GIVE){
        if (a > 0) {
            if (abs(x - a) > 1) {
                if (a - x <= 3 && a -x > 1) {
                    probDelta += 30;
                }else if(a - x <= 5 && a - x > 3){
                    probDelta += 10;
                }else if(x - a <= 3 && x - a > 1){
                    probDelta -= 30;
                }else if(x - a <= 5 && x - a > 3){
                    probDelta -= 10;
                }
            }
        }else if(b > 0){
            // TODO
        }
        
        prob = (originProb += probDelta);
    }else if(choice == Player::PLAYER_CHOICE_KEEP){
        if (a > 0) {
            prob = 100 * getValueForLimit(13 - a - 1, 0, true) / 10;
        }else{
            if (originProb > 50) {
                prob = getValueForLimit(originProb - 20, 50, true);
            }else{
                prob = getValueForLimit(originProb + 20, 50, false);
            }
        }
    }
    
    prob = std::max(prob, 0);
    prob = std::min(prob, 100);
    
    return prob;
}

void AIPlayer::dumpStat(){
//    LOGF("AIPlayer.dumpStat");
//    for (int i = 0; i < 13*13; i++) {
//        int last = i / 13;
//        int current = i % 13;
//        LOGF("%d\t%d\t%d\t%d\t%5.1f%%", last, current, mStatKeep[i], mStatCount[i], mStatCount[i] == 0? 0 : 100 * float(mStatKeep[i]) / mStatCount[i]);
//    }
    
}



