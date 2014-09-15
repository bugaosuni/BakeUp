#include "GameOverDialog.h"
#include "DataMgr.h"
#include "UtilHelper.h"

Scene* GameOverDialog::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOverDialog::create();
	scene->addChild(layer);
	return scene;
}

GameOverDialog::GameOverDialog()
{
	m_distance = 0;
	m_coins = 0;
	m_type = DRIVER_DOWN;
	m_canTouch = false;
}

GameOverDialog::~GameOverDialog()
{
}

bool GameOverDialog::init()
{
	if ( !Base::init() )
	{
		return false;
	}

	m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/GameOverDialog.ExportJson");
	this->addChild(m_view);

	initUI();
	
	return true;
}
void GameOverDialog::initUI()
{
	//root
	auto panelRoot = Helper::seekWidgetByName(m_view, "Panel_Root");
	panelRoot->setSize(getVisableSize());
	panelRoot->setPosition(VisibleRect::leftBottom());
	panelRoot->addTouchEventListener(this, toucheventselector(GameOverDialog::playCallback));

	if (UtilHelper::screenShoot())
	{
		scheduleOnce(schedule_selector(GameOverDialog::showScreenshot), 0.5f);
	}
	
	Action* action1 = Spawn::createWithTwoActions(RotateTo::create(0.5f, 15), FadeIn::create(0.5f));
	Action* action2 = Sequence::create(DelayTime::create(0.5f), 
		Spawn::createWithTwoActions(RotateTo::create(0.5f, -15), FadeIn::create(0.5f)), 
		CallFunc::create(
						// lambda
						[&](){
							m_canTouch = true;
					}  ), NULL);
	Action* action3 = RepeatForever::create(Sequence::createWithTwoActions(FadeOut::create(0.5f), FadeIn::create(0.5f)));
	
	//dist
	char ch[32];
	auto bitmapLabelDist = Helper::seekWidgetByName(m_view, "BitmapLabel_Dist");
	sprintf(ch, DataMgr::getInstance()->getTextData()["game_dist"].c_str(), m_distance);
	setLabelText(bitmapLabelDist, ch);
	bitmapLabelDist->runAction(action1);

	//coins
	auto bitmapLabelCoins = Helper::seekWidgetByName(m_view, "BitmapLabel_Coins");
	sprintf(ch, DataMgr::getInstance()->getTextData()["game_coins"].c_str(), m_coins);
	setLabelText(bitmapLabelCoins, ch);
	bitmapLabelCoins->runAction(action2);

	//Title
	auto bitmapLabelTitle = Helper::seekWidgetByName(m_view, "BitmapLabel_Title");
	setLabelText(bitmapLabelTitle, DataMgr::getInstance()->getTextData()[m_type.c_str()]);

	//Tips
	auto bitmapLabelTips = Helper::seekWidgetByName(m_view, "BitmapLabel_Tips");
	bitmapLabelTips->runAction(action3);
}

void GameOverDialog::showScreenshot(float dt)
{
	//screenshot
	auto panelScreenshot = Helper::seekWidgetByName(m_view, "Panel_Screenshot");
	Size size = panelScreenshot->getSize();
	string path = FileUtils::getInstance()->getWritablePath()+"screenshoot.png";
	Sprite* sp = Sprite::create(path);
	if (sp)
	{
		sp->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sp->setPosition(size.width / 2, size.height / 2);
		sp->setScale(0);
		panelScreenshot->addChild(sp);
		sp->runAction(Spawn::createWithTwoActions(RotateTo::create(0.5f, -1095), ScaleTo::create(0.5f, 0.4f)));
	}
}

void GameOverDialog::cantouch()
{
	m_canTouch = true;
}

void GameOverDialog::playCallback(Ref* sender,TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			if (m_canTouch)
			{
				if (m_callbackTarget && m_callbackFunc)
				{
					(m_callbackTarget->*m_callbackFunc)();
				}
				remove();
			}
		}
		break;
	default:
		break;
	}
}

void GameOverDialog::setDistance(int distance)
{
	m_distance = distance;
}

void GameOverDialog::setCoins(int coins)
{
	m_coins = coins;
}

void GameOverDialog::setOverType(string type)
{
	m_type = type;
}

void GameOverDialog::setCallbackFunc(Ref* sender, SEL_CallFunc callbackFunc)
{
	m_callbackTarget =  sender;
	m_callbackFunc = callbackFunc;
}