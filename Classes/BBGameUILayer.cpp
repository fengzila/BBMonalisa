//
//  BBGameUILayer.cpp
//  puzzle
//
//  Created by FengZi on 14-1-22.
//
//

#include "BBGameUILayer.h"
#include "BBWelcomeScene.h"
#include "BBGameDataManager.h"

BBGameUILayer::BBGameUILayer()
{
    
}

BBGameUILayer::~BBGameUILayer()
{
    
}

bool BBGameUILayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    initData();
    initSprite();
    return true;
}

#pragma mark - initData
void BBGameUILayer::initData()
{
    // add your codes here...
    
}
#pragma mark - initSprite
void BBGameUILayer::initSprite()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    int fontSize = 40 * BBGameDataManager::getInstance()->getScreenScale();
    
    auto backLabel = LabelTTF::create("back", "Arial", fontSize);
    backLabel->setColor(Color3B(255, 255, 255));
    auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_1(BBGameUILayer::menuBackCallback, this));
    
//    auto beginItem = MenuItemImage::create(
//                                           "CloseNormal.png",
//                                           "CloseSelected.png",
//                                           CC_CALLBACK_1(BBWelcomeScene::menuBeginCallback, this));
    
	backItem->setPosition(Point(30 + backItem->getContentSize().width/2, visibleSize.height - 20 - backItem->getContentSize().height/2));
    
    auto tipsLabel = LabelTTF::create("hint", "Arial", fontSize);
    tipsLabel->setColor(Color3B(255, 255, 255));
    auto tipsItem = MenuItemLabel::create(tipsLabel, CC_CALLBACK_1(BBGameUILayer::menuTipsCallback, this));
    
	tipsItem->setPosition(Point(visibleSize.width - 30 - tipsItem->getContentSize().width/2, visibleSize.height - 20 - tipsItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(backItem, tipsItem, NULL);
    menu->setPosition(Point::ZERO);
    addChild(menu);
}

void BBGameUILayer::menuBackCallback(Object* pSender)
{
    Director::getInstance()->replaceScene(TransitionSlideInL::create(.5, BBWelcomeScene::createScene()));
}

void BBGameUILayer::menuTipsCallback(Object* pSender)
{
    log("tap showTips btn");
    NotificationCenter::getInstance()->postNotification("kShowTips");
}