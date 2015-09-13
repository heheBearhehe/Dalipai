//
//  AIPlayer.cpp
//  dalipoker
//
//  Created by lugan on 7/19/15.
//  Copyright (c) 2015 CooTek. All rights reserved.
//

#include "AIPlayer.h"
#include "Player.h"


AIPlayer::AIPlayer(Game* game):mGame(game){
    
}

AIPlayer::~AIPlayer(){
    
}

int AIPlayer::makeChoice(Player* player, Card* card, int availableChoice, PlayerActionCallBack* callback){
    if ((availableChoice & Player::PLAYER_CHOICE_KEEP) > 0) {
        return Player::PLAYER_CHOICE_KEEP;
    }else if ((availableChoice & Player::PLAYER_CHOICE_GIVE) > 0){
        return Player::PLAYER_CHOICE_GIVE;
    }else if ((availableChoice & Player::PLAYER_CHOICE_REMOVE_FOR_GIVE) > 0){
        return Player::PLAYER_CHOICE_REMOVE_FOR_GIVE;
    }else if ((availableChoice & Player::PLAYER_CHOICE_KEEP_FOR_GIVE) > 0){
        return Player::PLAYER_CHOICE_KEEP_FOR_GIVE;
    }else{
        LOGI("AI.ERROR makeChoice  card=[%s] availableChoice=[0x%x]", card, availableChoice);
    }
    
    return Player::PLAYER_CHOICE_AUTO;
}

bool AIPlayer::onChoiceMade(Player* player, int choice, Card* currentCard, Card* lastCard){
    return true;
}
