#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocostudio;

class SplashScene: public Layer
{
public:
	SplashScene();
	virtual ~SplashScene();
    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(SplashScene);

	void initUI();
	void gotoGame();
};

#endif // __UPGRADECAR_SCENE_H__
