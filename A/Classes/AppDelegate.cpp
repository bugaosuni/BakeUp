#include "AppDelegate.h"
#include "SplashScene.h"
#include "MainScene.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "MobClickCpp.h"
#endif

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("5407f94efd98c5cd85005bc7", "google");
	umeng::MobClickCpp::setLogEnabled(false);
#endif
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLView::create("xxgame");
		director->setOpenGLView(glview);
		glview->setFrameSize(1280, 720);
	}

	auto designSize = Size(1024, 576);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	// a bug in DirectX 11 level9-x on the device prevents ResolutionPolicy::NO_BORDER from working correctly
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);
#else
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::NO_BORDER);
#endif

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = SplashScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    umeng::MobClickCpp::applicationDidEnterBackground();
#endif
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    umeng::MobClickCpp::applicationWillEnterForeground();
#endif
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
