//
//  StatisticScene.h
//  DaLiPoker
//
//  Created by lugan on 8/27/16.
//
//

#ifndef StatisticScene_h
#define StatisticScene_h

#include <stdio.h>
#include "cocos2d.h"
#include <ui/CocosGUI.h>
#include "GameStat.h"

class StatisticScene : public cocos2d::Scene
{
public:
    virtual bool init();
    
    CREATE_FUNC(StatisticScene);
    
    Node* getStatNode(int character, const GameStat& stat);
    
    Node* createLabel(std::string text, int fontSize, cocos2d::Color3B fontColor, cocos2d::Size size, cocos2d::Vec2 pos);
    
private:
    
};

#endif /* StatisticScene_h */
