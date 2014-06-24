//
//  BBHandleLayer.h
//  puzzle
//
//  Created by FengZi on 14-1-22.
//
//

#ifndef __puzzle__BBHandleLayer__
#define __puzzle__BBHandleLayer__

#include "cocos2d.h"
USING_NS_CC;

class BBHandleLayer : public Layer
{
public:
    BBHandleLayer();
    ~BBHandleLayer();
    virtual bool init();
    CREATE_FUNC(BBHandleLayer);
    
public:
    virtual bool        onTouchBegan(Touch *touch, Event * event);
    virtual void        onTouchEnded(Touch *touch, Event * event);
    virtual void        onTouchCancelled(Touch *touch, Event * event);
    virtual void        onTouchMoved(Touch *touch, Event * event);
    
private:
    EventListenerTouchOneByOne* m_listener;
    Size                m_visibleSize;
    __Array             *m_blockArr;            // 拼图格子对象集合
    int                 m_row;                  // 行数
    int                 m_col;                  // 列数
    float               m_relativeX;            // 所有格子的相对原点坐标
    float               m_relativeY;            // 所有格子的相对原点坐标
    bool                m_isReady;              // 是否可以开始游戏
    int                 m_blockLength;
    Point               m_firstPoint;
    int                 m_curIndex;
    int                 m_countDown;
    LayerColor          *m_testLayer;
    Texture2D           *m_texture;
    bool                m_showTips;
    Layer               *m_blockContainer;
    int                 m_moves;
    LabelTTF            *m_movesLabel;
    Sprite              *m_blockBgSpt;
    bool                m_over;
    Layer               *m_tipsLayer;
    bool                m_blockAnimOver;
    int                 m_idleBlockLastDirection;
    
    void                initData();
    void                initSprite();
    int                 computeIndex(float x, float y);
    void                mixBlock();
    void                mixBlockOneByOne();
    int                 m_mixTimes;
    void                createHandleBg();
    void                createBlocks();
    void                createIdleBlock();
    void                beginCountDown();
    void                addEventListener();     // 添加触摸事件
    bool                checkMove(Point pos);   // 检测是否可以移动
    bool                checkIndex(int index);
    void                update(float dt);
    int                 getNearByIdleIndex(int index);
    void                showTips(Object* obj);
    void                removeTipsLayer();
    void                showMovesLabel();
    bool                checkOver();
    void                showOverLayer();
    void                saveData();
    void                blockDrop();
};

#endif /* defined(__puzzle__BBHandleLayer__) */
