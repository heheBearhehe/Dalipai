//
//  StatisticScene.cpp
//  DaLiPoker
//
//  Created by lugan on 8/27/16.
//
//

#include "StatisticScene.h"
#include "Utils.h"
#include "../model/def.h"
#include "GameManager.h"
#include "GameStat.h"
#include "Settings.h"
#include "R.h"

using namespace std;
using namespace cocos2d::ui;
USING_NS_CC;


static const int HEADER_HEIGHT = 120;
static const int ITEM_HEIGHT   = 780;



bool StatisticScene::init(){
    if (!Scene::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    GameManager::getInstance()->initTopBar(this, "统计");
    
    int padding = 20;
    int width = visibleSize.width - 2 * padding;
    int totalHeight = ITEM_HEIGHT * 6;
    auto contentView = ScrollView::create();
    contentView->setAnchorPoint(Vec2(0, 1));
    contentView->setContentSize(Size(width, visibleSize.height - HEADER_HEIGHT));
    contentView->setInnerContainerSize(Size(width, totalHeight));
    contentView->setPosition(Vec2(padding, visibleSize.height - HEADER_HEIGHT));
    this->addChild(contentView);
    
    int posX = 0;
    int posY = totalHeight - ITEM_HEIGHT;
    
    for (int i = 0; i <= 5; i++) {
        GameStat stat = Settings::getInstance()->getGameStat(i);
        Node* node = getStatNode(i, stat);
        node->setAnchorPoint(Vec2(0, 0));
        node->setPosition(Vec2(posX, posY));
        node->setContentSize(Size(width, ITEM_HEIGHT));
        
        contentView->addChild(node);
        
        auto dividerLine = DrawNode::create();
        dividerLine->setContentSize(Size(width, 2));
        dividerLine->setPosition(Vec2(0, 0));
        dividerLine->drawLine(Vec2(0,0), Vec2(width, 0), Color4F(R::COLOR_SETTINGS_DIVIDER));
        
        node->addChild(dividerLine);
        
        posY -= node->getContentSize().height;
    }
    
    return true;
}

Node* StatisticScene::getStatNode(int character, const GameStat& stat){
    Node* root = Node::create();
    root->setAnchorPoint(Vec2(0,1));

    Color3B colorNormal = R::COLOR_TEXT_STAT_NORMAL;
    Color3B colorHighlight = R::COLOR_TEXT_SETTINGS_TITLE;

    int fontSizeTitle = 50;
    int fontSizeNormal = 40;
    
    int posX = 0;
    int posY = ITEM_HEIGHT - 40;
    int lineHeight = 70;
    
    std::ostringstream os;
    // title
    string charactorName = character == 0? "所有对手" : GameManager::getInstance()->getOppnentCharactorName(character);
    Node* title = createLabel("对战-" + charactorName, fontSizeTitle, colorHighlight, Size(300, 50), Vec2(posX, posY));
    root->addChild(title);
    
    // total
    {
        posY -= 90;
        Node* label = createLabel("总共玩了", fontSizeNormal, colorNormal, Size(100, 50), Vec2(posX, posY));
        posX += label->getContentSize().width;
        
        os.str("");
        os << stat.totalGameCount << "局";
        Node* label2 = createLabel(os.str(), fontSizeNormal, colorHighlight, Size(200, 50), Vec2(posX, posY));
        
        root->addChild(label);
        root->addChild(label2);
    }
    
    // win
    {
        posX = 0;
        posY -= lineHeight;
        Node* label = createLabel("胜利", fontSizeNormal, colorNormal, Size(50, 50), Vec2(posX, posY));
        posX += label->getContentSize().width;
        
        os.str("");
        os << stat.totalWinCount << "局";
        Node* label2 = createLabel(os.str(), fontSizeNormal, colorHighlight, Size(100, 50), Vec2(posX, posY));
        posX += label2->getContentSize().width;
        
        Node* label3 = createLabel("，胜率", fontSizeNormal, colorNormal, Size(150, 50), Vec2(posX, posY));
        posX += label3->getContentSize().width;
        
        os.str("");
        os << (int)(stat.winRatio * 100 + 0.5) << "%";
        Node* label4 = createLabel(os.str(), fontSizeNormal, colorHighlight, Size(200, 50), Vec2(posX, posY));
        
        root->addChild(label);
        root->addChild(label2);
        root->addChild(label3);
        root->addChild(label4);
    }
    
    // draw
    {
        posX = 0;
        posY -= lineHeight;
        Node* label = createLabel("打平", fontSizeNormal, colorNormal, Size(50, 50), Vec2(posX, posY));
        posX += label->getContentSize().width;
        
        os.str("");
        os << stat.totalDrawCount << "局";
        Node* label2 = createLabel(os.str(), fontSizeNormal, colorHighlight, Size(100, 50), Vec2(posX, posY));
        posX += label2->getContentSize().width;
        
        Node* label3 = createLabel("，平率", fontSizeNormal, colorNormal, Size(150, 50), Vec2(posX, posY));
        posX += label3->getContentSize().width;
        
        os.str("");
        os << (int)(stat.drawRatio * 100 + 0.5) << "%";
        Node* label4 = createLabel(os.str(), fontSizeNormal, colorHighlight, Size(200, 50), Vec2(posX, posY));
        
        root->addChild(label);
        root->addChild(label2);
        root->addChild(label3);
        root->addChild(label4);
    }
    
    // combo win
    {
        posX = 0;
        posY -= lineHeight;
        Node* label = createLabel("最长连胜：", fontSizeNormal, colorNormal, Size(50, 50), Vec2(posX, posY));
        posX += label->getContentSize().width;
        
        os.str("");
        os << stat.maxComboWin << "局";
        Node* label2 = createLabel(os.str(), fontSizeNormal, colorHighlight, Size(100, 50), Vec2(posX, posY));
        posX += label2->getContentSize().width;
        
        root->addChild(label);
        root->addChild(label2);
    }
    // top points
    {
        posX = 0;
        posY -= lineHeight;
        Node* label = createLabel("单局最高分：", fontSizeNormal, colorNormal, Size(50, 50), Vec2(posX, posY));
        posX += label->getContentSize().width;
        
        os.str("");
        os << stat.topPoints << "分";
        Node* label2 = createLabel(os.str(), fontSizeNormal, colorHighlight, Size(100, 50), Vec2(posX, posY));
        posX += label2->getContentSize().width;
        
        root->addChild(label);
        root->addChild(label2);
    }
    // combo points
    {
        posX = 0;
        posY -= lineHeight;
        Node* label = createLabel("最高连续得分：", fontSizeNormal, colorNormal, Size(50, 50), Vec2(posX, posY));
        posX += label->getContentSize().width;
        
        os.str("");
        os << stat.topMyComboPoints << "分";
        Node* label2 = createLabel(os.str(), fontSizeNormal, colorHighlight, Size(100, 50), Vec2(posX, posY));
        posX += label2->getContentSize().width;
        
        root->addChild(label);
        root->addChild(label2);
    }
    // win ratio 10
    {
        posX = 0;
        posY -= lineHeight;
        Node* label = createLabel("最近10局胜率：", fontSizeNormal, colorNormal, Size(50, 50), Vec2(posX, posY));
        posX += label->getContentSize().width;
        
        os.str("");
        if (stat.winRatio10 >= 0) {
            os << (int)(stat.winRatio10 * 100) << "%";
        }else{
            os << "-";
        }
        Node* label2 = createLabel(os.str(), fontSizeNormal, colorHighlight, Size(100, 50), Vec2(posX, posY));
        posX += label2->getContentSize().width;
        
        root->addChild(label);
        root->addChild(label2);
    }
    // win ratio 20
    {
        posX = 0;
        posY -= lineHeight;
        Node* label = createLabel("最近20局胜率：", fontSizeNormal, colorNormal, Size(50, 50), Vec2(posX, posY));
        posX += label->getContentSize().width;
        
        os.str("");
        if (stat.winRatio20 >= 0) {
            os << (int)(stat.winRatio20 * 100) << "%";
        }else{
            os << "-";
        }
        Node* label2 = createLabel(os.str(), fontSizeNormal, colorHighlight, Size(100, 50), Vec2(posX, posY));
        posX += label2->getContentSize().width;
        
        root->addChild(label);
        root->addChild(label2);
    }
    // win ratio 50
    {
        posX = 0;
        posY -= lineHeight;
        Node* label = createLabel("最近50局胜率：", fontSizeNormal, colorNormal, Size(50, 50), Vec2(posX, posY));
        posX += label->getContentSize().width;
        
        os.str("");
        if (stat.winRatio50 >= 0) {
            os << (int)(stat.winRatio50 * 100) << "%";
        }else{
            os << "-";
        }
        Node* label2 = createLabel(os.str(), fontSizeNormal, colorHighlight, Size(100, 50), Vec2(posX, posY));
        posX += label2->getContentSize().width;
        
        root->addChild(label);
        root->addChild(label2);
    }
    
    
    
    return root;
}

Node* StatisticScene::createLabel(std::string text, int fontSize, Color3B fontColor, Size size, Vec2 pos){
    Label* label = Label::create();
    label->setAnchorPoint(Vec2(0, 1));
    label->setString(text);
    label->setSystemFontSize(fontSize);
    label->setColor(fontColor);
    label->setContentSize(size);
    label->setPosition(pos);
    return label;
}

