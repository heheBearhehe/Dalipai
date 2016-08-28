//
//  GameManager.h
//  DaLiPoker
//
//  Created by lugan on 7/31/16.
//
//

#ifndef GameManager_h
#define GameManager_h

#include <stdio.h>
#include "cocos2d.h"
#include <CocosGUI.h>

static const int CHARACTOR_RANDOM  = 0;
static const int CHARACTOR_GUO     = 1;
static const int CHARACTOR_DAMI    = 2;
static const int CHARACTOR_DAXIONG = 3;
static const int CHARACTOR_DALI    = 4;
static const int CHARACTOR_YUJIE   = 5;
static const int CHARACTOR_COUNT   = 5;

static const int MY_AVATAR_TYPE_RANDOM = 0;
static const int MY_AVATAR_TYPE_BOY    = 1;
static const int MY_AVATAR_TYPE_GIRL   = 2;
static const int MY_AVATAR_TYPE_COUNT  = 2;

static const int AVATAR_STATUS_NORMAL = 0;
static const int AVATAR_STATUS_WIN    = 1;
static const int AVATAR_STATUS_LOSE   = 2;

class AIPlayer;
class Game;

class GameManager
{
public:
    static GameManager* getInstance();
    static void destroy();
    
    void initAvatar();
    
    cocos2d::ui::ImageView* getOppenentAvatar(int charactor, int status);
    cocos2d::ui::ImageView* getMyAvatar(int charactor, int status);
    std::string getOppnentCharactorName(int charactor);
    std::string getMyAvatarName(int charactor);
    std::string getOppnentCharactorDescription(int charactor);
    std::string getFirstPlayDescription(int charactor);
    
    cocos2d::ui::ImageView* getOppenentAvatar(int status);
    cocos2d::ui::ImageView* getMyAvatar(int status);
    
    AIPlayer* createOppenentAIPlayer(Game* game);
    
    
    void initTopBar(cocos2d::Node* node, std::string title);
    void touchEvent(cocos2d::Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    
private:
    GameManager();
    static GameManager* sInstance;
    
private:
    cocos2d::ui::ImageView* getAvatar(std::string path);
    AIPlayer* createOppenentAIPlayer(Game* game, int charactor);
    
    int  mCurrentGameCharactor;
    int  mCurrentMyAvatar;
    
};


#endif /* GameManager_h */
