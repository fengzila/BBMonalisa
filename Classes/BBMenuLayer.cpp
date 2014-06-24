//
//  BBMenuLayer.cpp
//  puzzle
//
//  Created by FengZi on 14-4-8.
//
//

#include "BBMenuLayer.h"
#include "BBGameScene.h"

BBMenuLayer::BBMenuLayer()
{
    
}

BBMenuLayer::~BBMenuLayer()
{
    
}

bool BBMenuLayer::init()
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
void BBMenuLayer::initData()
{
    m_visibleSize = Director::getInstance()->getVisibleSize();
    
    m_curPage = 1;
    m_totalPages = 4;
    m_picWidth = m_visibleSize.width;
}

void BBMenuLayer::initSprite()
{
    m_scrollViewContent = Layer::create();
    addChild(m_scrollViewContent);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(BBMenuLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(BBMenuLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(BBMenuLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_scrollViewContent);
    
    for (int i = 1; i <= m_totalPages; i++) {
        //背景
        Sprite *spt = Sprite::create(String::createWithFormat("help_%d.png", i-1)->getCString());
        spt->setPosition(Point(m_visibleSize.width/2 + m_picWidth*(i-1), m_visibleSize.height/2));
        m_scrollViewContent->addChild(spt);
        
        if (i == m_totalPages) {
            MenuItemImage *playItem = MenuItemImage::create(
                                                            "btn_play.png",
                                                            "btn_play.png",
                                                            "",
                                                            this,
                                                            menu_selector(BBMenuLayer::menuBeginCallback));
            playItem->setPosition(Point(spt->getContentSize().width/2, spt->getContentSize().height/3));
            playItem->setTag(1);
            
            auto menu = Menu::create(playItem, NULL);
            menu->setPosition(Point::ZERO);
            spt->addChild(menu);
        }
    }
}

bool BBMenuLayer::onTouchBegan(Touch *touch, Event * event)
{
    Point location = Director::getInstance()->convertToGL(touch->getLocation());
    
    m_beginX = location.x;
    m_preX = location.x;
    return true;
}

void BBMenuLayer::onTouchMoved(Touch *touch, Event * event)
{
    Point location = Director::getInstance()->convertToGL(touch->getLocation());
    
    m_scrollViewContent->setPositionX(m_scrollViewContent->getPositionX() + location.x - m_preX);
    
    m_preX = location.x;
}

void BBMenuLayer::onTouchEnded(Touch *touch, Event * event)
{
    Point location = Director::getInstance()->convertToGL(touch->getLocation());
    
    float diffX = location.x - m_beginX;
    if (diffX < -100) {
        // 往下翻
        nextPage();
    } else if (diffX > 100) {
        // 上
        prePage();
    } else {
        // 原位
        m_scrollViewContent->setPositionX(- (m_curPage-1)*m_picWidth);
    }
}

void BBMenuLayer::onTouchCancelled(Touch *touch, Event * event)
{
    
}

int BBMenuLayer::validPageValue(int value)
{
    if ( value == 0 ) value = 1;
    if ( value == m_totalPages + 1 ) value = m_totalPages;
    
    return value;
}

void BBMenuLayer::menuBeginCallback(Object* pSender)
{
    int tag = ((MenuItemLabel*)pSender)->getTag();
    UserDefault *ud = UserDefault::getInstance();
    switch (tag) {
        case 1:
            ud->setBoolForKey("notNewUser", true);
            Director::getInstance()->replaceScene(TransitionSlideInR::create(.5, BBGameScene::createScene()));
            break;
        default:
            break;
    }
    ud->flush();
}

void BBMenuLayer::nextPage()
{
    m_curPage = validPageValue(m_curPage + 1);
    if (m_curPage == m_totalPages) {
        m_scrollViewContent->setPositionX(- 3*m_picWidth);
    } else {
        m_scrollViewContent->setPositionX(- (m_curPage-1)*m_picWidth);
    }
}

void BBMenuLayer::prePage()
{
    m_curPage = validPageValue(m_curPage - 1);
    if (m_curPage == 1) {
        m_scrollViewContent->setPositionX(0);
    } else {
        m_scrollViewContent->setPositionX(-(m_curPage-1)*m_picWidth);
    }
}