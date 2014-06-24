//
//  BBGameScene.h
//  puzzle
//
//  Created by FengZi on 14-1-22.
//
//

#ifndef __puzzle__BBGameScene__
#define __puzzle__BBGameScene__

#include "cocos2d.h"
USING_NS_CC;

class BBGameScene : public Layer
{
public:
    BBGameScene();
    ~BBGameScene();
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(BBGameScene);
    
private:
    void initData();
    void initSprite();
};

#endif /* defined(__puzzle__BBGameScene__) */
