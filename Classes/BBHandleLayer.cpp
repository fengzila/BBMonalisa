//
//  BBHandleLayer.cpp
//  puzzle
//
//  Created by FengZi on 14-1-22.
//
//

#include "BBHandleLayer.h"
#include "BBBlockSprite.h"
#include "GameCenterApi.h"
#include "BBGameDataManager.h"
#include "GameCenterApi.h"

#define kBlockLength     140

BBHandleLayer::BBHandleLayer()
{
    
}

BBHandleLayer::~BBHandleLayer()
{
    m_blockArr->release();
    NotificationCenter::getInstance()->removeObserver(this, "kShowTips");
}

bool BBHandleLayer::init()
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
void BBHandleLayer::initData()
{
    m_visibleSize = Director::getInstance()->getVisibleSize();
    
    m_blockLength = kBlockLength * BBGameDataManager::getInstance()->getScreenScale();
    
    m_row = 5;
    m_col = 4;
    
    m_countDown = 2;
    
    m_blockArr = Array::create();
    m_blockArr->retain();
    
    m_isReady = false;
    m_showTips = false;
    m_moves = 0;
    m_over = false;
    
    m_mixTimes = 100;
    m_blockAnimOver = true;
    m_idleBlockLastDirection = 0;
    m_curIndex = m_row * m_col + 1;
    
    UserDefault *ud = UserDefault::getInstance();
    if (ud->getBoolForKey("hasLocalData")) {
        m_moves = ud->getIntegerForKey("moves");
    }
}
#pragma mark - initSprite
void BBHandleLayer::initSprite()
{
    m_testLayer = LayerColor::create(Color4B(255, 0, 0, 255), 10, 10);
    addChild(m_testLayer);
    
    createHandleBg();
    
    createIdleBlock();

    createBlocks();
    
    addEventListener();
    
    m_testLayer->setPosition(Point(m_relativeX, m_relativeY));
    
    NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BBHandleLayer::showTips), "kShowTips", NULL);
    
    if (BBGameDataManager::getInstance()->getScreenScale() != 2) {
        GameCenterApi::getInstance()->hiddenAdBanner();
    }
}

void BBHandleLayer::createHandleBg()
{
    auto bgLayer = LayerColor::create(Color4B(40, 40, 40, 255), m_visibleSize.width, m_visibleSize.height);
    addChild(bgLayer);
    
    m_blockBgSpt = Sprite::create("bg_handle.png");
    m_blockBgSpt->setPosition(Point(m_visibleSize.width/2, (m_visibleSize.height - 50*BBGameDataManager::getInstance()->getScreenScale())/2));
    m_blockBgSpt->setOpacity(0);
    addChild(m_blockBgSpt);
    m_relativeX = m_blockBgSpt->getPositionX()-m_blockBgSpt->getContentSize().width/2 + 5*BBGameDataManager::getInstance()->getScreenScale();
    m_relativeY = m_blockBgSpt->getPositionY()-m_blockBgSpt->getContentSize().height/2 + 5*BBGameDataManager::getInstance()->getScreenScale() + m_blockLength/2;
    log("m_relativeX is %f, m_relativeY is %f", m_relativeX, m_relativeY);
    
    m_blockContainer = Layer::create();
    m_blockContainer->setCascadeOpacityEnabled(true);
    addChild(m_blockContainer);
    
    // 步数显示
    m_movesLabel = LabelTTF::create(__String::createWithFormat("Moves:%d", m_moves)->getCString(), "Arial", 50 * BBGameDataManager::getInstance()->getScreenScale());
    m_movesLabel->setColor(Color3B(255, 255, 255));
//    m_movesLabel->setHorizontalAlignment();
//    m_movesLabel->setAnchorPoint(Point(0, 1));
    m_movesLabel->setOpacity(0);
    m_movesLabel->setPosition(Point(m_relativeX + 1 * m_blockLength + (m_col - 1) * m_blockLength / 2, m_relativeY + m_row * m_blockLength + 60*BBGameDataManager::getInstance()->getScreenScale()));
    addChild(m_movesLabel);
}

void BBHandleLayer::createBlocks()
{
    m_texture = TextureCache::sharedTextureCache()->addImage("puzzel.png");
    
    int i = 0;
    for (int r = 1; r <= m_row; r++) {
        for (int c = 1; c <= m_col; c++) {
            auto frame = SpriteFrame::createWithTexture(m_texture, Rect(m_blockLength*(c - 1), m_blockLength*(m_row - r), m_blockLength, m_blockLength));
            auto blockSpt = BBBlockSprite::create("bg_block.png");
            float blockX = m_relativeX + m_blockLength * (c - 1) + blockSpt->getContentSize().width/2;
            float blockY = m_relativeY +m_blockLength * (r - 1) + blockSpt->getContentSize().height/2;
            blockSpt->setPosition(Point(blockX, blockY));
            blockSpt->setOrder(computeIndex(blockX, blockY));
            blockSpt->setTag(computeIndex(blockX, blockY));
            m_blockContainer->addChild(blockSpt);
            
            auto spt = Sprite::createWithSpriteFrame(frame);
            float scaleRate = 129.0 / 130.0;
            spt->setScale(scaleRate, scaleRate);
            spt->setPosition(Point(blockSpt->getContentSize().width/2, blockSpt->getContentSize().height/2));
            blockSpt->addChild(spt);
            
            blockSpt->setCascadeOpacityEnabled(true);
            blockSpt->setOpacity(0);
            
            if (r == m_row && c == m_col) {
                // 最后一个
                blockSpt->runAction(Sequence::create(DelayTime::create(0.05 * i), JumpTo::create(0.3f, Point(blockX, blockY), 30, 1), FadeIn::create(.3), CallFunc::create(CC_CALLBACK_0(BBHandleLayer::beginCountDown, this)), NULL));
            } else {
                blockSpt->runAction(Sequence::create(DelayTime::create(0.05 * i), JumpTo::create(0.3f, Point(blockX, blockY), 30, 1), FadeIn::create(.3), NULL));
            }
            
            m_blockArr->addObject(blockSpt);
            
            i++;
        }
    }
}

void BBHandleLayer::createIdleBlock()
{
    auto idleBlockSpt = BBBlockSprite::create("IdleBlock.png");
    
    float blockX = m_relativeX + m_blockLength * 0 + idleBlockSpt->getContentSize().width/2;
    float blockY = m_relativeY +m_blockLength * (m_row - 0) + idleBlockSpt->getContentSize().height/2;
    
    idleBlockSpt->setPosition(Point(blockX, blockY));
    idleBlockSpt->setTag(computeIndex(blockX, blockY));
    idleBlockSpt->setOrder(computeIndex(blockX, blockY));
    m_blockContainer->addChild(idleBlockSpt);
    
    idleBlockSpt->setOpacity(0);
    
    // 加入到集合末尾
    m_blockArr->addObject(idleBlockSpt);
}

void BBHandleLayer::showTips(Object* obj)
{
    if (!m_isReady) {
        return;
    }
    if (m_showTips) {
        return;
    }
    
    Object *spt = NULL;
    int i = 0;
    CCARRAY_FOREACH(m_blockArr, spt)
    {
        auto blockSpt = (BBBlockSprite*)spt;
        int order = blockSpt->getOrder();
        int c = order % m_col;
        int r = floor(order / m_col) + 1;
        if (c == 0){
            c = m_col;
            r = r - 1;
        }
        
        float blockX = m_relativeX + m_blockLength * (c - 1) + blockSpt->getContentSize().width/2;
        float blockY = m_relativeY +m_blockLength * (r - 1) + blockSpt->getContentSize().height/2;
        
        blockSpt->runAction(Sequence::createWithTwoActions(DelayTime::create(0.03 * i), JumpTo::create(0.3f, Point(blockX, blockY), 30, 1)));
        
        i++;
    }
    
    m_tipsLayer = LayerColor::create(Color4B(0, 0, 0, 50));
    addChild(m_tipsLayer);
    
    LabelTTF *tipsTitleLabel = LabelTTF::create("Tap To Continue", "Arial", 40*BBGameDataManager::getInstance()->getScreenScale());
    tipsTitleLabel->setColor(Color3B(255, 255, 255));
    tipsTitleLabel->setPosition(Point(m_visibleSize.width/2, m_visibleSize.height/2 - 260*BBGameDataManager::getInstance()->getScreenScale()));
    m_tipsLayer->addChild(tipsTitleLabel);
    
    tipsTitleLabel->runAction(RepeatForever::create(Sequence::create(FadeIn::create(.45), FadeOut::create(.65),DelayTime::create(.3), NULL)));
    
    m_showTips = true;
}

void BBHandleLayer::removeTipsLayer()
{
    if (!m_showTips) {
        return;
    }
    
    Object *spt = NULL;
    int i = 0;
    CCARRAY_FOREACH(m_blockArr, spt)
    {
        auto blockSpt = (BBBlockSprite*)spt;
        int order = blockSpt->getTag();
        int c = order % m_col;
        int r = floor(order / m_col) + 1;
        if (c == 0){
            c = m_col;
            r = r - 1;
        }
        
        float blockX = m_relativeX + m_blockLength * (c - 1) + blockSpt->getContentSize().width/2;
        float blockY = m_relativeY +m_blockLength * (r - 1) + blockSpt->getContentSize().height/2;
        
        blockSpt->runAction(Sequence::createWithTwoActions(DelayTime::create(0.03 * i), JumpTo::create(0.3f, Point(blockX, blockY), 30, 1)));
        i++;
    }
    
    m_tipsLayer->removeFromParentAndCleanup(true);
    
    m_showTips = false;
}

void BBHandleLayer::beginCountDown()
{
    UserDefault *ud = UserDefault::getInstance();
    if (ud->getBoolForKey("hasLocalData")) {
        // 继续上局进度玩
        
        Object *spt = NULL;
        int i = 0;
        CCARRAY_FOREACH(m_blockArr, spt)
        {
            i++;
            auto blockSpt = (BBBlockSprite*)spt;
//            log("lastOrder is %d", blockSpt->getOrder());
            __String *key = __String::createWithFormat("%d", blockSpt->getOrder());
            int tag = ud->getIntegerForKey(key->getCString());
            blockSpt->setTag(tag);
//            log("lastTag is %d", tag);
            
            int c = tag % m_col;
            int r = floor(tag / m_col) + 1;
            if (c == 0){
                c = m_col;
                r = r - 1;
            }
            
            float blockX = m_relativeX + m_blockLength * (c - 1) + blockSpt->getContentSize().width/2;
            float blockY = m_relativeY +m_blockLength * (r - 1) + blockSpt->getContentSize().height/2;
            blockSpt->setTag(computeIndex(blockX, blockY));
            
            blockSpt->setPosition(Point(blockX, blockY));
        }
        
        showMovesLabel();
        m_isReady = true;
        return;
    }
    schedule(schedule_selector(BBHandleLayer::update), 1);
}

void BBHandleLayer::update(float dt)
{
    m_countDown--;
    
    if (m_countDown > 0) {
        Object *spt = NULL;
        CCARRAY_FOREACH(m_blockArr, spt)
        {
            auto blockSpt = (BBBlockSprite*)spt;
            
//            int angle = 5;
//            blockSpt->runAction(Sequence::create(RotateBy::create(.065, -angle), RotateBy::create(.065, angle*2), RotateBy::create(.065, -angle*2), RotateBy::create(.065, angle*2), RotateBy::create(.065, -angle), NULL));
//            blockSpt->runAction(Sequence::create(ScaleTo::create(0.15, 1.35), ScaleTo::create(0.1, 1), NULL));
//            blockSpt->runAction(JumpTo::create(.2, blockSpt->getPosition(), 15, 1));
            
            int order = blockSpt->getOrder();
            int c = order % m_col;
            if (c == 0){
                c = m_col;
            }
            blockSpt->runAction(Sequence::create(DelayTime::create(0.15 * c), ScaleTo::create(.15, 1.10), ScaleTo::create(.1, 1), NULL));
        }
    } else {
        
        // 打乱重排
//        blockDrop();
        
        mixBlock();
        
        unschedule(schedule_selector(BBHandleLayer::update));
    }
}

void BBHandleLayer::addEventListener()
{
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(true);
    m_listener->onTouchBegan = CC_CALLBACK_2(BBHandleLayer::onTouchBegan, this);
    m_listener->onTouchMoved = CC_CALLBACK_2(BBHandleLayer::onTouchMoved, this);
    m_listener->onTouchEnded = CC_CALLBACK_2(BBHandleLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);
}

void BBHandleLayer::blockDrop()
{
    Object *spt = NULL;
    CCARRAY_FOREACH(m_blockArr, spt)
    {
        auto blockSpt = (BBBlockSprite*)spt;
        
        int order = blockSpt->getOrder();
        int c = order % m_col;
        if (c == 0){
            c = m_col;
        }
        
        int randNum = (rand()%10) + 1;
        blockSpt->runAction(Sequence::create(DelayTime::create(0.15 * randNum), JumpTo::create(0.4f, Point(m_relativeX + ((rand()%(m_blockLength*m_col)) + 1), m_relativeY), 350, 1), NULL));
    }

}

// 打乱重排
void BBHandleLayer::mixBlock()
{
    m_blockAnimOver = false;
    mixBlockOneByOne();
}

void BBHandleLayer::mixBlockOneByOne()
{
    m_mixTimes--;
    if (m_mixTimes <= 0) {
        m_blockAnimOver = true;
        showMovesLabel();
        return;
    }
    
    auto idleSpt = (BBBlockSprite*)m_blockContainer->getChildByTag(m_curIndex);
    
    int targetIndex = 0;
    BBBlockSprite *targetSpt;
    
    __Array *indexArr = Array::create();
    indexArr->retain();
    
    __Array *directionArr = Array::create();
    directionArr->retain();
    
    int c = m_curIndex % m_col;
    int r = floor(m_curIndex / m_col) + 1;
    if (c == 0){
        c = m_col;
        r = r - 1;
    }
    
    for (int i = 1; i <= 4; i++) {
        switch (i) {
            case 1:
                if (m_idleBlockLastDirection == 3) {
                    continue;
                }
                if (c == 1) {
                    if (r == m_row + 1) {
                        continue;
                    }
                } else if (r == m_row) {
                    continue;
                }
                targetIndex = m_curIndex + m_col;
                
                log("up, %d", targetIndex);
                
                indexArr->addObject(Integer::create(targetIndex));
                directionArr->addObject(Integer::create(1));
                break;
            case 2:
                if (m_idleBlockLastDirection == 4) {
                    continue;
                }
                if (c == m_col || r == m_row + 1) {
                    continue;
                }
                targetIndex = m_curIndex + 1;
                log("left, %d", targetIndex);
                indexArr->addObject(Integer::create(targetIndex));
                directionArr->addObject(Integer::create(2));
                break;
            case 3:
                if (r != m_row + 1 && m_idleBlockLastDirection == 1) {
                    continue;
                }
                if (r == 1) {
                    continue;
                }
                targetIndex = m_curIndex - m_col;
                
                log("down, %d", targetIndex);
                
                indexArr->addObject(Integer::create(targetIndex));
                directionArr->addObject(Integer::create(3));
                break;
            case 4:
                if (m_idleBlockLastDirection == 2) {
                    continue;
                }
                if (c == 1) {
                    continue;
                }
                log("right, %d", targetIndex);
                targetIndex = m_curIndex - 1;
                
                indexArr->addObject(Integer::create(targetIndex));
                directionArr->addObject(Integer::create(4));
                break;
            default:
                break;
        }
    }
    
    if (indexArr->count() == 0) {
        m_blockAnimOver = true;
        m_mixTimes = 0;
        return;
    }
    int randIndex = (arc4random() % indexArr->count()) + 0;
    __Integer *index = (__Integer*)indexArr->getObjectAtIndex(randIndex);
    targetIndex = index->getValue();
    
    m_idleBlockLastDirection = ((__Integer*)directionArr->getObjectAtIndex(randIndex))->getValue();
    
    targetSpt = (BBBlockSprite*)m_blockContainer->getChildByTag(targetIndex);
    Point curPoint = idleSpt->getPosition();
    int curTag = idleSpt->getTag();
    idleSpt->setPosition(targetSpt->getPosition());
    idleSpt->setTag(targetSpt->getTag());
    targetSpt->runAction(Sequence::create(MoveTo::create(.015, curPoint), CallFunc::create([this](){
        mixBlockOneByOne();
    }), NULL));
    
    targetSpt->setTag(curTag);
    
    m_curIndex = targetIndex;
    
    indexArr->release();
    directionArr->release();
    
    saveData();
    
    // 调试时使用
    //    showMovesLabel();
    m_isReady = true;
}

void BBHandleLayer::saveData()
{
    // 存储数据
    UserDefault* ud = UserDefault::getInstance();
    ud->setBoolForKey("hasLocalData", true);
    
    Object *spt = NULL;
    CCARRAY_FOREACH(m_blockArr, spt)
    {
        auto blockSpt = (BBBlockSprite*)spt;
        
        __String *key = __String::createWithFormat("%d", blockSpt->getOrder());
        ud->setIntegerForKey(key->getCString(), blockSpt->getTag());
//        log("save data tag = %d", blockSpt->getTag());
    }
    ud->flush();
}

void BBHandleLayer::showMovesLabel()
{
    auto *idleBlockSpt = (BBBlockSprite*)m_blockArr->getObjectAtIndex(0);
    Point pos = idleBlockSpt->getPosition();
    idleBlockSpt->setPosition(Point(pos.x, pos.y + 500));
    idleBlockSpt->runAction(MoveTo::create(.2, pos));
    idleBlockSpt->runAction(FadeIn::create(.2));
    
    Object *spt = NULL;
    CCARRAY_FOREACH(m_blockArr, spt)
    {
        auto blockSpt = (BBBlockSprite*)spt;
        
        Point pos = blockSpt->getPosition();
        
        blockSpt->runAction(MoveTo::create(.2, Point(pos.x, pos.y - m_blockLength/2)));
    }
    
    m_relativeY = m_blockBgSpt->getPositionY()-m_blockBgSpt->getContentSize().height/2 + 5*BBGameDataManager::getInstance()->getScreenScale();
    
    m_movesLabel->setScale(15);
    m_movesLabel->setPosition(Point(m_relativeX + 1 * m_blockLength + (m_col - 1) * m_blockLength / 2, m_relativeY + m_row * m_blockLength + 60*BBGameDataManager::getInstance()->getScreenScale()));
    m_movesLabel->runAction(FadeIn::create(.2));
    m_movesLabel->runAction(ScaleTo::create(.2, 1.0f));
    m_blockBgSpt->runAction(Sequence::create(DelayTime::create(.15), FadeIn::create(0.1f), NULL));
}

int BBHandleLayer::computeIndex(float x, float y)
{
    
    if (x < m_relativeX || x > m_relativeX + m_blockLength * (m_col + 0) || y < m_relativeY || y > m_relativeY + m_blockLength * (m_row + 1)) {
        return -1;
    }
    
    float realX = x - m_relativeX;
    float realY = y - m_relativeY;
    
    int index = ceil(realX / m_blockLength) + (ceil(realY / m_blockLength) - 1) * (m_col + 0);
    if (index < 1 || index > (m_row + 1) * m_col - (m_col - 1)) {
        index = -1;
    }
    return index;
}

bool BBHandleLayer::checkIndex(int index)
{
    if (index < 1 && index > (m_row + 1) * m_col - (m_col - 1))
    {
        return false;
    }
    
    for (int i = 0; i < m_blockArr->count(); i++) {
        auto blockSpt = (Sprite*)m_blockArr->getObjectAtIndex(i);
        if (blockSpt->getTag() == index) {
            return true;
        }
    }
    
    return false;
}

bool BBHandleLayer::checkMove(Point pos)
{
    if (!m_isReady) {
        return false;
    }
    
    int index = computeIndex(pos.x, pos.y);
    log("%f-------%f-------%d", pos.x, pos.y, index);
    
    if (!checkIndex(index)) {
        return false;
    }
    
    m_testLayer->setPosition(Point(pos.x, pos.y));
    
    m_curIndex = index;
    
    return true;
}

int BBHandleLayer::getNearByIdleIndex(int index)
{
    int targetIndex = 0;
    BBBlockSprite *targetSpt;
    
    int idleOrder = m_row * m_col + 1;
    
    // up
    targetIndex = m_curIndex + m_col;
    if (checkIndex(targetIndex)) {
        targetSpt = (BBBlockSprite*)m_blockContainer->getChildByTag(targetIndex);
        if (targetSpt->getOrder() == idleOrder) {
            return targetIndex;
        }
    }
    // right
    targetIndex = m_curIndex + 1;
    if (checkIndex(targetIndex)) {
        if (targetIndex % m_col != 1) {
            targetSpt = (BBBlockSprite*)m_blockContainer->getChildByTag(targetIndex);
            if (targetSpt->getOrder() == idleOrder) {
                return targetIndex;
            }
        }
    }
    // down
    targetIndex = m_curIndex - m_col;
    if (checkIndex(targetIndex)) {
        targetSpt = (BBBlockSprite*)m_blockContainer->getChildByTag(targetIndex);
        if (targetSpt->getOrder() == idleOrder) {
            return targetIndex;
        }
    }
    // left
    targetIndex = m_curIndex - 1;
    if (checkIndex(targetIndex)) {
        if (targetIndex % m_col != 0) {
            targetSpt = (BBBlockSprite*)m_blockContainer->getChildByTag(targetIndex);
            if (targetSpt->getOrder() == idleOrder) {
                return targetIndex;
            }
        }
    }
    
    return 0;
}

bool BBHandleLayer::checkOver()
{
    Object *spt = NULL;
    CCARRAY_FOREACH(m_blockArr, spt)
    {
        auto blockSpt = (BBBlockSprite*)spt;
        int order = blockSpt->getOrder();
        
        if (order != blockSpt->getTag()) {
            return false;
        }
    }
    return true;
}

bool BBHandleLayer::onTouchBegan(Touch *touch, Event * event)
{
    if (m_showTips) {
        return true;
    }
    if (m_over) {
        return true;
    }
    m_firstPoint = touch->getLocation();
    if (checkMove(m_firstPoint)) {
        
        // 判断四个方向，哪个可以移动
        int targetIndex = getNearByIdleIndex(m_curIndex);
        
        if (targetIndex == 0) {
            return false;
        }
        
        auto curSpt = (BBBlockSprite*)m_blockContainer->getChildByTag(m_curIndex);
        auto targetSpt = (BBBlockSprite*)m_blockContainer->getChildByTag(targetIndex);
        log("exchange pos, targetIndex is %d", targetIndex);
        Point curPoint = curSpt->getPosition();
        int curTag = curSpt->getTag();
//        curSpt->setPosition(targetSpt->getPosition());
        curSpt->runAction(MoveTo::create(0.2, targetSpt->getPosition()));
        curSpt->setTag(targetSpt->getTag());
        targetSpt->setPosition(curPoint);
        targetSpt->setTag(curTag);
        
        m_moves++;
        m_movesLabel->setString(__String::createWithFormat("Moves:%d", m_moves)->getCString());
        
        UserDefault *ud = UserDefault::getInstance();
        __String *key1 = __String::createWithFormat("%d", curSpt->getOrder());
        ud->setIntegerForKey(key1->getCString(), curSpt->getTag());
        
        __String *key2 = __String::createWithFormat("%d", targetSpt->getOrder());
        ud->setIntegerForKey(key2->getCString(), targetSpt->getTag());
        
        ud->setIntegerForKey("moves", m_moves);
        ud->flush();
        
        return true;
        
    }
    return false;
}

void BBHandleLayer::onTouchEnded(Touch *touch, Event * event)
{
    if (m_showTips) {
        removeTipsLayer();
        return;
    }
    
    if (m_over) {
        return;
    }
    
    if (checkOver()) {
        // 拼图成功 弹出结算面板
        showOverLayer();
        return;
    }
    
//    Point pos = touch->getLocation();
//    
//    double angle = atan2(pos.x - m_firstPoint.x, pos.y - m_firstPoint.y) * 180 / 3.14;
//    
//    int targetIndex = 0;
//    log("%f", angle);
//    if (angle > -45 && angle < 45) {
//        log("up");
//        targetIndex = m_curIndex + m_col + 1;
//    } else if (angle > 45 && angle < 135) {
//        log("right");
//        targetIndex = m_curIndex + 1;
//        if (targetIndex % (m_col + 1) == 1) {
//            return;
//        }
//    } else if ((angle > 135 && angle < 180) || (angle > -180 && angle < -135)) {
//        log("down");
//        targetIndex = m_curIndex - m_col - 1;
//    } else if (angle > -135 && angle < -45) {
//        log("left");
//        targetIndex = m_curIndex - 1;
//        if (targetIndex % (m_col + 1) == 0) {
//            return;
//        }
//    }
//    
//    auto targetSpt = (BBBlockSprite*)getChildByTag(targetIndex);
//    auto curSpt = (BBBlockSprite*)getChildByTag(m_curIndex);
//    m_testLayer->setPosition(targetSpt->getPosition());
//    
//    int idleOrder = m_row * m_col + 1;
//    
//    log("targetIndex is %d, targetOrder is %d", targetIndex, targetSpt->getOrder());
//    
//    if (targetSpt->getOrder() != idleOrder) {
//        return;
//    }
//    
//    // 交换坐标
//    log("exchange pos");
//    Point curPoint = curSpt->getPosition();
//    int curTag = curSpt->getTag();
//    curSpt->setPosition(targetSpt->getPosition());
//    curSpt->setTag(targetSpt->getTag());
//    targetSpt->setPosition(curPoint);
//    targetSpt->setTag(curTag);
}
void BBHandleLayer::onTouchCancelled(Touch *touch, Event * event)
{
}
void BBHandleLayer::onTouchMoved(Touch *touch, Event * event)
{
}

void BBHandleLayer::showOverLayer()
{
    GameCenterApi::getInstance()->hiddenAdBanner();
    
    UserDefault *ud = UserDefault::getInstance();
    ud->setBoolForKey("hasLocalData", false);
    ud->flush();

    LayerColor *bgLayer = LayerColor::create(Color4B(0, 0, 0, 212), m_visibleSize.width, m_visibleSize.height);
    addChild(bgLayer);
    
    LabelTTF *titleLabel = LabelTTF::create("CONGRATULATION", "Arial", 60);
    titleLabel->setColor(Color3B(255, 255, 255));
    titleLabel->setPosition(Point(m_visibleSize.width/2 - 1000, m_visibleSize.height/2 + 100));
    bgLayer->addChild(titleLabel);

    LabelTTF *movesLabel = LabelTTF::create(__String::createWithFormat("moves:%d", m_moves)->getCString(), "Arial", 50);
    movesLabel->setColor(Color3B(255, 255, 255));
    movesLabel->setPosition(Point(m_visibleSize.width/2 + 1000, m_visibleSize.height/2 - 10));
    bgLayer->addChild(movesLabel);
    
    titleLabel->runAction(MoveTo::create(.35, Point(m_visibleSize.width/2, m_visibleSize.height/2 + 100)));
    movesLabel->runAction(MoveTo::create(.35, Point(m_visibleSize.width/2, m_visibleSize.height/2 - 10)));
    
    GameCenterApi::getInstance()->reportScore(m_moves, "1");
    
    m_over = true;
}