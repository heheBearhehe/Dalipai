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
    
    test();
    
    return true;
}

void GameLayer::invalidate(){
    this->removeAllChildren();
    if (mGame == NULL) {
        return;
    }
    
    updateMyCard(mGame->getMyPlayerCardList());
    updateOpponentCard(mGame->getOpponentCardsCount());
    updateResetCard(mGame->getResetCardsCount());
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
    discardList->push_back(new Card(10, 2));
    discardList->push_back(new Card(11, 1));
    discardList->push_back(new Card(11, 1));
    discardList->push_back(new Card(3, 0));
    discardList->push_back(new Card(10, 2));
    discardList->push_back(new Card(11, 1));
    
    int opponentCardCount = 6;
    
    int resetCardCount = 10;
    int myPoints = 3;
    
    updateMyCard(myCards);
    updateDiscardCards(discardList);
    updateOpponentCard(opponentCardCount);
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
    float delta = cardWidth / 3;
    int posX = origin.x + 50;
    int posY = origin.y + 50 + cardHeight;
    int visibleCardCount = 3;
    
    for(int i = 0; i < cards->size(); i++){
        if (i < cards->size() - visibleCardCount) {
            drawCard(NULL, Vec2(posX, posY), Size(cardWidth, cardHeight));
            posX += delta;
        }else{
            drawCard(cards->at(i), Vec2(posX, posY), Size(cardWidth, cardHeight));
            posX += cardWidth;
        }
    }
    
    posX += cardWidth + 20;
    stringstream ss;
    ss << cards->size();
    drawText(ss.str(), Vec2(posX, posY), Size(cardWidth, cardHeight));
}

void GameLayer::updateOpponentCard(int count){
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float cardWidth = visibleSize.width / 10;
    float cardHeight = cardWidth * 1.5;
    float delta = cardWidth / 3;
    int posX = origin.x + 50;
    int posY = origin.y + visibleSize.height - 200;
    
    for(int i = 0; i < count; i++){
        drawCard(NULL, Vec2(posX, posY), Size(cardWidth, cardHeight));
        posX += delta;
    }
    
    posX += delta + 20;
    stringstream ss;
    ss << count;
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
    
    for(int i = 0; i < cards->size(); i++){
        drawCard(cards->at(i), Vec2(posX, posY), Size(cardWidth, cardHeight));
        
        if (posX % 7 == 0) {
            posY -= cardHeight - 10;
            posX = startX;
        }else{
            posX += delta;
        }
    }
    
    posX += delta + 20;
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








