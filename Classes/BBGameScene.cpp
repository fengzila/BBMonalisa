//
//  BBGameScene.cpp
//  puzzle
//
//  Created by FengZi on 14-1-22.
//
//

#include "BBGameScene.h"
#include "BBHandleLayer.h"
#include "BBGameUILayer.h"

BBGameScene::BBGameScene()
{
    
}

BBGameScene::~BBGameScene()
{
    
}

Scene* BBGameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BBGameScene::create();
    scene->addChild(layer);
    return scene;
}

bool BBGameScene::init()
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
void BBGameScene::initData()
{
    // add your codes here...
    
}
#pragma mark - initSprite
void BBGameScene::initSprite()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto handleLayer = BBHandleLayer::create();
    addChild(handleLayer);
    
    auto uiLayer = BBGameUILayer::create();
    addChild(uiLayer);
    // add your codes here...
    
}