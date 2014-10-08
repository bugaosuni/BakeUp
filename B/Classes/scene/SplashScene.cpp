#include "SplashScene.h"
#include "DataMgr.h"
#include "MainScene.h"
#include "AudioEnginMgr.h"
#include "AnimationManager.h"

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

    auto view = GUIReader::getInstance()->widgetFromJsonFile("UI/Splash.ExportJson");
    this->addChild(view);

    initUI();
    

    AnimationManager::getInstance()->addAllAnimationCache();
    return true;
}
void SplashScene::initUI()
{
    //init music
    AudioEnginMgr::initMusic();
    //load gamedata
    if (DataMgr::getInstance()->initGameData())
    {
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create(
            [&](){
                Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MainScene::createScene()));
        })));
    }
}