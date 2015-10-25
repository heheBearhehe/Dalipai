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

static const int TAG_CALC_POINTS_ME        = 100;
static const int TAG_CALC_POINTS_OPP       = 101;
static const int TAG_CALC_RESULT           = 102;
static const int TAG_CALC_MYCARD_START     = 1000;
static const int TAG_CALC_OPPCARD_START    = 2000;
static const int TAG_CALC_MYLINE_START     = 3000;
static const int TAG_CALC_OPPLINE_START    = 4000;
static const int TAG_CALC_CARD_LAST_OFFSET = 999;

bool CalcScoreLayer::init(){
    if (!GameLayer::init()){
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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    float buttonWidth = 80;
    float buttonHeight = 50;
    float posY = origin.y + 200;
    
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
    
    mCurrentCardIndex = 0;
    calcNextCard();
}

void CalcScoreLayer::scheduleNextCard(){
    DelayTime * delayAction = DelayTime::create(0.2);
    CallFunc * callFunc = CallFunc::create(CC_CALLBACK_0(CalcScoreLayer::calcNextCard, this));
    this->runAction(CCSequence::createWithTwoActions(delayAction, callFunc));
}

void CalcScoreLayer::calcNextCard(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    updateGameInfo(calcPoints(mGame->getMyPlayerCardList(), mCurrentCardIndex + 1),
                   calcPoints(mGame->getOpponentCardsList(), mCurrentCardIndex + 1));
    
    updateCard(mGame->getMyPlayerCardList(), mCurrentCardIndex, origin.y + 350, TAG_CALC_MYCARD_START);
    updateCard(mGame->getOpponentCardsList(), mCurrentCardIndex, origin.y + visibleSize.height - 100, TAG_CALC_OPPCARD_START);
    
    updatePointsLine(mGame->getMyPlayerCardList(), mCurrentCardIndex, origin.y + visibleSize.height / 2 - 50, TAG_CALC_MYLINE_START);
    updatePointsLine(mGame->getOpponentCardsList(), mCurrentCardIndex, origin.y + visibleSize.height / 2 + 300, TAG_CALC_OPPLINE_START);
    
    updateResult();
    
    mCurrentCardIndex++;
    if (mCurrentCardIndex <= MAX(mGame->getMyPlayerCardList()->size(), mGame->getOpponentCardsList()->size())) {
        scheduleNextCard();
    }
}


void CalcScoreLayer::updateCard(std::vector<Card*>* cards, int cardIndex, int posY, int tagStart){
    if (cardIndex > cards->size()) {
        return;
    }
    
    this->removeChildByTag(tagStart + TAG_CALC_CARD_LAST_OFFSET);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = 80;
    float cardHeight = 120;
    float delta = cardWidth / 5;
    int posX = origin.x + 20 + cardWidth / 2;
    int visibleCardCount = 3;
    int size = MIN(cards->size(), cardIndex);
    
    for(int i = 0; i < size; i++){
        Widget* card = (Widget *)this->getChildByTag(tagStart);
        if (card == NULL) {
            if (i < size - visibleCardCount) {
                card = createPokerFront(NULL);
                delta = card->getContentSize().width / 6;
            }else{
                card = createPokerFront(cards->at(i));
                delta = card->getContentSize().width / 2;
            }
            
            card->setPosition(Vec2(posX, posY));
            this->addChild(card);
        }
        
        posX += delta;
    }
    
    if (cardIndex < cards->size()) {
        Widget* lastCard = createPokerFront(cards->at(cardIndex));
        lastCard->setTag(tagStart + TAG_CALC_CARD_LAST_OFFSET);
        lastCard->setPosition(Vec2(origin.x + visibleSize.width - 100 - cardWidth / 2, posY));
        this->addChild(lastCard);
    }
}

void CalcScoreLayer::updatePointsLine(std::vector<Card*>* cards, int cardIndex, int posY, int tagStart){
    if (cardIndex > cards->size()) {
        return;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = 80;
    float cardHeight = 120;
    float delta = cardWidth / 5 + 2;
    int posX = origin.x + 20 + cardWidth / 2;
    int maxY = posY - 60;
    int minY = maxY - 30;
    int yStep = (maxY - minY) / 12;
    int size = MIN(cards->size(), cardIndex);
    
    for(int i = 0; i < size; i++){
        DrawNode* node = (DrawNode *)this->getChildByTag(tagStart);
        Card* card = cards->at(i);
        int posYEnd   = card->getIndex() * yStep + minY;
        if (node == NULL && card != NULL) {
            node = DrawNode::create();
            if (i > 0) {
                Card* lastCard = cards->at(i - 1);
                int posYStart = lastCard->getIndex() * yStep + minY;
                node->drawSegment(Vec2(posX - delta, posYStart), Vec2(posX , posYEnd), 3, Color4F(0,0,0,1));
                node->drawDot(Vec2(posX - delta, posYStart), 10, lastCard->getScored() ? Color4F(1,1,1,1) : Color4F(0,0,0,1));
            }
            node->drawDot(Vec2(posX, posYEnd), 10, card->getScored() ? Color4F(1,1,1,1) : Color4F(0,0,0,1));
            this->addChild(node);
        }
        
        posX += delta;
    }
}

void CalcScoreLayer::updateGameInfo(int myPoints, int opponentPoints){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    {
        auto label = (Label *)this->getChildByTag(TAG_CALC_POINTS_OPP);
        if(label == NULL){
            label = Label::create();
            label->setContentSize(Size(100, 50));
            label->setSystemFontSize(50);
            label->setColor(Color3B::BLACK);
            label->setPosition(Vec2(origin.x + 200, origin.y + visibleSize.height / 2  + 150));
            label->setTag(TAG_CALC_POINTS_OPP);
            this->addChild(label, 1);
        }
        
        if (myPoints >= 0) {
            stringstream ss;
            ss << opponentPoints;
            label->setString(ss.str());
            label->setVisible(true);
        }else{
            label->setVisible(false);
        }
    }
    {
        auto label = (Label *)this->getChildByTag(TAG_CALC_POINTS_ME);
        if(label == NULL){
            label = Label::create();
            label->setContentSize(Size(100, 50));
            label->setSystemFontSize(50);
            label->setColor(Color3B::BLACK);
            label->setPosition(Vec2(origin.x + 200, origin.y + visibleSize.height / 2 + 50));
            label->setTag(TAG_CALC_POINTS_ME);
            this->addChild(label, 1);
        }
        if (myPoints >= 0) {
            stringstream ss;
            ss << myPoints;
            label->setString(ss.str());
            label->setVisible(true);
        }else{
            label->setVisible(false);
        }
    }
}

void CalcScoreLayer::updateResult(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto label = (Label *)this->getChildByTag(TAG_CALC_RESULT);
    if(label == NULL){
        label = Label::create();
        label->setContentSize(Size(100, 50));
        label->setSystemFontSize(80);
        label->setColor(Color3B::BLACK);
        label->setPosition(Vec2(origin.x + 400, origin.y + visibleSize.height / 2 + 100));
        label->setTag(TAG_CALC_RESULT);
        this->addChild(label, 1);
    }
    
    if (mCurrentCardIndex < MAX(mGame->getMyPlayerCardList()->size(), mGame->getOpponentCardsList()->size())) {
        label->setVisible(false);
        return;
    }
    
    label->setVisible(true);
    
    int oppPts = mGame->getOpponentPoints();
    int myPts = mGame->getMyPlayerPoints();
    string msg;
    
    if (myPts < oppPts) {
        msg = "落败";
    }else if (myPts > oppPts) {
        msg = "胜利";
    }else{
        msg = "平局";
    }
    label->setString(msg);
}

int CalcScoreLayer::calcPoints(vector<Card *>* cardsList, int calcSize){
    if (cardsList == NULL || cardsList->size() == 0) {
        return 0;
    }
    
    int points = 0;
    for(int i = 0; i < MIN(calcSize, cardsList->size()); i++){
        cardsList->at(i)->setScored(false);
        if(i == 0 || i == cardsList->size() - 1){
            points++;
            cardsList->at(i)->setScored(true);
        }else{
            Card* prev = cardsList->at(i - 1);
            Card* cur  = cardsList->at(i);
            Card* next = cardsList->at(i + 1);
            if ((prev->getRank() < cur->getRank() && cur->getRank() < next->getRank())
                || (prev->getRank() > cur->getRank() && cur->getRank() > next->getRank())) {
                points++;
                cur->setScored(true);
            }
        }
    }
    
    return points;
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
            if (mGameActionCallBack != NULL && !isCalculating()) {
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

bool CalcScoreLayer::isCalculating(){
    return mCurrentCardIndex <= MAX(mGame->getMyPlayerCardList()->size(), mGame->getOpponentCardsList()->size());
}