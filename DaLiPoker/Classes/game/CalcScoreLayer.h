//
//  CalcScoreLayer.h
//  DaLiPoker
//
//  Created by lugan on 10/17/15.
//
//

#ifndef __DaLiPoker__CalcScoreLayer__
#define __DaLiPoker__CalcScoreLayer__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include <CocosGUI.h>
#include "GameLayer.h"

class Game;
class GameActionCallBack;


class CalcScoreLayer : public GameLayer
{
public:
    virtual bool init();
    CREATE_FUNC(CalcScoreLayer);
    
    void show(Game* game, GameActionCallBack* callback);
    void invalidate();
    
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    cocos2d::ui::Button* addButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    
    void drawText(const std::string& text, const cocos2d::Vec2& position, const cocos2d::Size & size);
    
protected:
    void updateCard(std::vector<Card*>* cards, int cardIndex, int posY, int tagStart);
    void updatePointsLine(std::vector<Card*>* cards, int cardIndex, int posY, int tagStart);
    void updateGameInfo(int myPoints, int opponentPoints);
    void updateResult();
    void scheduleNextCard();
    void calcNextCard();
    
    int calcPoints(std::vector<Card *>* cardsList, int calcSize);
    bool isCalculating();
    
private:
    GameActionCallBack* mGameActionCallBack;
    int                 mCurrentCardIndex;
    
};



#endif /* __DaLiPoker__CalcScoreLayer__ */
