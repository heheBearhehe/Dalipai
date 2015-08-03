//
//  GameLayer.h
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#ifndef __DaLiPoker__GameLayer__
#define __DaLiPoker__GameLayer__

#include <stdio.h>
#include <vector>

class Game;
class Card;

class GameLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(GameLayer);
    
    void setGame(Game* game) { mGame = game;}
    void invalidate();
    virtual void update(float delta);
    void onFinished();
    
protected:
    void updateMyCard(std::vector<Card*>* cards);
    void updateOpponentCard(int count);
    void updateResetCard(int count);
    void updateDiscardCards(std::vector<Card*>* cards);
    void updateGameInfo(int myPoints);
    
    
    void drawText(const std::string& text, const cocos2d::Vec2& position, const cocos2d::Size & size);
    void drawCard(Card* card, const cocos2d::Vec2& position, const cocos2d::Size & size);
    
    void test();
    
private:
    Game* mGame;
    
};

#endif /* defined(__DaLiPoker__GameLayer__) */
