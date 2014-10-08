#include "GameScene.h"
#include "GameMap.h"
#include "Hero.h"
#include "AnimationManager.h"
#include "Global.h"
#include "VisibleRect.h"

float GameScene::m_fMapMaxH;

Scene *GameScene::createScene()
{
    Scene *scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

GameScene::GameScene()
{
    m_pMainLayer = NULL;
    m_pGameMap = NULL;
    m_pHero = NULL;
    m_pLeftButton = NULL;
    m_pRightButton = NULL;
    m_pJumpButton = NULL;
    m_pShootButton = NULL;
    
    m_mapPosition = Point(0.0f, 0.0f);
    m_heroSize = Size(0.0f, 0.0f);
    m_mapSize = Size(0.0f, 0.0f);
    m_mapMoveAnchor = Point(0.0f, 0.0f);
    m_heroCurrentPos  = Point(0.0f, 0.0f);

    m_fMapMaxH = 0.0f;
    m_fMoveOffset = 0.0f;
    m_fJumpOffset = 0.0f;
    m_fCcMoveOffset = 2.0f;
    m_fCcJumpOffset = 0.3f;

    m_bIsLeftPress = false;
    m_bIsRightPress = false;
    m_bIsJumpPress = false;
    m_bIsFirePress = false;

    m_bIsSky = false;
    m_bJumpClick = false;
    m_bHeroFireable = true;

}

GameScene::~GameScene()
{
    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
}

bool GameScene::init()
{
    if ( !Base::init() )
    {
        return false;
    }

    m_pMainLayer = Layer::create();
    m_pMainLayer->retain();

    Size winsize = Director::getInstance()->getWinSize();
    m_mapMoveAnchor = Point(winsize.width / 2 - 80, winsize.height / 2);

    this->initHeroAndMap();
    this->initControlUI();

    //键盘事件
    auto keyboardListen = EventListenerKeyboard::create();
    keyboardListen->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    keyboardListen->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListen, this);

    this->scheduleUpdate();

    return true;
}

void GameScene::initControlUI()
{
    m_pLeftButton = Button::create("image/Button_Left.png", "image/Button_Left_p.png");
    m_pLeftButton->addTouchEventListener(this, toucheventselector(GameScene::leftCallback));
    m_pLeftButton->setPosition(Vec2(VisibleRect::left().x + 80, VisibleRect::bottom().y + 80));
    this->addChild(m_pLeftButton);

    m_pRightButton = Button::create("image/Button_Right.png", "image/Button_Right_p.png");
    m_pRightButton->addTouchEventListener(this, toucheventselector(GameScene::rightCallback));
    m_pRightButton->setPosition(Vec2(VisibleRect::left().x + m_pLeftButton->getContentSize().width*2 + 50,  VisibleRect::bottom().y + 80));
    this->addChild(m_pRightButton);

    m_pJumpButton = Button::create("image/Button_Jump.png", "image/Button_Jump_p.png");
    m_pJumpButton->addTouchEventListener(this, toucheventselector(GameScene::jumpCallback));
    m_pJumpButton->setPosition(Vec2(VisibleRect::right().x - 80,  VisibleRect::bottom().y + 80));
    this->addChild(m_pJumpButton);

    m_pShootButton = Button::create("image/Button_Shoot.png", "image/Button_Shoot_p.png");
    m_pShootButton->addTouchEventListener(this, toucheventselector(GameScene::shootCallback));
    m_pShootButton->setPosition(Vec2(VisibleRect::right().x - 80, VisibleRect::bottom().y + m_pJumpButton->getContentSize().width*2 + 50));
    this->addChild(m_pShootButton);
}

void GameScene::leftCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_BEGAN:
        m_bIsLeftPress = true;
        break;
    case TOUCH_EVENT_ENDED:
        m_bIsLeftPress = false;
        m_fMoveOffset = 0.0f;
        m_pHero->setHeroState(eNormalLeft);
        break;
    default:
        break;
    }
}

void GameScene::rightCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_BEGAN:
        m_bIsRightPress = true;
        break;
    case TOUCH_EVENT_ENDED:
        m_bIsRightPress = false;
        m_fMoveOffset = 0.0f;
        m_pHero->setHeroState(eNormalRight);
        break;
    default:
        break;
    }
}

void GameScene::jumpCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_BEGAN:
        if (!m_bIsSky)
        {
            if(m_bJumpClick)
            {
                m_bJumpClick = false;
                m_fJumpOffset = 10.0f;
                m_bIsJumpPress = true;
                log("==============doulb click");
            }
            else
            {
                scheduleOnce(schedule_selector(GameScene::singleJump), 0.25f);
                m_bJumpClick = true;
            }
            
        }
        break;
    case TOUCH_EVENT_ENDED:
        m_bIsJumpPress = false;
        break;
    default:
        break;
    }
}

void GameScene::shootCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_BEGAN:
        m_bIsFirePress = true;
        break;
    case TOUCH_EVENT_ENDED:
        m_bIsFirePress = false;
        break;
    default:
        break;
    }
}

void GameScene::singleJump(float dt)  
{  
    if (m_bJumpClick) {
        log("==============single click");
        m_bJumpClick = false;  
        m_fJumpOffset = 6.0f;
        m_bIsJumpPress = true;
    }  
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        m_bIsLeftPress = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        m_bIsRightPress = true;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        if (!m_bIsSky)
        {
            if(m_bJumpClick)
            {
                m_bJumpClick = false;
                m_fJumpOffset = 10.0f;
                m_bIsJumpPress = true;
            }
            else
            {
                scheduleOnce(schedule_selector(GameScene::singleJump), 0.25f);
                m_bJumpClick = true;
            }
        }
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_E:
        m_bIsFirePress = true;
        break;
    default:
        break;
    }
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode)
    {
    case cocos2d::EventKeyboard::KeyCode::KEY_A:
        m_bIsLeftPress = false;
        m_fMoveOffset = 0.0f;
        m_pHero->setHeroState(eNormalLeft);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_D:
        m_bIsRightPress = false;
        m_fMoveOffset = 0.0f;
        m_pHero->setHeroState(eNormalRight);
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_W:
        m_bIsJumpPress = false;
        break;
    case cocos2d::EventKeyboard::KeyCode::KEY_E:
        m_bIsFirePress = false;
        break;
    default:
        break;
    }
}

void GameScene::initHeroAndMap()
{
    char temp[30] = { 0 };
    sprintf(temp, "image/tmx/MarioMap%d.tmx", Global::getInstance()->getCurrentLevel());
    m_pGameMap = GameMap::create(temp);

    m_mapSize = Size(m_pGameMap->getMapSize().width * m_pGameMap->getTileSize().width,
        m_pGameMap->getMapSize().height * m_pGameMap->getTileSize().height);

    m_pGameMap->setPosition(Point(0, 0));
    m_pMainLayer->addChild(m_pGameMap);

    m_pHero = Hero::create();
    m_pHero->setBodyType(Global::getInstance()->currentHeroType);
    m_pHero->setAnchorPoint(Point(0.5f, 0.0f));
    m_pHero->setPosition(m_pGameMap->getHeroBirthPos());
    m_heroSize = m_pHero->getCurrentSize();
    m_pMainLayer->addChild(m_pHero);

    m_pMainLayer->setPosition(m_mapPosition);
    this->addChild(m_pMainLayer);
}

void GameScene::updateControl()
{
    if (!m_pHero->isHeroDied())
    {
        if (m_bIsLeftPress)
        {
            m_fMoveOffset = -m_fCcMoveOffset;
            m_pHero->setHeroState(eLeft);
        }
        else if (m_bIsRightPress)
        {
            m_fMoveOffset = m_fCcMoveOffset;
            m_pHero->setHeroState(eRight);
        }
        if (m_bIsJumpPress)
        {
            m_bIsJumpPress = false;
            if (!m_bIsSky)
            {
                m_bIsSky = true;
                m_pHero->m_bIsSky = true;
            }
        }
  
        if (m_bIsFirePress)
        {
            // 发射子弹
            if (m_pHero->isBulletable())
            {
                if (m_bHeroFireable)
                {
                    m_pGameMap->createNewBullet();
                    m_pHero->fireAction();
                    m_bHeroFireable = false;
                    DelayTime *pDelay = DelayTime::create(0.5f);
                    this->runAction(Sequence::create(pDelay,
                        CallFunc::create(this, callfunc_selector(GameScene::reSetHeroFireable)), NULL));

                }
            }
        }

    }

}

void GameScene::update(float dt)
{  

    // 是否死亡判断
    if (m_pHero->isHeroDied())
    {
        return;
    }

    this->updateControl();

    m_heroCurrentPos = m_pHero->getPosition();

    m_heroCurrentPos.x += m_fMoveOffset;
    m_heroCurrentPos.y += m_fJumpOffset;
    
    if (m_bIsSky)
    {
        switch (m_pHero->m_eFace)
        {
        case eLeft:
            if(m_fJumpOffset > 0)
            {
                m_pHero->setHeroState(eJumpLeft);
            }
            else
            {
                m_pHero->setHeroState(eDownLeft);
            }
            break;
        case eRight:
            if(m_fJumpOffset > 0)
            {
                m_pHero->setHeroState(eJumpRight);
            }
            else
            {
                m_pHero->setHeroState(eDownRight);
            }
            break;
        default:
            break;
        }
    }
    m_pHero->setPosition(m_heroCurrentPos);
    setSceneScrollPosition();
    collistionV();
    collistionH();
}

// 水平方向碰撞检测
// 悬浮在半空中大约一秒钟的bug应该是水平碰撞的问题，已解决
void GameScene::collistionH()
{
    m_heroCurrentPos = m_pHero->getPosition();
    // 做判断不让主角移除屏幕的左侧
    if ((m_heroCurrentPos.x - m_heroSize.width / 2 - m_fMapMaxH) <= 0)
    {
        Point pp = Point(m_fMapMaxH + m_heroSize.width / 2, m_heroCurrentPos.y);
        m_pHero->setPosition(pp);
        return;
    }

    // 右侧判断
    Point rightCollision = Point(m_heroCurrentPos.x + m_heroSize.width / 2, m_heroCurrentPos.y);
    Point rightTileCoord = m_pGameMap->positionToTileCoord(rightCollision);

    Point rightPos = m_pGameMap->tilecoordToPosition(rightTileCoord);
    rightPos = Point(rightPos.x - m_heroSize.width / 2, m_heroCurrentPos.y);
    TileType tileType = m_pGameMap->tileTypeforPos(rightTileCoord);
    switch (tileType)
    {
    case eTile_Land:
        m_pHero->setPosition(rightPos);
        break;
    case eTile_Coin:
        m_pGameMap->getCoinLayer()->removeTileAt(rightTileCoord);
        break;
    default:
        break;
    }

    // 主角的左侧碰撞检测点
    Point leftCollision = Point(m_heroCurrentPos.x - m_heroSize.width / 2, m_heroCurrentPos.y);
    Point leftTileCoord = m_pGameMap->positionToTileCoord(leftCollision);

    Point leftPos = m_pGameMap->tilecoordToPosition(leftTileCoord);
    leftPos = Point(leftPos.x + m_heroSize.width / 2 + m_pGameMap->getTileSize().width, m_heroCurrentPos.y);
    tileType = m_pGameMap->tileTypeforPos(leftTileCoord);
    switch (tileType)
    {
    case eTile_Land:
        m_pHero->setPosition(leftPos);
        break;
    case eTile_Coin:
        m_pGameMap->getCoinLayer()->removeTileAt(leftTileCoord);
        break;
    default:
        break;
    }
}

// 垂直方向碰撞检测
void GameScene::collistionV()
{
    Point currentPos = m_pHero->getPosition();

    // 下面是判断马里奥的脚底是否已经接近地图下边缘的5像素处，以此为die
    if (currentPos.y <= 0)
    {
        m_pHero->setHeroDie(true);
        m_pHero->setPosition(Point(currentPos.x, 1));
        m_pHero->dieForTrap();  // 执行一段动画，表示因掉入陷阱而死
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("DiaoRuXianJingSi.ogg");
        return;
    }

    // 下面是判断主角是否头顶越过游戏画面最上层
    if (currentPos.y > m_mapSize.height - m_heroSize.height - 2)
    {
        m_fJumpOffset = 0.0f;
        m_pHero->setPosition(Point(currentPos.x, m_mapSize.height - m_heroSize.height - 2));
        m_bIsSky = false;
        return;
    }

    // 主角头顶的碰撞检测
    for (int heroIdx = 6; heroIdx <= m_heroSize.width - 6; ++heroIdx)
    {
        Point upCollision = Point(currentPos.x - m_heroSize.width / 2 + heroIdx, currentPos.y + m_heroSize.height - 40);
        Point upTileCoord = m_pGameMap->positionToTileCoord(upCollision);
 
        Point upPos = m_pGameMap->tilecoordToPosition(upTileCoord);
        upPos = Point(currentPos.x, upPos.y - m_heroSize.height);
        TileType tileType = m_pGameMap->tileTypeforPos(upTileCoord);
        bool flagUp = false;
        switch (tileType)
        {
        case eTile_Land:
            if (m_fJumpOffset > 0)
            {
                if(m_pGameMap->crossedHeadBlock(upTileCoord, m_pHero->getCurrentBodyType()))
                {
                    return;
                }
                m_fJumpOffset = 0.0f;
                m_pHero->setPosition(upPos);
                flagUp = true;
            }
            break;
        case eTile_Coin:
            m_pGameMap->getCoinLayer()->removeTileAt(upTileCoord);
            break;
        default:
            break;
        }
        if (flagUp)
        {
            m_fJumpOffset -= m_fCcJumpOffset;
            return;
        }
    }

    float heroLeftSide = currentPos.x - m_heroSize.width / 2;

    for (int heroIdx = 6; heroIdx <= m_heroSize.width - 6; ++heroIdx)
    {
        Point downCollision = Point(heroLeftSide + heroIdx, currentPos.y);
        Point downTileCoord = m_pGameMap->positionToTileCoord(downCollision);

        downTileCoord.y += 1;
        Point downPos = m_pGameMap->tilecoordToPosition(downTileCoord);
        downPos = Point(currentPos.x, downPos.y + m_pGameMap->getTileSize().height);

        TileType tileType = m_pGameMap->tileTypeforPos(downTileCoord);
        bool flagDown = false;
        switch (tileType)
        {
        case eTile_Land:
        {
            if (m_fJumpOffset < 0)
            {
                m_fJumpOffset = 0.0f;
                m_pHero->setPosition(downPos);
                m_bIsSky = false;
                m_pHero->m_bIsSky = false;
                switch (m_pHero->m_eFace)
                {
                case eLeft:
                    if (m_bIsLeftPress)
                    {
                        m_pHero->setHeroState(eLeft);
                    }
                    else
                    {
                        m_pHero->setHeroState(eNormalLeft);
                    }
                    break;
                case eRight:
                    if (m_bIsRightPress)
                    {
                        m_pHero->setHeroState(eRight);
                    }
                    else
                    {
                        m_pHero->setHeroState(eNormalRight);
                    }
                    break;
                default:
                    break;
                }
            }
            flagDown = true;
        }
            break;
        case eTile_Coin:
            //m_pGameMap->getCoinLayer()->removeTileAt(downTileCoord);
            break;
        default:
            break;
        }
        if (flagDown)
        {
            return;
        }

    }

    m_fJumpOffset -= m_fCcJumpOffset;
}

void GameScene::setSceneScrollPosition()
{
    Point pos = m_pHero->getPosition();
    Size winsize = Director::getInstance()->getWinSize();

    float x = MAX(pos.x, m_mapMoveAnchor.x);
    float y = MAX(pos.y, m_mapMoveAnchor.y);

    x = MIN(x, m_mapSize.width - winsize.width / 2 - 80);
    y = MIN(y, m_mapSize.height - winsize.height / 2);
    Point actualPosition = Point(x, y);

    Point viewPoint = ccpSub(m_mapMoveAnchor, actualPosition);

    // 使得地图不能倒退回去
    if (fabsf(viewPoint.x) <= m_fMapMaxH)
    {
        return;
    }
    else
    {
        viewPoint.y = 0;
        m_pMainLayer->setPosition(viewPoint);
        x = m_pMainLayer->getPosition().x;
        m_fMapMaxH = fabsf(x);
    }
}

float GameScene::getMapMaxH()
{
    return m_fMapMaxH;
}

void GameScene::reSetHeroFireable()
{
    m_bHeroFireable = true;
}






