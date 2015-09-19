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


AIPlayer::AIPlayer(Game* game):mGame(game){
    mStrategy = 0;
    mKeepStrategyWeight = NULL;
    mKeepStrategyWeight = new int[3] {100, 200, 100};
    mGiveStrategy = 2;
    mGiveCount = 0;
}

AIPlayer::~AIPlayer(){
    
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
            
            if ((availableChoice & Player::PLAYER_CHOICE_KEEP) > 0) {
                vector<Card *>* cardList = player->getCardList();
                int size = (int)cardList->size();
                if (size <= 2) {
                    return Player::PLAYER_CHOICE_KEEP;
                }
                
                int weight[3];
                if (mKeepStrategyWeight == NULL) {
                    weight[0] = 10;
                    weight[1] = 6;
                    weight[2] = 3;
                }else{
                    weight[0] = mKeepStrategyWeight[0];
                    weight[1] = mKeepStrategyWeight[1];
                    weight[2] = mKeepStrategyWeight[2];
                }
                
                
                Card* last = cardList->at(size - 1);
                Card* last2 = cardList->at(size - 2);
                int currentRank = card->getRank();
                int lastRank = last->getRank();
                int last2Rank = last2->getRank();
                int minRank = mGame->getMinRank();
                int maxRank = mGame->getMaxRank();
                int rankCount = maxRank - minRank + 1;
                int* totalScore = new int[rankCount];
                
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
                if (topRank <= (rankCount - 3) / 2) {
                    return Player::PLAYER_CHOICE_KEEP;
                }

            }
            
            if (mGiveStrategy > 0 && mStrategy > 0 && (availableChoice & Player::PLAYER_CHOICE_GIVE) > 0){
                int curSeq = card->getSeq();
                vector<Card *>* discardList = mGame->getDiscardCardList();
                Card* lastOpponentDiscardCard = NULL;
                if (discardList != NULL && !discardList->empty()) {
                    vector<Card *>::iterator iter;
                    for (iter = discardList->end() - 1; iter != discardList->begin(); iter--){
                        Card* discardCard = *iter;
                        if (discardCard->getTag() != player->getTag() && discardCard->getTag() > 0) {
                            lastOpponentDiscardCard = (*iter);
                            break;
                        }
                    }
                }
                if (lastOpponentDiscardCard != NULL && lastOpponentDiscardCard->getSeq() == curSeq - 1) {
                    if (abs(card->getRank() - lastOpponentDiscardCard->getRank()) <= mGiveStrategy) {
                        mGiveCount++;
                        return Player::PLAYER_CHOICE_GIVE;
                    }
                }
                
                if(getRandomSelect(20)){
                    choiceList.push_back((int)Player::PLAYER_CHOICE_GIVE);
                }
            }
            if (mStrategy > 0 && (availableChoice & Player::PLAYER_CHOICE_DISCARD) > 0){
                choiceList.push_back((int)Player::PLAYER_CHOICE_DISCARD);
            }
            
            if (choiceList.size() > 0) {
                return choiceList.at(getRandom((int)choiceList.size()));
            }
            
        }
            break;
            
        default:
            break;
    }
    
//    if ((availableChoice & Player::PLAYER_CHOICE_KEEP) > 0) {
//        return Player::PLAYER_CHOICE_KEEP;
//    }else if ((availableChoice & Player::PLAYER_CHOICE_GIVE) > 0){
//        return Player::PLAYER_CHOICE_GIVE;
//    }else if ((availableChoice & Player::PLAYER_CHOICE_REMOVE_FOR_GIVE) > 0){
//        return Player::PLAYER_CHOICE_REMOVE_FOR_GIVE;
//    }else if ((availableChoice & Player::PLAYER_CHOICE_KEEP_FOR_GIVE) > 0){
//        return Player::PLAYER_CHOICE_KEEP_FOR_GIVE;
//    }else{
//        LOGI("AI.ERROR makeChoice  card=[%s] availableChoice=[0x%x]", card, availableChoice);
//    }
    
    return Player::PLAYER_CHOICE_AUTO;
}

bool AIPlayer::onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard){
    return true;
}
