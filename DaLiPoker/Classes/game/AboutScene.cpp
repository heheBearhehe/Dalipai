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
#include "R.h"


using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;

static const int HEADER_HEIGHT = 120;

//\t制表符的使用是hardcode！直接写空格会造成空格换行
static string sAboutStr = "\n新语相声：\n\n　　用逻辑玩转大脑！由材料博士李宏烨带领的一大群来自上海交通大学的大脑洞，用语言传递思维，他们是一支文化界年轻且有潜力的原创力量。\n\n　　在他们高笑高品位的幽默方程式中，观众是作品的第一元素，让你在头脑风暴中开怀大笑。在高科技相声中，没有地域限制，男女老少都会喜爱，只要肯思考、愿意笑、你渴望正能量。\n\n　　在这个大数据时代，他们用有限元技术模拟幽默创作的逻辑搞笑，用独有且有深度的视角和工程原创技术进行幽默和喜剧的创作，两年前就曾被一些业内人士誉为“目前新相声的高地”。\n\n　　相信在不久的未来他们将会成为国内文化市场中一块新的具有竞争力的文化品牌。\n\n　　微信公众号：新语相声\n　　新浪微博：新语相声俱乐部\n   ";
static string sNamesStr = "\n制作鸣谢：\n\n扑克牌玩法：大李\n游戏程序设计：大雄\n游戏程序编写：悲剧、大雄\nUI设计：Renee、大米\n牌面手绘：小夏\nAI配音：郭老师、大米、大雄、钰姐、大李\n\n ";

bool AboutScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    GameManager::getInstance()->initTopBar(this, "关于");
    
    
    int paddingSide = 20;
    
    Label* labelNames = Label::create();
    labelNames->setAnchorPoint(Vec2(0, 0));
    labelNames->setPosition(paddingSide, 0);
    labelNames->setString(sNamesStr);
    labelNames->setSystemFontSize(35);
    labelNames->setColor(R::COLOR_TEXT_SETTINGS);
    labelNames->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    labelNames->setDimensions(visibleSize.width - paddingSide * 2, 0);
    
    float labelNamesHeight = labelNames->getContentSize().height;
    
    float dividerHeight = 2;
    auto dividerLine = DrawNode::create();
    dividerLine->setContentSize(Size(visibleSize.width, dividerHeight));
    dividerLine->setPosition(Vec2(0, 0));
    dividerLine->drawLine(Vec2(0, labelNamesHeight), Vec2(visibleSize.width, labelNamesHeight), Color4F(R::COLOR_SETTINGS_DIVIDER));
    
    Label* labelAbout = Label::create();
    labelAbout->setAnchorPoint(Vec2(0, 0));
    labelAbout->setPosition(paddingSide, labelNamesHeight + dividerHeight);
    labelAbout->setString(sAboutStr);
    labelAbout->setSystemFontSize(35);
    labelAbout->setColor(R::COLOR_TEXT_SETTINGS);
    labelAbout->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    labelAbout->setDimensions(visibleSize.width - paddingSide * 2, 0);
    
    float labelAboutHeight = labelAbout->getContentSize().height;
    
    auto logoImageView = cocos2d::ui::ImageView::create("logo_xinyu.png");
    logoImageView->ignoreContentAdaptWithSize(false);
    logoImageView->setAnchorPoint(Vec2(0.5, 0));
    logoImageView->setPosition(Vec2(visibleSize.width / 2, labelAboutHeight + labelNamesHeight + dividerHeight));
    float logoPaddingTop = 20;
    float logoWidth = visibleSize.width * 2 / 5;
    float logoHeight = logoWidth * logoImageView->getContentSize().height / logoImageView->getContentSize().width;
    logoImageView->setContentSize(Size(logoWidth, logoHeight));
    
    float contentHeight = labelAboutHeight + labelNamesHeight + dividerHeight + logoHeight + logoPaddingTop;
    
//    LOGI("Bear:#####label content height:%f; totalHeight:%d", labelAbout->getContentSize().height, totalHeight);
    auto contentScrollView = ScrollView::create();
    contentScrollView->setAnchorPoint(Vec2(0, 0));
    contentScrollView->setContentSize(Size(visibleSize.width, visibleSize.height - HEADER_HEIGHT));
    contentScrollView->setInnerContainerSize(Size(visibleSize.width, contentHeight));
    contentScrollView->setPosition(Vec2(0, 0));
    
    contentScrollView->addChild(labelNames);
    contentScrollView->addChild(dividerLine);
    contentScrollView->addChild(logoImageView);
    contentScrollView->addChild(labelAbout);

    this->addChild(contentScrollView);
    
    
    return true;
}

