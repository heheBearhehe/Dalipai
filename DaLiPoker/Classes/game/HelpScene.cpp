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
#include "DLUtils.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

static const int HEADER_HEIGHT = 120;

static string sGameHelpStr = "　　一副不带大小王的52张扑克牌，两人进行游戏。游戏开始两人手上没有牌，先手摸牌后两人轮流摸牌。一副牌摸完时游戏结束，结束时两人手中牌列按顺序数分。根据牌面数字形成一串数字序列，第一张牌和最后一张牌分别一分，其余牌拐点（比自身前后数字都大或都小的数字称为拐点）不得分，非拐点得一分。例如手中牌列为1，5，8，6，3，K，则1，5，6，K四张牌分别一分，总共得4分；8比前一张5和后一张6都大，3比前一张6和后一张K都小，这两张牌都属于拐点，不得分。算分结束后分数高者胜利，相等便是平局。\n\n　　玩家在摸得一张牌后，有三种处理牌方式：留下、给对方、丢弃。留下后该牌将成为手中牌列新的顶部牌；如果摸到的牌的数字和自己手中牌列原顶部牌数字相邻（如A和2，10和J是相邻，而A和K不算相邻）——我们称之为牌面冲突时，不能选择留下，只能丢弃或者给对方；丢弃会将牌丢到公共牌区，双方可见。在手中没牌时摸到牌后只能选择留下。\n\n　　玩家在接到对方给的牌时，当牌面数字与自己手中顶部牌冲突时，只能选择将该牌和自己顶部牌一同丢弃，否则必须将牌留下成为新的顶部牌。\n\n";

bool HelpScene::init(){
    if (!Scene::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    GameManager::getInstance()->initTopBar(this, "规则介绍");
    
    
    int padding = 20;
    Label* labelHelp = Label::create();
    labelHelp->setAnchorPoint(Vec2(0, 0));
    labelHelp->setPosition(padding, 0);
    labelHelp->setString(sGameHelpStr);
    labelHelp->setSystemFontSize(35);
    labelHelp->setColor(R::COLOR_TEXT_SETTINGS);
    labelHelp->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    labelHelp->setDimensions(visibleSize.width - padding * 2, 0);
    
    float labelHeight = labelHelp->getContentSize().height;
    
    Button* videoBtn = Button::create();
    videoBtn->setAnchorPoint(Vec2(0.5, 0));
    videoBtn->setTag(0);
    videoBtn->setTitleText("\n   ->点击观看视频教程<-   \n");
    videoBtn->setTouchEnabled(true);
    videoBtn->setTitleColor(R::COLOR_TEXT);
    videoBtn->setTitleFontSize(40);
    float btnHeight = videoBtn->getContentSize().height;
    videoBtn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(HelpScene::touchEvent,this)));
    videoBtn->setPosition(Vec2(visibleSize.width / 2, labelHeight));
    
    float contentHeight = labelHeight + btnHeight;
    
    if (contentHeight > visibleSize.height) {
        auto contentScrollView = ScrollView::create();
        contentScrollView->setAnchorPoint(Vec2(0, 0));
        contentScrollView->setContentSize(Size(visibleSize.width, visibleSize.height - HEADER_HEIGHT));
        contentScrollView->setInnerContainerSize(Size(visibleSize.width, contentHeight));
        contentScrollView->setPosition(Vec2(0, 0));
        
        contentScrollView->addChild(videoBtn);
        contentScrollView->addChild(labelHelp);
        
        this->addChild(contentScrollView);
    } else {
        labelHelp->setPosition(padding, visibleSize.height - contentHeight - HEADER_HEIGHT);
        videoBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - btnHeight - HEADER_HEIGHT));
        this->addChild(videoBtn);
        this->addChild(labelHelp);
    }
    
    return true;
}


void HelpScene::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
    cocos2d::ui::Button* btn = (cocos2d::ui::Button*)ref;
    int tag = btn->getTag();
    if (tag != 0) {
        return;
    }
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            DLUtils::openVideoUrl("http://www.bilibili.com/video/av10232497/");
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}
