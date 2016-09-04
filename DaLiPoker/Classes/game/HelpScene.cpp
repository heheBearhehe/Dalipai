//
//  HelpScene.cpp
//  DaLiPoker
//
//  Created by lugan on 8/27/16.
//
//

#include "HelpScene.h"
#include "Utils.h"
#include "../model/def.h"
#include "GameManager.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

static const int HEADER_HEIGHT = 120;
static const int ITEM_HEIGHT   = 120;

static const int TAG_BUTTON_BACK        = 1000;

static string sGameHelpStr = "　　一副不带大小王的52张扑克牌，两人进行游戏。游戏开始两人手上没有牌，先手摸牌后两人轮流摸牌。一副牌摸完时游戏结束，结束时两人手中牌列按顺序数分。根据牌面数字形成数列，第一张牌和最后一张牌分别一分，其余牌拐点不得分，非拐点得一分。例如手中牌列为1，5，8，6，3，K，则1，5，6，K四张牌分别一分，总共得4分；8比前一张和后一张都大，3比前一张和后一张都小，这两张牌都属于拐点，不得分。算分结束后分数高者胜利，相等便是平局。\n\n　　玩家在摸得一张牌后，有三种处理牌方式：留下、给对方、丢弃。留下后该牌将成为手中牌列新的顶部牌；在摸到的牌的数字和自己手中牌列顶部牌数字相同或相邻的时候（如A和2，10和J是相邻，而A和K不算相邻）——我们称之为牌面冲突时，不能选择留下，只能丢弃或者给对方；丢弃会将牌丢到公共牌区，双方可见。在手中没牌时摸到牌后只能选择留下。\n\n　　玩家在接到对方给的牌时，当牌面数字与自己手中顶部牌冲突时，只能选择将该牌和自己顶部牌一同丢弃，否则必须将牌留下成为新的顶部牌。";

bool HelpScene::init(){
    if (!Scene::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    GameManager::getInstance()->initTopBar(this, "规则介绍");
    
    int padding = 20;
    Label* labelHelp = Label::create();
    labelHelp->setAnchorPoint(Vec2(0, 1));
    labelHelp->setContentSize(Size(visibleSize.width - padding * 2, visibleSize.height - HEADER_HEIGHT));
    labelHelp->setPosition(padding, visibleSize.height - HEADER_HEIGHT - 30);
    labelHelp->setString(sGameHelpStr);
    labelHelp->setSystemFontSize(35);
    labelHelp->setColor(R::COLOR_TEXT_SETTINGS);
    labelHelp->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    labelHelp->setDimensions(visibleSize.width - padding * 2, 0);
    this->addChild(labelHelp);
    
    return true;
}
