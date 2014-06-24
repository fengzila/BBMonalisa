#include "AppDelegate.h"
#include "BBWelcomeScene.h"
#include "GameCenterApi.h"
#include "BBGameDataManager.h"
#include "MobClickCpp.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto eglView = EGLView::getInstance();
    director->setOpenGLView(eglView);
    
    auto frameSize = eglView->getFrameSize();
    std::vector<std::string> searchPaths;
    if ((int)frameSize.width == 1536) {
        log("hdr");
        searchPaths.push_back("hdr");
        BBGameDataManager::getInstance()->setScreenScale(2);
    }else{
        log("hd");
        searchPaths.push_back("hd");
    }
    
    FileUtils::getInstance()->setSearchPaths(searchPaths);
	
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = BBWelcomeScene::createScene();

    // run
    director->runWithScene(scene);
    
    GameCenterApi::getInstance()->authenticateLocalUser();
    
    // Umeng
    MobClickCpp::startWithAppkey("530fcefb56240b7110087b49");

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    MobClickCpp::applicationDidEnterBackground();
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
    MobClickCpp::applicationWillEnterForeground();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
