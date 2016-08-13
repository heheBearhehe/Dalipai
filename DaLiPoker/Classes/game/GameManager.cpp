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

static string sCharactorName[] = {
    "随机",
    "郭老师",
    "大米",
    "大雄",
    "大李",
    "钰姐",
};

static string sMyAvatarName[] = {
    "随机",
    "男",
    "女",
};


static string sCharactorDescription[] = {
    "随机",
    "呆萌的对手，个性内向，对胜负无欲无求，无论说相声还是玩儿牌都能给你带来最大的乐趣！",
    "一般的对手，玩牌水平起伏不定，和她玩牌你必须做好应付高手的准备，才能享受虐菜的快感。",
    "普通的对手，打牌时听天由命，赢了他也别骄傲，输了他……也可以换个人试试。",
    "激进的对手，急性子中的战斗急！想方设法地要控制对手的得分，至于自己得分多少？只要能让对手不爽就够了。",
    "高深的对手，步步为营，绵里藏针，手机AI根本不足以表现她的可怕，你和真人玩一把才会懂。",
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

cocos2d::ui::ImageView* GameManager::getOppenentAvatar(int charactor, int status){
    return getAvatar("avatar/" + sAvatarCharactor[charactor] + "-" + sAvatarStatus[status] + ".png");
}

cocos2d::ui::ImageView* GameManager::getMyAvatar(int charactor, int status){
    return getAvatar("avatar/" + sAvatarMe[charactor] + "-" + sAvatarStatus[status] + ".png");
}

std::string getOppnentCharactorDescription(int charactor){
    return sCharactorDescription[charactor];
}

std::string GameManager::getOppnentCharactorName(int charactor){
    return sCharactorName[charactor];
}

std::string GameManager::getMyAvatarName(int charactor){
    return sMyAvatarName[charactor];
}

cocos2d::ui::ImageView* GameManager::getOppenentAvatar(int status){
    return getOppenentAvatar(mCurrentGameCharactor, status);
}

cocos2d::ui::ImageView* GameManager::getMyAvatar(int status){
    return getMyAvatar(mCurrentMyAvatar, status);
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

