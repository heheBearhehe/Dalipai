//
//  CalcScoreLayer.cpp
//  DaLiPoker
//
//  Created by lugan on 10/17/15.
//
//

#include "CalcScoreLayer.h"
#include "../model/def.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;


bool CalcScoreLayer::init(){
    if (!Layer::init()){
        return false;
    }
    
    
    invalidate();
    
    return true;
}

void CalcScoreLayer::show(Game* game, GameActionCallBack* callback){
    if (game == NULL) {
        return;
    }
    
    mGame = game;
    mGameActionCallBack = callback;
    invalidate();
}

void CalcScoreLayer::invalidate(){
    this->removeAllChildren();
    if (mGame == NULL) {
        return;
    }
    
    
    int oppPts = mGame->getOpponentPoints();
    int myPts = mGame->getMyPlayerPoints();
    stringstream ss;
    ss << "您得分: " << myPts << " 对手得分: " << oppPts << "  ";
    
    if (myPts < oppPts) {
        ss << "您输了";
    }else if (myPts > oppPts) {
        ss << "您赢了";
    }else{
        ss << "平局";
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    this->drawText(ss.str(), Vec2(origin.x + 200, origin.y + 50), Size(200, 50));
    
    
    float buttonWidth = 80;
    float buttonHeight = 50;
    float posY = origin.y + visibleSize.height / 2 - 200;
    
    
    {
        auto btn = addButton("重数", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2 - 160, posY), GAME_ACTION::GAME_ACTION_RECALC_SCORE);
        btn->setPosition(Vec2(origin.x + visibleSize.width / 2 - btn->getContentSize().width * 5 / 4, posY));
    }
    {
        auto btn = addButton("再来一局", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2, posY), GAME_ACTION::GAME_ACTION_RESTART);
        btn->setPosition(Vec2(origin.x + visibleSize.width / 2 - btn->getContentSize().width / 4, posY));
    }
    {
        auto btn = addButton("复盘", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2 + 160, posY), GAME_ACTION::GAME_ACTION_REPLAY);
        btn->setPosition(Vec2(origin.x + visibleSize.width / 2 + btn->getContentSize().width * 3 / 4, posY));
    }
    {
        auto btn = addButton("退出", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2, posY), GAME_ACTION::GAME_ACTION_EXIT);
        btn->setPosition(Vec2(origin.x + visibleSize.width / 2 - btn->getContentSize().width / 4, posY - 100));
    }
}

void CalcScoreLayer::drawText(const string& text, const Vec2& position, const Size & size){
    auto label = Label::create();
    label->setContentSize(size);
    label->setString(text);
    label->setSystemFontSize(30);
    label->setColor(Color3B::BLACK);
    label->setPosition(position);
    this->addChild(label, 1);
}


cocos2d::ui::Button* CalcScoreLayer::addButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create("btn_choice.png", "btn_choice_h.png", "", TextureResType::LOCAL);
    //    btn->setUnifySizeEnabled(false);
    btn->setTag(tag);
    btn->setTitleText(text);
    
    btn->setTouchEnabled(true);
    btn->setTitleColor(Color3B::BLACK);
    btn->setTitleFontSize(40);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(CalcScoreLayer::touchEvent,this)));
    this->addChild(btn, 1);
    return btn;
}

void CalcScoreLayer::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
    cocos2d::ui::Button* btn = (cocos2d::ui::Button*)ref;
    LOGI("UI. touchEvent  tag=[%d]", btn->getTag());
    
    if (btn->getTag() <= 0) {
        return;
    }
    
    switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN:
            if (mGameActionCallBack != NULL) {
                mGameActionCallBack->onGameAction(btn->getTag());
            }
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}