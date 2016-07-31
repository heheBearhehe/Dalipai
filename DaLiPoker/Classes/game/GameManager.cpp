//
//  GameManager.cpp
//  DaLiPoker
//
//  Created by lugan on 7/31/16.
//
//

#include "GameManager.h"
#include <CocosGUI.h>
#include "Settings.h"
#include "../model/def.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;


static string sAvatarCharactor[] = {
    "",
    "郭",
    "米",
    "雄",
    "李",
    "钰",
};

static string sAvatarMe[] = {
    "",
    "男",
    "女",
};

static string sAvatarStatus[] = {
    "正",
    "喜",
    "悲",
};


GameManager* GameManager::sInstance = NULL;

GameManager* GameManager::getInstance(){
    if (sInstance == NULL) {
        sInstance = new GameManager();
    }
    return sInstance;
}

void GameManager::destroy(){
    delete sInstance;
    sInstance = NULL;
}

GameManager::GameManager(){
    mCurrentGameCharactor = 0;
    mCurrentMyAvatar = 0;

}

void GameManager::initAvatar(){
    mCurrentGameCharactor = Settings::getInstance()->opponentCharacter;
    mCurrentMyAvatar = Settings::getInstance()->myAvatar;
    
    if (mCurrentGameCharactor == CHARACTOR_RANDOM) {
        mCurrentGameCharactor = rand() % CHARACTOR_COUNT + 1;
    }
    
    if (mCurrentMyAvatar == MY_AVATAR_TYPE_RANDOM) {
        mCurrentMyAvatar = rand() % MY_AVATAR_TYPE_COUNT + 1;
    }
}

cocos2d::ui::ImageView* GameManager::getOppenentAvatar(int status){
    return getAvatar("avatar/" + sAvatarCharactor[mCurrentGameCharactor] + "-" + sAvatarStatus[status] + ".png");
}

cocos2d::ui::ImageView* GameManager::getMyAvatar(int status){
    return getAvatar("avatar/" + sAvatarMe[mCurrentMyAvatar] + "-" + sAvatarStatus[status] + ".png");
}

cocos2d::ui::ImageView* GameManager::getAvatar(std::string path){
    auto avatar = cocos2d::ui::ImageView::create(path);
    avatar->ignoreContentAdaptWithSize(false);
    avatar->setContentSize(Size(110, 110));
    return avatar;
}

AIPlayer* GameManager::createOppenentAIPlayer(Game* game){
    return createOppenentAIPlayer(game, mCurrentGameCharactor);
}

AIPlayer* GameManager::createOppenentAIPlayer(Game* game, int charactor){
    AIPlayer* player = new AIPlayer(game->getMinRank(), game->getMaxRank());
    
    int s0[3]={100, 200, 100};
    int s1[3]={1, 4, 10};
    int s2[3]={3, 9, 12};
    switch (charactor) {
        case CHARACTOR_GUO:
            player->setStrategy(2);
            player->setGiveStrategy(0);
            player->setGiveProb(0);
            player->setName(" GUO ");
            break;
            
        case CHARACTOR_DAMI:
            player->setGiveStrategy(1);
            player->setGiveProb(5);
            player->setKeepStrategyWeight(s1);
            player->setNeverGiveMid(true);
            player->setName("DAMI ");
            break;
            
        case CHARACTOR_DAXIONG:
            player->setGiveStrategy(3);
            player->setGiveProb(10);
            player->setName("BEAR ");
            player->setKeepStrategyWeight(s2);
            player->setDetect(false);
            player->setAttack(false, 0, 0);
            break;
            
        case CHARACTOR_DALI:
            player->setGiveStrategy(4);
            player->setGiveProb(30);
            player->setName("DALI ");
            player->setGiveMid(true);
            break;
            
        case CHARACTOR_YUJIE:
            player->setGiveStrategy(4);
            player->setGiveProb(0);
            player->setName("YUJIE");
            break;
            
        default:
            break;
    }
    
    return player;
}

