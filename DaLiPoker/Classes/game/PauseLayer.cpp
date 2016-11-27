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

static const int TAG_MENU     = 1000;
static const int TAG_FRAME    = 1001;


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
    
    Widget* frame = Widget::create();
    frame->ignoreContentAdaptWithSize(true);
    frame->setPosition(Vec2(0, 0));
    frame->setAnchorPoint(Vec2(0, 0));
    frame->setContentSize(visibleSize);
    frame->setTag(TAG_FRAME);
    frame->setTouchEnabled(true);
    frame->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(PauseLayer::touchEvent,this)));
    this->addChild(frame);
    
    float buttonWidth = 74;
    float buttonHeight = 74;
    
    auto menuBg = ImageView::create("menu_bg.png");
    menuBg->setPosition(Vec2(origin.x + menuBg->getContentSize().width / 2, origin.y + visibleSize.height + menuBg->getContentSize().height / 2));
    this->addChild(menuBg);
    menuBg->setTag(TAG_MENU);
    menuBg->setTouchEnabled(true);
    menuBg->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(PauseLayer::touchEvent,this)));
    
    float contentHeight = menuBg->getContentSize().height;
    float menuHeight = 400;
    float posX = menuBg->getContentSize().width / 2;
    float posY = menuHeight - 50;
    float gap = 140;
    auto btnReplay = createButton("menu_replay.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY), GAME_ACTION::GAME_ACTION_RESTART);
    auto textReplay= createMenuText("重玩", Vec2(posX, posY - btnReplay->getContentSize().height / 2 - 20));
    
    auto btnExit = createButton("menu_exit.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY - gap), GAME_ACTION::GAME_ACTION_EXIT);
    auto textExit = createMenuText("退出", Vec2(posX, posY - gap - btnExit->getContentSize().height / 2 - 20));
    
    auto btnClose = createButton("menu_close.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY - gap * 2), GAME_ACTION::GAME_ACTION_RESUME);
    
    menuBg->addChild(btnReplay);
    menuBg->addChild(btnExit);
    menuBg->addChild(btnClose);
    
    menuBg->addChild(textReplay);
    menuBg->addChild(textExit);
    
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

Node* PauseLayer::createMenuText(const std::string& text, const Vec2& position){
    auto label = Label::create();
    label->setString(text);
    label->setSystemFontSize(25);
    label->setColor(R::COLOR_TEXT_MENU_TEXT);
    label->setPosition(position);
    return label;
}


void PauseLayer::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
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
        {
            if (btn->getTag() == GAME_ACTION::GAME_ACTION_RESUME || btn->getTag() == TAG_FRAME) {
                auto menuBg = this->getChildByTag(TAG_MENU);
                if (menuBg != NULL) {
                    Size visibleSize = Director::getInstance()->getVisibleSize();
                    Vec2 origin = Director::getInstance()->getVisibleOrigin();
                    MoveTo * moveTo = MoveTo::create(0.1,
                                                     Vec2(menuBg->getContentSize().width / 2,
                                                          origin.y + visibleSize.height + menuBg->getContentSize().height / 2));
                    CallFunc * callFunc = CallFunc::create(CC_CALLBACK_0(PauseLayer::onMenuExit, this));
                    
                    menuBg->runAction(CCSequence::createWithTwoActions(moveTo, callFunc));
                }else{
                    onMenuExit();
                }
                
            }else if(btn->getTag() != TAG_MENU){
                if (mGameActionCallBack != NULL) {
                    mGameActionCallBack->onGameAction(btn->getTag());
                }
            }
            
        }
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

void PauseLayer::onMenuExit(){
    if (mGameActionCallBack != NULL) {
        mGameActionCallBack->onGameAction(GAME_ACTION::GAME_ACTION_RESUME);
    }
}
