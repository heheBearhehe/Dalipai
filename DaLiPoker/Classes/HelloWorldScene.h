#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <ui/CocosGUI.h>


typedef enum buttons{
    OPEN = 0,
    CLOSE,
    PLAY,
    HELP,
    STATISTIC,
    SETTINGS,
    MENU_BG,
    MENU_BG_OPENED,
}MAIN_BUTTONS;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    
    cocos2d::ui::Button* createButton(const std::string& text, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    cocos2d::ui::Button* createMenuButton(const std::string& picPath, const cocos2d::Size & size, const cocos2d::Vec2& position, int tag);
    Node* createMenuText(const std::string& text, const cocos2d::Vec2& position);
    void showMenu(bool show);
    void touchEvent(Ref* ref, cocos2d::ui::Widget::TouchEventType type);
};

#endif // __HELLOWORLD_SCENE_H__
