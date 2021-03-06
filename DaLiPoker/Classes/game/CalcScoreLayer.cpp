//
//  CalcScoreLayer.cpp
//  DaLiPoker
//
//  Created by lugan on 10/17/15.
//
//

#include "CalcScoreLayer.h"
#include "../model/def.h"
#include "GameManager.h"
#include "R.h"

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

static const int TAG_AVATAR_OPPONENT = 10001;
static const int TAG_AVATAR_ME       = 10002;
static const int TAG_FRAME           = 10003;


static const float CAL_DURATION_DEFAULT = 0.4;

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
    visibleSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    updateAvatar(0, 0);
    
    float buttonWidth = 200;
    float buttonHeight = 86;
    float posY = origin.y + 240;
    
    Widget* frame = Widget::create();
    frame->ignoreContentAdaptWithSize(true);
    frame->setPosition(Vec2(0, 0));
    frame->setAnchorPoint(Vec2(0, 0));
    frame->setContentSize(visibleSize);
    frame->setTag(TAG_FRAME);
    frame->setTouchEnabled(true);
    frame->addTouchEventListener(cocos2d::ui::Widget::ccWidgetTouchCallback(CC_CALLBACK_2(CalcScoreLayer::touchEvent,this)));
    this->addChild(frame);
    
    {
        auto btn = addButton("重数", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2 - 160, posY), GAME_ACTION::GAME_ACTION_RECALC_SCORE);
        btn->setPosition(Vec2(origin.x + visibleSize.width / 2 - btn->getContentSize().width - 30, posY));
    }
    {
        auto btn = addButton("再来一局", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2, posY), GAME_ACTION::GAME_ACTION_RESTART);
        btn->setPosition(Vec2(origin.x + visibleSize.width / 2, posY));
    }
    {
        auto btn = addButton("复盘", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2 + 160, posY), GAME_ACTION::GAME_ACTION_REPLAY);
        btn->setPosition(Vec2(origin.x + visibleSize.width / 2 + btn->getContentSize().width + 30, posY));
    }
    {
        auto btn = addButton("退出", Size(buttonWidth,buttonHeight), Vec2(origin.x + visibleSize.width / 2, posY), GAME_ACTION::GAME_ACTION_EXIT);
        btn->setPosition(Vec2(origin.x + visibleSize.width / 2, posY - 100));
    }
    
    mCurrentCardIndex = 0;
    mCalDuration = CAL_DURATION_DEFAULT;
    calcNextCard();
}

void CalcScoreLayer::scheduleNextCard(){
    DelayTime * delayAction = DelayTime::create(mCalDuration);
    CallFunc * callFunc = CallFunc::create(CC_CALLBACK_0(CalcScoreLayer::calcNextCard, this));
    this->runAction(CCSequence::createWithTwoActions(delayAction, callFunc));
}

void CalcScoreLayer::calcNextCard(){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    updateGameInfo(calcPoints(mGame->getMyPlayerCardList(), mCurrentCardIndex + 1),
                   calcPoints(mGame->getOpponentCardsList(), mCurrentCardIndex + 1));
    
    updateCard(mGame->getMyPlayerCardList(), mCurrentCardIndex, origin.y + 400, TAG_CALC_MYCARD_START);
    updateCard(mGame->getOpponentCardsList(), mCurrentCardIndex, origin.y + visibleSize.height - 200, TAG_CALC_OPPCARD_START);
    
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
    
//    this->removeChildByTag(tagStart + TAG_CALC_CARD_LAST_OFFSET);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = 134;
    float cardHeight = 167;
    float delta = 15;
    int posX = origin.x + 20 + cardWidth / 2 + 20;
    int visibleCardCount = 3;
    int size = MIN(cards->size(), cardIndex);
    
    for(int i = 0; i < size; i++){
        Widget* card = (Widget *)this->getChildByTag(tagStart);
        if (card == NULL) {
            if (i < size - visibleCardCount) {
                card = createPokerFront(cards->at(i), false, true);
                delta = card->getContentSize().width / 6;
            }else{
                card = createPokerFront(cards->at(i), true, true);
                delta = card->getContentSize().width / 2;
            }
            
            if (posX + 2 * cardWidth >= visibleSize.width && i < size - visibleCardCount) {
                continue;
            }
            
            card->setPosition(Vec2(posX, posY));
            this->addChild(card);
        }
        
        posX += delta;
    }
    
//    if (cardIndex < cards->size()) {
//        Widget* lastCard = createPokerFront(cards->at(cardIndex));
//        lastCard->setTag(tagStart + TAG_CALC_CARD_LAST_OFFSET);
//        lastCard->setPosition(Vec2(origin.x + visibleSize.width - 80 - cardWidth / 2, posY));
//        this->addChild(lastCard);
//    }
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
    int maxY = posY - 55;
    int minY = maxY - 35;
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
                node->drawSegment(Vec2(posX - delta, posYStart), Vec2(posX , posYEnd), 2, Color4F(0.349,0.56,0.447,1)); // 0x598f72
                if(lastCard->getSource() == CARD_SOURCE::GIVE){
                    node->drawDot(Vec2(posX - delta, posYStart), 10, Color4F(0x7a/255.0, 0xf2/255.0, 0x21/255.0, 1)); // #7af221
                }
                node->drawDot(Vec2(posX - delta, posYStart), 8, lastCard->getScored() ? Color4F(1,0.8235,0.2078,1) : Color4F(0.588,0.376,0,1));
                //  #ffd235 #966000
            }
            
            if(card->getSource() == CARD_SOURCE::GIVE){
                node->drawDot(Vec2(posX, posYEnd), 10, Color4F(0x7a/255.0, 0xf2/255.0, 0x21/255.0, 1)); // #7af221
            }
            node->drawDot(Vec2(posX, posYEnd), 8, card->getScored() ? Color4F(1,0.8235,0.2078,1) : Color4F(0.588,0.376,0,1));
            
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
            label->setSystemFontSize(40);
            label->setColor(R::COLOR_TEXT);
            label->setPosition(Vec2(origin.x + 200, origin.y + visibleSize.height / 2  + 150));
            label->setTag(TAG_CALC_POINTS_OPP);
            this->addChild(label, 1);
        }
        
        if (myPoints >= 0) {
            stringstream ss;
            ss << "对方得分: " << opponentPoints << "分";
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
            label->setSystemFontSize(40);
            label->setColor(R::COLOR_TEXT);
            label->setPosition(Vec2(origin.x + 200, origin.y + visibleSize.height / 2 + 50));
            label->setTag(TAG_CALC_POINTS_ME);
            this->addChild(label, 1);
        }
        if (myPoints >= 0) {
            stringstream ss;
            ss << "本方得分: " << myPoints << "分";
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
        label->setColor(R::COLOR_TEXT);
        label->setPosition(Vec2(origin.x + 500, origin.y + visibleSize.height / 2 + 100));
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
    
    int resultMe = 0;
    int resultOppo = 0;
    int soundEffect;
    if (myPts < oppPts) {
        msg = "落败";
        resultMe = 2;
        resultOppo = 1;
        soundEffect = SOUND_EFFECT_WIN;
    }else if (myPts > oppPts) {
        msg = "胜利";
        resultMe = 1;
        resultOppo = 2;
        soundEffect = SOUND_EFFECT_LOSE;
    }else{
        msg = "平局";
        soundEffect = SOUND_EFFECT_DRAW;
    }
    label->setString(msg);
    
    updateAvatar(resultMe, resultOppo);
    
    GameManager::getInstance()->playSound(GameManager::getInstance()->getCurrentCharacter(), soundEffect);
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
    label->setColor(R::COLOR_TEXT);
    label->setPosition(position);
    this->addChild(label, 1);
}

cocos2d::ui::Button* CalcScoreLayer::addButton(const std::string& text, const Size & size, const Vec2& position, int tag){
    auto btn = cocos2d::ui::Button::create("btn_choice.png", "btn_choice_h.png", "", TextureResType::LOCAL);
    //    btn->setUnifySizeEnabled(false);
    btn->setTag(tag);
    btn->setTitleText(text);
    
    btn->setTouchEnabled(true);
    btn->setTitleColor(Color3B::WHITE);
    btn->setTitleFontSize(35);
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
            break;
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            if (btn->getTag() == TAG_FRAME) {
                if (isCalculating()) {
                    LOGI("UI.calc.SKIP");
                    mCalDuration = 0;
                }
            }else if (mGameActionCallBack != NULL) {
                if (isCalculating()) {
                    this->stopAllActions();
                }
                mGameActionCallBack->onGameAction(btn->getTag());
            }
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


void CalcScoreLayer::updateAvatar(int resultMe, int resultOppo){
    if (this->getChildByTag(TAG_AVATAR_OPPONENT) != NULL) {
        this->removeChildByTag(TAG_AVATAR_OPPONENT);
    }
    if (this->getChildByTag(TAG_AVATAR_ME) != NULL) {
        this->removeChildByTag(TAG_AVATAR_ME);
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    visibleSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto avatarMe = GameManager::getInstance()->getMyAvatar(resultMe);
    avatarMe->setTag(TAG_AVATAR_ME);
    auto avatarOppo = GameManager::getInstance()->getOppenentAvatar(resultOppo);
    avatarOppo->setTag(TAG_AVATAR_OPPONENT);
    
    avatarMe->setPosition(Vec2(origin.x + avatarMe->getContentSize().width / 2, origin.y + avatarMe->getContentSize().height / 2 + 20));
    avatarOppo->setPosition(Vec2(origin.x + visibleSize.width - avatarOppo->getContentSize().width / 2, origin.y + visibleSize.height - avatarMe->getContentSize().height / 2 - 10));
    
    this->addChild(avatarMe);
    this->addChild(avatarOppo);
}
