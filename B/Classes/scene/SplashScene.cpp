#include "SplashScene.h"
#include "DataMgr.h"
#include "MainScene.h"
#include "UtilHelper.h"

Scene* SplashScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SplashScene::create();
	scene->addChild(layer);
	return scene;
}

SplashScene::SplashScene()
{
}

SplashScene::~SplashScene()
{
}

bool SplashScene::init()
{
	if ( !Layer::init() )
	{
		return false;
	}

	auto view = GUIReader::getInstance()->widgetFromJsonFile("UI/SplashScene.ExportJson");
	this->addChild(view);

	initUI();
	
	return true;
}
void SplashScene::initUI()
{
	if (DataMgr::getInstance()->initGameData())
	{
		auto action = Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create(this, callfunc_selector(SplashScene::gotoGame)));
		this->runAction(action);
	}
}

void SplashScene::gotoGame()
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MainScene::createScene()));
}