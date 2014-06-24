//
//  BBWelcomeScene.cpp
//  puzzle
//
//  Created by FengZi on 14-1-22.
//
//

#include "BBWelcomeScene.h"
#include "BBGameScene.h"
#include "GameCenterApi.h"
#include "BBGameDataManager.h"

BBWelcomeScene::BBWelcomeScene()
{
    
}

BBWelcomeScene::~BBWelcomeScene()
{
    
}

Scene* BBWelcomeScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BBWelcomeScene::create();
    scene->addChild(layer);
    return scene;
}

bool BBWelcomeScene::init()
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
void BBWelcomeScene::initData()
{
    // add your codes here...
    
}
#pragma mark - initSprite
void BBWelcomeScene::initSprite()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    int backLength = 500*BBGameDataManager::getInstance()->getScreenScale();
    
    // 添加背景层
    addChild(LayerColor::create(Color4B(40, 40, 40, 255), visibleSize.width, visibleSize.height));
    
    int fontSize = 40 * BBGameDataManager::getInstance()->getScreenScale();
    
    auto beginLabel = LabelTTF::create("New Game", "Arial", fontSize);
    beginLabel->setColor(Color3B(255, 255, 255));
    beginLabel->setAnchorPoint(Point(.5, .5));
    auto beginItem = MenuItemLabel::create(beginLabel, CC_CALLBACK_1(BBWelcomeScene::menuBeginCallback, this));
    beginItem->setTag(1);
	beginItem->setPosition(Point(visibleSize.width/2 - backLength, visibleSize.height/2 - 100 * BBGameDataManager::getInstance()->getScreenScale()));
    beginItem->runAction(Sequence::create(DelayTime::create(0.15 * 1), MoveTo::create(.2, Point(visibleSize.width/2, visibleSize.height/2 - 100 * BBGameDataManager::getInstance()->getScreenScale())), NULL));
    
    auto ranksLabel = LabelTTF::create("Ranks", "Arial", fontSize);
    ranksLabel->setColor(Color3B(255, 255, 255));
    auto ranksItem = MenuItemLabel::create(ranksLabel, CC_CALLBACK_1(BBWelcomeScene::menuBeginCallback, this));
    ranksItem->setTag(2);
    ranksItem->setPosition(Point(visibleSize.width/2 - backLength, visibleSize.height/2-200 * BBGameDataManager::getInstance()->getScreenScale()));
    ranksItem->runAction(Sequence::create(DelayTime::create(0.15 * 2), MoveTo::create(.2, Point(visibleSize.width/2, visibleSize.height/2 - 200 * BBGameDataManager::getInstance()->getScreenScale())), NULL));
    
    auto moreLabel = LabelTTF::create("Reviews", "Arial", fontSize);
    moreLabel->setColor(Color3B(255, 255, 255));
    auto moreItem = MenuItemLabel::create(moreLabel, CC_CALLBACK_1(BBWelcomeScene::menuBeginCallback, this));
    moreItem->setTag(3);
    moreItem->setPosition(Point(visibleSize.width/2 - backLength, visibleSize.height/2-300 * BBGameDataManager::getInstance()->getScreenScale()));
    moreItem->runAction(Sequence::create(DelayTime::create(0.15 * 3), MoveTo::create(.2, Point(visibleSize.width/2, visibleSize.height/2 - 300 * BBGameDataManager::getInstance()->getScreenScale())), NULL));
    
    UserDefault *ud = UserDefault::getInstance();
    log("hasLocalData is %d", ud->getBoolForKey("hasLocalData"));
    if (ud->getBoolForKey("hasLocalData")) {
        auto continueLabel = LabelTTF::create("Continue", "Arial", fontSize);
        continueLabel->setColor(Color3B(255, 255, 255));
        auto continueItem = MenuItemLabel::create(continueLabel, CC_CALLBACK_1(BBWelcomeScene::menuBeginCallback, this));
        continueItem->setTag(4);
        continueItem->setPosition(Point(visibleSize.width/2 - backLength, visibleSize.height/2));
        continueItem->runAction(MoveTo::create(.2, Point(visibleSize.width/2, visibleSize.height/2)));
        
        auto menu = Menu::create(beginItem, continueItem, ranksItem, moreItem, NULL);
        menu->setPosition(Point::ZERO);
        addChild(menu);
    } else {
        auto menu = Menu::create(beginItem, ranksItem, moreItem, NULL);
        menu->setPosition(Point::ZERO);
        addChild(menu);
    }
    
    GameCenterApi::getInstance()->showAdBanner();
}

void BBWelcomeScene::menuBeginCallback(Object* pSender)
{
    int tag = ((MenuItemLabel*)pSender)->getTag();
    UserDefault *ud = UserDefault::getInstance();
    switch (tag) {
        case 1:
            // play
            ud->setBoolForKey("hasLocalData", false);
            ud->flush();
            Director::getInstance()->replaceScene(TransitionSlideInR::create(.5, BBGameScene::createScene()));
            break;
        case 2:
            GameCenterApi::getInstance()->showLeaderboard();
            break;
        case 3:
            GameCenterApi::getInstance()->goToAppStoreEvaluate();
            break;
        case 4:
            Director::getInstance()->replaceScene(TransitionSlideInR::create(.5, BBGameScene::createScene()));
            break;
            
        default:
            break;
    }
}