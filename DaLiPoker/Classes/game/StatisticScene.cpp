//
//  StatisticScene.cpp
//  DaLiPoker
//
//  Created by lugan on 8/27/16.
//
//

#include "StatisticScene.h"
#include "Utils.h"
#include "../model/def.h"
#include "GameManager.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;


static const int HEADER_HEIGHT = 120;
static const int ITEM_HEIGHT   = 120;

static const int TAG_BUTTON_BACK        = 1000;


bool StatisticScene::init(){
    if (!Scene::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    GameManager::getInstance()->initTopBar(this, "统计");
    
    Label* labelHelp = Label::create();
    labelHelp->setAnchorPoint(Vec2(0, 0.5));
    labelHelp->setContentSize(Size(visibleSize.width, visibleSize.height - HEADER_HEIGHT));
    labelHelp->setPosition(0, HEADER_HEIGHT);
//    labelHelp->setString(sGameHelpStr);
    labelHelp->setSystemFontSize(30);
    labelHelp->setColor(R::COLOR_TEXT_SETTINGS);
    this->addChild(labelHelp);
    
    return true;
}


