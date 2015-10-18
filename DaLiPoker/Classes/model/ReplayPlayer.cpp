//
//  ReplayPlayer.cpp
//  DaLiPoker
//
//  Created by lugan on 10/18/15.
//
//

#include "ReplayPlayer.h"
#include "Player.h"
#include "Card.h"
#include "Recorder.h"
#include "Utils.h"

ReplayPlayer::ReplayPlayer(Game* game):mGame(game){
    
}

ReplayPlayer::~ReplayPlayer(){
    
}

int ReplayPlayer::makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback){
    if ((availableChoice & Player::PLAYER_CHOICE_REMOVE_FOR_GIVE) > 0){
        return Player::PLAYER_CHOICE_REMOVE_FOR_GIVE;
    }else if ((availableChoice & Player::PLAYER_CHOICE_KEEP_FOR_GIVE) > 0){
        return Player::PLAYER_CHOICE_KEEP_FOR_GIVE;
    }
    
    if (mRecorder == NULL) {
        return Player::PLAYER_CHOICE_AUTO;
    }
    
    vector<int>* cardIndexList = mRecorder->getCardIndexList();
    vector<int>* actionList = mRecorder->getActionList();
    int actionIndex = -1;
    for (int i = 0; i < cardIndexList->size(); i++) {
        if (cardIndexList->at(i) == card->getIndex()) {
            actionIndex = i;
            break;
        }
    }
    
    if (actionList->size() > actionIndex) {
        int action = mRecorder->getAction(actionIndex, player->getTag());
        LOGI("- replay. tag=[%d] card=[%s]  action=[%d]", player->getTag(), card->getDisplay().c_str(), action);
        return action;
    }else{
        return Player::PLAYER_CHOICE_AUTO;
    }
}

bool ReplayPlayer::onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard){
    return true;
}
