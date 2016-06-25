//
//  ReplayLayer.cpp
//  DaLiPoker
//
//  Created by lugan on 10/18/15.
//
//

#include "ReplayLayer.h"
#include "../model/def.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

static const int TAG_MENU         = 1000;
static const int TAG_TEXT_PAUSE   = 1001;
static const int TAG_TEXT_RESUME  = 1002;
static const int TAG_TEXT_SLOW    = 1003;
static const int TAG_TEXT_FAST    = 1004;


bool ReplayLayer::init(){
    if (!Layer::init()){
        return false;
    }
    
    invalidate();
    
    return true;
}

void ReplayLayer::show(Game* game, GameActionCallBack* callback){
    if (game == NULL) {
        return;
    }
    
    mGame = game;
    mGameActionCallBack = callback;
    invalidate();
}

void ReplayLayer::invalidate(){
    this->removeAllChildren();
    if (mGame == NULL) {
        return;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float buttonWidth = 74;
    float buttonHeight = 74;
    
    auto menuBg = ImageView::create("menu_replay_bg.png");
    menuBg->setPosition(Vec2(origin.x + menuBg->getContentSize().width / 2, origin.y + visibleSize.height - menuBg->getContentSize().height / 2));
    this->addChild(menuBg);
    menuBg->setTag(TAG_MENU);
    
    float contentHeight = menuBg->getContentSize().height;
    float posX = menuBg->getContentSize().width / 2;
    float posY = menuBg->getContentSize().height - 50;
    float gap = 135;
    float textGap = 10;
    
    auto btnFast = createButton("menu_replay_fast.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY), GAME_ACTION::GAME_ACTION_REPLAY_FAST);
    auto textFast= createMenuText("快进", Vec2(posX, posY - btnFast->getContentSize().height / 2 - textGap));
    textFast->setTag(TAG_TEXT_FAST);
    
    auto btnPause = createButton("menu_replay_pause.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY - gap), GAME_ACTION::GAME_ACTION_REPLAY_PAUSE);
    auto textPause = createMenuText("暂停", Vec2(posX, posY - gap - btnPause->getContentSize().height / 2 - textGap));
    textPause->setTag(TAG_TEXT_PAUSE);
    
    auto btnResume = createButton("menu_replay_start.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY - gap), GAME_ACTION::GAME_ACTION_REPLAY_RESUME);
    auto textResume = createMenuText("恢复", Vec2(posX, posY - gap - btnResume->getContentSize().height / 2 - textGap));
    textResume->setTag(TAG_TEXT_RESUME);
    
    auto btnSlow = createButton("menu_replay_slow.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY - gap * 2), GAME_ACTION::GAME_ACTION_REPLAY_SLOW);
    auto textSlow = createMenuText("慢进", Vec2(posX, posY - gap * 2 - btnSlow->getContentSize().height / 2 - textGap));
    textSlow->setTag(TAG_TEXT_SLOW);
    
    auto btnExit = createButton("menu_replay_exit.png", Size(buttonWidth,buttonHeight), Vec2(posX, posY - gap * 3), GAME_ACTION::GAME_ACTION_REPLAY_EXIT);
    auto textExit = createMenuText("退出", Vec2(posX, posY - gap * 3 - btnExit->getContentSize().height / 2 - textGap));
    
    menuBg->addChild(btnFast);
    menuBg->addChild(btnPause);
    menuBg->addChild(btnResume);
    menuBg->addChild(btnSlow);
    menuBg->addChild(btnExit);
    
    menuBg->addChild(textFast);
    menuBg->addChild(textPause);
    menuBg->addChild(textResume);
    menuBg->addChild(textSlow);
    menuBg->addChild(textExit);
    
    updateReplay(false);
}

void ReplayLayer::drawText(const string& text, const Vec2& position, const Size & size){
    auto label = Label::create();
    label->setContentSize(size);
    label->setString(text);
    label->setSystemFontSize(30);
    label->setColor(R::COLOR_TEXT);
    label->setPosition(position);
    this->addChild(label, 1);
}


cocos2d::ui::Button* ReplayLayer::createButton(const std::string picPath, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create(picPath, picPath, "", TextureResType::LOCAL);
    btn->setTag(tag);
    
    btn->setTouchEnabled(true);
    btn->setContentSize(size);
    btn->setPosition(position);
    btn->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(ReplayLayer::touchEvent,this)));
    
    return btn;
}

Node* ReplayLayer::createMenuText(const std::string& text, const Vec2& position){
    auto label = Label::create();
    label->setString(text);
    label->setSystemFontSize(25);
    label->setColor(R::COLOR_TEXT_MENU_TEXT);
    label->setPosition(position);
    return label;
}

void ReplayLayer::touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type){
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
            if (mGameActionCallBack != NULL) {
                mGameActionCallBack->onGameAction(btn->getTag());
            }
            
            if (btn->getTag() == GAME_ACTION_REPLAY_PAUSE) {
                updateReplay(true);
            }else if(btn->getTag() == GAME_ACTION_REPLAY_RESUME){
                updateReplay(false);
            }
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
}

void ReplayLayer::updateReplay(bool pause){
    auto menu = this->getChildByTag(TAG_MENU);
    
    cocos2d::ui::Button* btnResume = (cocos2d::ui::Button*)menu->getChildByTag(GAME_ACTION_REPLAY_RESUME);
    Node* textResume = menu->getChildByTag(TAG_TEXT_RESUME);
    cocos2d::ui::Button* btnPause = (cocos2d::ui::Button*)menu->getChildByTag(GAME_ACTION_REPLAY_PAUSE);
    Node* textPause = menu->getChildByTag(TAG_TEXT_PAUSE);
    if (pause) {
        btnResume->setVisible(true);
        textResume->setVisible(true);
        
        btnPause->setVisible(false);
        textPause->setVisible(false);
    }else{
        btnResume->setVisible(false);
        textResume->setVisible(false);
        
        btnPause->setVisible(true);
        textPause->setVisible(true);
    }
}


