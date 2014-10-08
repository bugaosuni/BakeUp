#include "MainScene.h"
#include "WorldScene.h"
#include "DataMgr.h"

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

MainScene::MainScene()
{
	m_buttonMusic = nullptr;
	m_buttonSound = nullptr;
	m_isPause = false;
}

MainScene::~MainScene()
{
}

bool MainScene::init()
{
	if ( !Base::init() )
	{
		return false;
	}

	m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/Menu.ExportJson");
	this->addChild(m_view);

	initUI();

	m_listener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this); 
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener, this);
	
	if (DataMgr::bIsSpalshEnter)
	{
		// 进入游戏的广告
		//UtilHelper::showStartAppAd(1);
		DataMgr::bIsSpalshEnter = false;
	}

	return true;
}
void MainScene::initUI()
{
	if (!AudioEnginMgr::isMusicPlaying())
	{
		AudioEnginMgr::playBgMusic(MAIN_MENU);
	}

	auto panelRoot = Helper::seekWidgetByName(m_view, "Panel_Root");
	panelRoot->setSize(getVisableSize());
	panelRoot->setPosition(VisibleRect::leftBottom());
	Size size = panelRoot->getSize();
	
	auto actionTitle = Sequence::createWithTwoActions(DelayTime::create(0.5f),
		EaseElasticInOut::create(MoveTo::create(1.0f, Vec2(size.width * 0.5f, size.height * 0.88f))));
	auto actionMtMid = Sequence::createWithTwoActions(DelayTime::create(0.5f),
		EaseExponentialInOut::create(MoveTo::create(1.1f, Vec2(size.width * 0.5f, 0))));
	auto actionMtLeft = Sequence::createWithTwoActions(DelayTime::create(0.5f),
		MoveTo::create(0.3f, Vec2(0, 0)));
	auto actionMtRight = Sequence::createWithTwoActions(DelayTime::create(0.5f),
		MoveTo::create(0.3f, Vec2(size.width, 0)));
	auto actionSound = Sequence::createWithTwoActions(DelayTime::create(0.7f),
		EaseBackOut::create(ScaleTo::create(0.3f, 1.0f)));
	auto actionMusic = Sequence::createWithTwoActions(DelayTime::create(0.8f),
		EaseBackOut::create(ScaleTo::create(0.3f, 1.0f)));
	auto actionRate = Sequence::createWithTwoActions(DelayTime::create(0.9f),
		EaseBackOut::create(ScaleTo::create(0.3f, 1.0f)));
	auto actionPlay = Sequence::createWithTwoActions(DelayTime::create(1.0f),
		EaseBackOut::create(ScaleTo::create(0.3f, 1.0f)));

	//play
	auto buttonPlay = Helper::seekWidgetByName(m_view, "Button_Play");
	buttonPlay->addTouchEventListener(this, toucheventselector(MainScene::playCallBack));

	//rate
	auto buttonRate = Helper::seekWidgetByName(m_view, "Button_Rate");
	buttonRate->addTouchEventListener(this, toucheventselector(MainScene::rateCallBack));

	//music
	m_buttonMusic = dynamic_cast<Button*>(Helper::seekWidgetByName(m_view, "Button_Music"));
	m_buttonMusic->addTouchEventListener(this, toucheventselector(MainScene::musicCallBack));
	if (UtilHelper::getFromBool(MUSIC_OFF))
	{
		m_buttonMusic->loadTextureNormal("menu_btn_music_off.png", TextureResType::PLIST);
	}

	//sound
	m_buttonSound = dynamic_cast<Button*>(Helper::seekWidgetByName(m_view, "Button_Sound"));
	m_buttonSound->addTouchEventListener(this, toucheventselector(MainScene::soundCallBack));
	if (UtilHelper::getFromBool(SOUND_OFF))
	{
		m_buttonSound->loadTextureNormal("menu_btn_sound_off.png", TextureResType::PLIST);
	}

	auto imageTitle = Helper::seekWidgetByName(m_view, "Image_Title");
	auto imageMtMid = Helper::seekWidgetByName(m_view, "Image_Mt_Mid");
	auto imageMtLeft = Helper::seekWidgetByName(m_view, "Image_Mt_Left");
	auto imageMtRight = Helper::seekWidgetByName(m_view, "Image_Mt_Right");

	buttonPlay->runAction(actionPlay);
	buttonRate->runAction(actionRate);
	m_buttonMusic->runAction(actionMusic);
	m_buttonSound->runAction(actionSound);
	imageTitle->runAction(actionTitle);
	imageMtMid->runAction(actionMtMid);
	imageMtLeft->runAction(actionMtLeft);
	imageMtRight->runAction(actionMtRight);

	addOwnAds();
}

void MainScene::playCallBack(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		AudioEnginMgr::playEffect(BUTTON);
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, WorldScene::createScene()));
		break;
	default:
		break;
	}
}

void MainScene::rateCallBack(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		AudioEnginMgr::playEffect(BUTTON);
		//Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectMapScene::createScene()));
		break;
	default:
		break;
	}
}

void MainScene::musicCallBack(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			if (UtilHelper::getFromBool(MUSIC_OFF))
			{
				UtilHelper::writeToBool(MUSIC_OFF, false);
				m_buttonMusic->loadTextureNormal("menu_btn_music_on.png", TextureResType::PLIST);
				if (m_isPause)
				{
					AudioEnginMgr::resumeBgMusic();
				}
				else
				{
					AudioEnginMgr::playBgMusic(MAIN_MENU);
				}
			}
			else
			{
				UtilHelper::writeToBool(MUSIC_OFF, true);
				m_buttonMusic->loadTextureNormal("menu_btn_music_off.png", TextureResType::PLIST);
				m_isPause = true;
				AudioEnginMgr::pauseBgMusic();
			}
		}
		break;
	default:
		break;
	}
}

void MainScene::soundCallBack(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			if (UtilHelper::getFromBool(SOUND_OFF))
			{
				UtilHelper::writeToBool(SOUND_OFF, false);
				m_buttonSound->loadTextureNormal("menu_btn_sound_on.png", TextureResType::PLIST);
			}
			else
			{
				UtilHelper::writeToBool(SOUND_OFF, true);
				m_buttonSound->loadTextureNormal("menu_btn_sound_off.png", TextureResType::PLIST);
			}
		}
		break;
	default:
		break;
	}
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keycode,Event * pEvent) 
{ 
	/*
	if(EventKeyboard::KeyCode::KEY_RETURN == keycode || EventKeyboard::KeyCode::KEY_ESCAPE == keycode || EventKeyboard::KeyCode::KEY_BACKSPACE == keycode)
	{
		if (!DataMgr::modalShow)
		{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			umeng::MobClickCpp::event("user_exit");
#endif
			UtilHelper::showStartAppAd(2);
		}
	}*/
}