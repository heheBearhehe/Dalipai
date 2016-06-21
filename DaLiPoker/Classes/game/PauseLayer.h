//
//  PauseLayer.h
//  DaLiPoker
//
//  Created by lugan on 8/3/15.
//
//

#ifndef __DaLiPoker__PauseLayer__
#define __DaLiPoker__PauseLayer__

#include <stdio.h>
#include <vector>
#include "cocos2d.h"
#include <CocosGUI.h>

class Game;
class GameActionCallBack;

class PauseLayer : public cocos2d::LayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(PauseLayer);
    
    void show(Game* game, GameActionCallBack* callback);
    void invalidate();
    
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    cocos2d::ui::Button* createButton(const std::string picPath, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    
    cocos2d::Node* createMenuText(const std::string& text, const cocos2d::Vec2& position);
    
private:
    Game* mGame;
    GameActionCallBack* mGameActionCallBack;
    
};


#endif /* defined(__DaLiPoker__PauseLayer__) */
