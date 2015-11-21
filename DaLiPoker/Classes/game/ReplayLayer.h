//
//  ReplayLayer.hpp
//  DaLiPoker
//
//  Created by lugan on 10/18/15.
//
//

#ifndef ReplayLayer_h
#define ReplayLayer_h

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include <CocosGUI.h>

class Game;
class GameActionCallBack;

class ReplayLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(ReplayLayer);
    
    void show(Game* game, GameActionCallBack* callback);
    void invalidate();
    void updateReplay(bool pause);
    
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    cocos2d::ui::Button* addButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    void drawText(const std::string& text, const cocos2d::Vec2& position, const cocos2d::Size & size);
protected:
    
    
private:
    Game* mGame;
    GameActionCallBack* mGameActionCallBack;
    
};

#endif /* ReplayLayer_h */
