//
//  AboutScene.cpp
//  DaLiPoker
//
//  Created by Bear on 2017/4/30.
//
//

#include "AboutScene.h"
#include "Utils.h"
#include "../model/def.h"
#include "GameManager.h"


using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

static const int HEADER_HEIGHT = 120;

static string sAboutStr = "\n新语相声：\n\n    用逻辑玩转大脑！由材料博士李宏烨带领的一大群来自上海交通大学的大脑洞，用语言传递思维，他们是一支文化界年轻且有潜力的原创力量。\n\n    在他们高笑高品位的幽默方程式中，观众是作品的第一元素，让你在头脑风暴中开怀大笑。在高科技相声中，没有地域限制，男女老少都会喜爱，只要肯思考、愿意笑、你渴望正能量。\n\n    在这个大数据时代，他们用有限元技术模拟幽默创作的逻辑搞笑，用独有且有深度的视角和工程原创技术进行幽默和喜剧的创作，两年前就曾被一些业内人士誉为“目前新相声的高地”。\n\n    相信在不久的未来他们将会成为国内文化市场中一块新的具有竞争力的文化品牌。\n\n    微信公众号：新语相声\n    新浪微博：新语相声俱乐部\n\n\n\n制作鸣谢：\n\n扑克牌玩法：大李\n游戏程序设计：大雄\n游戏程序编写：悲剧、大雄\nUI设计：Renee、大米\nAI配音：郭老师、大米、大雄、钰姐、大李\n\n";

bool AboutScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    GameManager::getInstance()->initTopBar(this, "关于");
    
    int padding = 20;
    Label* labelAbout = Label::create();
    labelAbout->setAnchorPoint(Vec2(0, 0));
//    labelAbout->setContentSize(Size(visibleSize.width - padding * 2, totalHeight));//Bear:TEST
    labelAbout->setPosition(padding, 0);
    labelAbout->setString(sAboutStr);
    labelAbout->setSystemFontSize(35);
    labelAbout->setColor(R::COLOR_TEXT_SETTINGS);
    labelAbout->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    labelAbout->setDimensions(visibleSize.width - padding * 2, 0);
    
    float labelHeight = labelAbout->getContentSize().height;
    
//    LOGI("Bear:#####label content height:%f; totalHeight:%d", labelAbout->getContentSize().height, totalHeight);
    if (labelHeight > visibleSize.height - HEADER_HEIGHT) {
        auto contentView = ScrollView::create();
        contentView->setAnchorPoint(Vec2(0, 0));
        contentView->setContentSize(Size(visibleSize.width, visibleSize.height - HEADER_HEIGHT));
        contentView->setInnerContainerSize(Size(visibleSize.width, labelHeight));
        contentView->setPosition(Vec2(0, 0));
        contentView->addChild(labelAbout);
    
        this->addChild(contentView);
    } else {
        labelAbout->setPosition(padding, visibleSize.height - HEADER_HEIGHT - labelHeight);
        this->addChild(labelAbout);
    }
    
    
    return true;
}

