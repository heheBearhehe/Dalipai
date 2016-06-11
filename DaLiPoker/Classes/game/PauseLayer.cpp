//
//  PauseLayer.cpp
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#include "PauseLayer.h"
#include "../model/def.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

LayerColor * LayerColor::create(const cocos2d::Color4B& color){
    return cocos2d::LayerColor::create(color);
}

bool PauseLayer::init(){
    if (!LayerColor::initWithColor(cocos2d::Color4B(0,0,0,128))){
        return false;
    }
    
    invalidate();
    
    return true;
}

void PauseLayer::show(Game* game, GameActionCallBack* callback){
    if (game == NULL) {
        return;
    }
    
    mGame = game;
    mGameActionCallBack = callback;
    invalidate();
}

void PauseLayer::invalidate(){
    this->removeAllChildren();
    if (mGame == NULL) {
        return;
    }
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    float buttonWidth = 74;
    float buttonHeight = 74;
    
    auto menuBg = ImageView::create("menu_bg.png");
    menuBg->setPosition(Vec2(origin.x + menuBg->getContentSize().width / 2, origin.y + visibleSize.height + menuBg->getContentSize().height / 2));
    this->addChild(menuBg);
    
    float contentHeight = menuBg->getContentSize().height;
    float menuHeight = 400;
    float posX = menuBg->getContentSize().width / 2;
    float posY = menuHeight - 80;
    float gap = 120;
    auto btnReplay = createButton("menu_replay.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY), GAME_ACTION::GAME_ACTION_RESTART);
    auto btnExit = createButton("menu_exit.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY - gap), GAME_ACTION::GAME_ACTION_EXIT);
    auto btnClose = createButton("menu_close.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY - gap * 2), GAME_ACTION::GAME_ACTION_RESUME);
    
    menuBg->addChild(btnReplay);
    menuBg->addChild(btnExit);
    menuBg->addChild(btnClose);
    
    MoveTo * moveTo = MoveTo::create(0.1,
                                     Vec2(menuBg->getContentSize().width / 2,
                                          origin.y + visibleSize.height - menuHeight + contentHeight / 2));
    menuBg->runAction(moveTo);
}

cocos2d::ui::Button* PauseLayer::createButton(const std::string picPath, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create(picPath, picPath, "", TextureResType::LOCAL);
    //    btn->setUnifySizeEnabled(false);
    btn->setTag(tag);
    
    btn->setTouchEnabled(true);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(PauseLayer::touchEvent,this)));
    
    return btn;
}

void PauseLayer::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
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