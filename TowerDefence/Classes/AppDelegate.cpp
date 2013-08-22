//
//  AppDelegate
//  TowerDefence
//
//  Created by Kirill Muzykov on 8/21/13.
//  Copyright PixelOxygen 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"
#include <string>

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

//Implementing support of multiple resolutions (as suggested in documentation)
typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];    
}Resource;

//List of supported resolutions
static Resource iphone  =  { cocos2d::CCSizeMake(480, 320),   "iphone" };
static Resource iphonehd  =  { cocos2d::CCSizeMake(960, 640),   "iphonehd" };

//We don't actually support iPhone5 (don't have images for it), but you can create such folder in resources and add required graphics.
//Everything should work ok.
static Resource iphone5  =  { cocos2d::CCSizeMake(1136, 640),   "iphone5" };

static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(480, 320);
static cocos2d::CCSize designResolutionSizeiPhone5 = cocos2d::CCSizeMake(568, 320);

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    //Setting default design resolution to iPhone3,4
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
    
    //Search paths (order of folders in which we'll search for resources)
    std::vector<std::string> searchPaths;

    CCSize frameSize = pEGLView->getFrameSize();
    if (frameSize.width > iphonehd.size.width)
    {
        //Overriding design resolution for iPhone5
        pEGLView->setDesignResolutionSize(designResolutionSizeiPhone5.width, designResolutionSizeiPhone5.height, kResolutionNoBorder);
        
        //First search in iphone5 folder, then iphonehd (since many resources can be reused)
        searchPaths.push_back("iphone5");
        searchPaths.push_back("iphonehd");
        
        //Setting content scale factor.        
        pDirector->setContentScaleFactor(iphone5.size.height / designResolutionSize.height);
    }
    else if (frameSize.width > iphone.size.width)
    {
        //Only adding iphonehd folder. See below for fall back.
        searchPaths.push_back("iphonehd");
        
        //Setting content scale factor.
        pDirector->setContentScaleFactor(iphonehd.size.height / designResolutionSize.height);
    }
    else
    {
        //This should be by default 1. But just in case.
        pDirector->setContentScaleFactor(iphone.size.height / designResolutionSize.height);
    }
    
    //Adding folders with resources common to all resolutions.
    searchPaths.push_back("iphone"); //In case iPhone 3Gs or we didn't have HD graphics so falling back to non-retina.
    searchPaths.push_back("common"); //Resources not dependant on resolution (.plist, music, sound & etc.)
    
    //Setting our search paths.
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);    

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = GameLayer::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
