//
//  SettingsScene.h
//  DaLiPoker
//
//  Created by lugan on 10/17/15.
//
//

#ifndef SettingsScene_h
#define SettingsScene_h

#include <stdio.h>
#include "cocos2d.h"
#include <CocosGUI.h>
#include "cocos-ext.h"
#include "GameManager.h"

class SettingsScene : public cocos2d::Scene
{
public:
    virtual bool init();
    
    CREATE_FUNC(SettingsScene);
    cocos2d::ui::Button* addButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    
    cocos2d::ui::Widget* createAvatar(int charactor, bool oppnent);
    cocos2d::Label* createLabel(const std::string& text);
    
    cocos2d::Layer* createPlayerDialog();
    cocos2d::ui::Widget* createAvatarInDialog(int charactor, cocos2d::Vec2 position, cocos2d::Size size, bool selected);
    
    cocos2d::ui::Widget* getSwitcher(bool selected, int tag);
    void onClickSwitcher(cocos2d::ui::Widget* switcher);
    
    cocos2d::ui::Widget* addItem(const std::string& title, int actionTag, cocos2d::Node* rightItem);
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
    
    std::string getFirstPlayerDesc(int firstPlayer);
    std::string getGameModeDesc(int gameMode);
    
    void invalidate();
private:
    cocos2d::Layer* mDialog;
    cocos2d::Label* mCharactorDesc;
    cocos2d::ui::Widget* mCharactorFrame[CHARACTOR_COUNT];
    cocos2d::ui::Layout* mBtnH;
};


#endif /* SettingsScene_h */
