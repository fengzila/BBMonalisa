//
//  BBMenuLayer.h
//  puzzle
//
//  Created by FengZi on 14-4-8.
//
//

#ifndef __puzzle__BBMenuLayer__
#define __puzzle__BBMenuLayer__

#include "cocos2d.h"
USING_NS_CC;

class BBMenuLayer : public Layer
{
public:
    BBMenuLayer();
    ~BBMenuLayer();
    virtual bool init();
    CREATE_FUNC(BBMenuLayer);
    
public:
    virtual bool        onTouchBegan(Touch *touch, Event * event);
    virtual void        onTouchEnded(Touch *touch, Event * event);
    virtual void        onTouchCancelled(Touch *touch, Event * event);
    virtual void        onTouchMoved(Touch *touch, Event * event);

    
private:
    Size                m_visibleSize;
    Layer               *m_scrollViewContent;
    
    float               m_beginX;
    float               m_preX;
    int                 m_curPage;
    int                 m_totalPages;
    int                 m_picWidth;
    
private:
    void                initData();
    void                initSprite();
    void                menuBeginCallback(Object* pSender);
    void                setContentView();
    int                 validPageValue(int value);
    void                nextPage();
    void                prePage();
};

#endif /* defined(__puzzle__BBMenuLayer__) */
