//
//  GameLayer.cpp
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#include "GameLayer.h"
#include "../model/def.h"
#include <CocosGUI.h>

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;


bool GameLayer::init(){
    if (!Layer::init()){
        return false;
    }
    
//    test();
    invalidate();
    
    return true;
}

void GameLayer::invalidate(){
    this->removeAllChildren();
    if (mGame == NULL) {
        return;
    }
    
    updateMyCard(mGame->getMyPlayerCardList());
    updateOpponentCard(mGame->getOpponentCardsList());
    updateResetCard(mGame->getResetCardsCount());
    updateDiscardCards(mGame->getDiscardCardList());
    updateGameInfo(mGame->getMyPlayerPoints());
}

void GameLayer::test(){
    this->removeAllChildren();

    vector<Card*>* myCards = new vector<Card*>();
    myCards->push_back(new Card(0, 2));
    myCards->push_back(new Card(0, 3));
    myCards->push_back(new Card(3, 0));
    myCards->push_back(new Card(10, 2));
    myCards->push_back(new Card(11, 1));
    myCards->push_back(new Card(11, 1));
    myCards->push_back(new Card(12, 3));
    
    vector<Card*>* discardList = new vector<Card*>();
    discardList->push_back(new Card(0, 2));
    discardList->push_back(new Card(0, 3));
    discardList->push_back(new Card(3, 0));
    discardList->push_back(new Card(10, 1));
    discardList->push_back(new Card(6, 1));
    discardList->push_back(new Card(7, 1));
    discardList->push_back(new Card(3, 0));
    discardList->push_back(new Card(10, 2));
    discardList->push_back(new Card(11, 1));
    
    int opponentCardCount = 6;
    
    int resetCardCount = 10;
    int myPoints = 3;
    
    updateMyCard(myCards);
    updateDiscardCards(discardList);
    updateOpponentCard(discardList);
    updateResetCard(resetCardCount);
    updateGameInfo(myPoints);
}

void GameLayer::update(float delta){
}

void GameLayer::drawCard(Card* card, const Vec2& position, const Size & size){
    drawText(card != NULL? card->getDisplay() : "*", position, size);
}

void GameLayer::drawText(const string& text, const Vec2& position, const Size & size){
    auto label = Label::create();
    label->setContentSize(size);
    label->setString(text);
    label->setSystemFontSize(30);
    label->setColor(Color3B::BLACK);
    label->setPosition(position);
    this->addChild(label, 1);
}

void GameLayer::updateMyCard(vector<Card*>* cards){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = visibleSize.width / 10;
    float cardHeight = cardWidth * 1.5;
    float delta = cardWidth / 5;
    int posX = origin.x + 50;
    int posY = origin.y + 50 + cardHeight;
    int visibleCardCount = 3;
    int size = cards->size();
    
    for(int i = 0; i < size; i++){
        Widget* card;
        if (i < size - visibleCardCount) {
            card = createPokerFront(NULL);
            delta = card->getContentSize().width / 6;
        }else{
            card = createPokerFront(cards->at(i));
            delta = card->getContentSize().width / 2;
        }
        
        card->setPosition(Vec2(posX, posY));
        this->addChild(card);
        
        posX += delta;
    }
    
    posX += cardWidth + 20;
    stringstream ss;
    ss << cards->size();
    drawText(ss.str(), Vec2(posX, posY), Size(cardWidth, cardHeight));
}

void GameLayer::updateOpponentCard(vector<Card*>* cards){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = visibleSize.width / 10;
    float cardHeight = cardWidth * 1.5;
    float delta = cardWidth / 3;
    int posX = origin.x + 50;
    int posY = origin.y + visibleSize.height - 250;
    int size = cards->size();
    
    for(int i = 0; i < size; i++){
        //        drawCard(NULL, Vec2(posX, posY), Size(cardWidth, cardHeight));
        
        drawCard(cards->at(i), Vec2(posX, posY), Size(cardWidth, cardHeight));
        posX += cardWidth;
        if (i %10 == 9) {
            posY += cardHeight;
        }
    }
    
    posX += delta + 20;
    stringstream ss;
    ss << size;
    drawText(ss.str(), Vec2(posX, posY), Size(cardWidth, cardHeight));
}

void GameLayer::updateResetCard(int count){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = visibleSize.width / 10;
    float cardHeight = cardWidth * 1.5;
    float delta = cardHeight / 5;
    int posX = origin.x + 20;
    int posY = origin.y + visibleSize.height - 400;
    
    for(int i = 0; i < count; i++){
        drawCard(NULL, Vec2(posX, posY), Size(cardWidth, cardHeight));
        posY -= delta;
        if (count >= 15) {
            break;
        }
    }
    
    posY -= (delta + 20);
    stringstream ss;
    ss << count;
    drawText(ss.str(), Vec2(posX, posY), Size(cardWidth, cardHeight));
}

void GameLayer::updateDiscardCards(vector<Card*>* cards){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = visibleSize.width / 10;
    float cardHeight = cardWidth * 1.5;
    float delta = cardWidth;
    int startX = origin.x + 150;
    int posX = startX;
    int posY = origin.y + visibleSize.height / 2;
    int start = cards->size() - 20;
    if (start <= 0) {
        start = 0;
    }else{
        drawText("...", Vec2(posX, posY), Size(cardWidth, cardHeight));
        posX += delta;
    }
    
    for(int i = start; i < cards->size(); i++){
        drawCard(cards->at(i), Vec2(posX, posY), Size(cardWidth, cardHeight));
        posX += delta;
        if (i %10 == 9) {
            posY += cardHeight;
        }
    }
    
    posX += 20;
    stringstream ss;
    ss << cards->size();
    drawText(ss.str(), Vec2(posX, posY), Size(cardWidth, cardHeight));
}

void GameLayer::updateGameInfo(int myPoints){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    stringstream ss;
    ss << "总分: " << myPoints;
    
    auto label = Label::create();
    label->setContentSize(Size(100, 40));
    label->setString(ss.str());
    label->setSystemFontSize(30);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(origin.x + visibleSize.width - label->getContentSize().width,
                            origin.y + label->getContentSize().height + 50));
    this->addChild(label, 1);
}

void GameLayer::onFinished(){
    invalidate();
    
    int oppPts = mGame->getOpponentPoints();
    stringstream ss;
    ss << "对手得分: " << oppPts << "  ";
    
    int myPts = mGame->getMyPlayerPoints();
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
}


cocos2d::ui::Widget* GameLayer::createPokerFront(Card* card){
    auto btnCard = ImageView::create("poker_front.png");
    btnCard->ignoreContentAdaptWithSize(false);
    btnCard->setContentSize(Size(80, 120));
    
    if (card != NULL) {
        Size contentSize = btnCard->getContentSize();
        float numTextSize = 20;
        float suitTextSize = 10;
        auto cardDisplay = createPokerDisplay(card, numTextSize, suitTextSize);
        cardDisplay->setPosition(Vec2(numTextSize, contentSize.height - numTextSize * 2));
        btnCard->addChild(cardDisplay);
        
        auto cardDisplayReverse = cardDisplay->clone();
        cardDisplayReverse->setRotation(180);
        cardDisplayReverse->setPosition(Vec2(contentSize.width - numTextSize, numTextSize * 2));
        btnCard->addChild(cardDisplayReverse);
    }
    
    return btnCard;
}

cocos2d::ui::Widget* GameLayer::createPokerDisplay(Card* card, float numTextSize, float suitTextSize){
    auto widget = Widget::create();
    
    auto btnCardText = cocos2d::ui::Button::create();
    btnCardText->setTitleText(card->getDisplayNum());
    btnCardText->setTitleColor(Color3B::BLACK);
    btnCardText->setTitleFontSize(numTextSize);
    btnCardText->setPosition(Vec2(0, numTextSize));
    widget->addChild(btnCardText);
    
    
    auto btnCardSuit = cocos2d::ui::Button::create();
    btnCardSuit->setTitleText(card->getDisplaySuit());
    btnCardSuit->setTitleColor(Color3B::BLACK);
    btnCardSuit->setTitleFontSize(suitTextSize);
    btnCardSuit->setPosition(Vec2(0, 0));
    widget->addChild(btnCardSuit);
    
    return widget;
}

cocos2d::ui::Widget* GameLayer::createPokerBack(Card* card){
    auto btnCard = ImageView::create("poker_back.png");
    btnCard->ignoreContentAdaptWithSize(false);
    btnCard->setContentSize(Size(80, 120));
    return btnCard;
}






