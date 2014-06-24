//
//  GameCenterApi.mm
//  puzzle
//
//  Created by FengZi on 14-2-21.
//
//

#include "GameCenterApi.h"

#include <iostream>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "GameCenterManager.h"
#include "AdBannerManager.h"
#endif

static GameCenterApi *instance = NULL;

GameCenterApi::GameCenterApi()
{
    
}

GameCenterApi::~GameCenterApi()
{
    
}

GameCenterApi* GameCenterApi::getInstance()
{
    if (!instance) {
        instance = new GameCenterApi();
    }
    return instance;
}

void GameCenterApi::authenticateLocalUser()
{
    [[GameCenterManager getInstance] authenticateLocalUser];
}

void GameCenterApi::showLeaderboard()
{
    [[GameCenterManager getInstance] showLeaderboard];
}

void GameCenterApi::reportScore(int64_t score, const char* category)
{
    [[GameCenterManager getInstance] reportScore:score forCategory:[NSString stringWithUTF8String:category]];
}

void GameCenterApi::goToAppStoreEvaluate()
{
    [GameCenterManager goToAppStoreEvaluate];
}

void GameCenterApi::showAdBanner()
{
    [AdBannerManager show];
}

void GameCenterApi::hiddenAdBanner()
{
    [AdBannerManager hiden];
}