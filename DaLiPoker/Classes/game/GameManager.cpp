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
#include <sys/stat.h>
#include "dirent.h"
#include "unistd.h"
#include "Utils.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

static const int HEADER_HEIGHT = 120;

static const int TAG_BUTTON_BACK        = 1000;


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


static string sCharactorSoundFolder[] = {
    "",
    "guolaoshi",
    "dami",
    "daxiong",
    "dali",
    "yujie",
};

static string sSoundEffectName[] = {
    "胜利",
    "平局",
    "失败",
    "弃牌",
    "留牌",
    "给牌",
    "被给牌",
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
    initSoundEffect();
}

void GameManager::initAvatar(){
    mCurrentGameCharactor = Settings::getInstance()->opponentCharacter;
    mCurrentMyAvatar = Settings::getInstance()->myAvatar;
    
    if (mCurrentGameCharactor == CHARACTOR_RANDOM) {
        mCurrentGameCharactor = rand() % (CHARACTOR_COUNT - 1) + 1;
    }
    
    if (mCurrentMyAvatar == MY_AVATAR_TYPE_RANDOM) {
        mCurrentMyAvatar = rand() % MY_AVATAR_TYPE_COUNT + 1;
    }
}

int GameManager::getCurrentCharacter(){
    return mCurrentGameCharactor;
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


void GameManager::initTopBar(Node* node, std::string title){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto bg = Sprite::create("game_bg.jpg");
    bg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    node->addChild(bg, 0);
    
    auto bgTop = ImageView::create("bg_bottom.png");
    bgTop->setScale9Enabled(true);
    bgTop->ignoreContentAdaptWithSize(true);
    bgTop->setContentSize(Size(visibleSize.width, HEADER_HEIGHT));
    bgTop->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - HEADER_HEIGHT / 2));
    node->addChild(bgTop);
    
    auto btnBack = Layout::create();
    btnBack->setContentSize(Size(HEADER_HEIGHT, HEADER_HEIGHT));
    btnBack->setAnchorPoint(Vec2(0.5, 0.5));
    btnBack->setPosition(Vec2(40, HEADER_HEIGHT / 2));
    btnBack->setTouchEnabled(true);
    btnBack->setTag(TAG_BUTTON_BACK);
    btnBack->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(GameManager::touchEvent,this)));
    bgTop->addChild(btnBack);
    
    auto iconBack = ImageView::create("settings_back.png");
    iconBack->setPosition(Vec2(btnBack->getContentSize().width / 2, btnBack->getContentSize().height / 2));
    btnBack->addChild(iconBack);
    
    Label* labelSettings = Label::create();
    labelSettings->setContentSize(bgTop->getContentSize());
    labelSettings->setPosition(bgTop->getPosition());
    labelSettings->setString(title);
    labelSettings->setSystemFontSize(50);
    labelSettings->setColor(R::COLOR_TEXT_SETTINGS_TITLE);
    node->addChild(labelSettings);
}

void GameManager::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
    cocos2d::ui::Button* btn = (cocos2d::ui::Button*)ref;
    LOGI("UI. touchEvent  tag=[%d]", btn->getTag());
    
    if (btn->getTag() <= 0) {
        return;
    }
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            if (btn->getTag() == TAG_BUTTON_BACK) {
                Director::getInstance()->popScene();
            }
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
        default:
            break;
    }
}

void GameManager::initSoundEffect(){
    for (int charactor = 1; charactor < CHARACTOR_COUNT; charactor++) {
        for (int j = 0; j < SOUND_EFFECT_COUNT; j++) {
            mSoundEffect[charactor][j] = new vector<std::string>;
        }
        
        std::string searchPath = "sound/" + sCharactorSoundFolder[charactor];
        std::string filePath = FileUtils::getInstance()->fullPathForFilename(searchPath);
        LOGI("%s",filePath.c_str());
        
        DIR *dp;
        struct dirent *entry;
        struct stat statbuf;
        int count = 0;
        
        dp = opendir(filePath.c_str());
        chdir(filePath.c_str());
        while((entry = readdir(dp)) != NULL && count < 255)
        {
            stat(entry->d_name,&statbuf);
            if(!S_ISREG(statbuf.st_mode))
                continue;
            
            for (int j = 0; j < SOUND_EFFECT_COUNT; j++) {
                int index = (int)(string(entry->d_name)).find(sSoundEffectName[j]);
                if(index == 0){
                    mSoundEffect[charactor][j]->push_back(entry->d_name);
                    LOGI("c=[%d] type=[%d] file=[%s]", charactor, j, entry->d_name);
                }
            }
        }
    }
}

void GameManager::playSound(int charactor, int soundEffect){
    if (!Settings::getInstance()->soundEffect) {
        return;
    }
    
    if (charactor < 0 || charactor >= CHARACTOR_COUNT) {
        return;
    }
    if (soundEffect < 0 || soundEffect >= SOUND_EFFECT_COUNT) {
        return;
    }
    
    std::vector<std::string>* soundList = mSoundEffect[charactor][soundEffect];
    int size = (int)soundList->size();
    if (size > 0) {
        
        int index = getRandom(size);
        std::string soundName = soundList->at(index);
        
        std::string searchPath = "sound/" + sCharactorSoundFolder[charactor];
        std::string filename = searchPath + "/" + soundName;
        LOGI("playSound c=[%d] s=[%d] index=[%d/%d] name=[%s] filename=[%s]",charactor, soundEffect, index, size, soundName.c_str(), filename.c_str());
        SimpleAudioEngine::getInstance()->playEffect(filename.c_str());
    }
    
//    SimpleAudioEngine::getInstance()->playEffect("sound/大米/失败-大米-算你厉害.mp3");
}

