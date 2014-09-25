#include "GameScene.h"
#include "SplashScene.h"
#include "PauseDialog.h"
#include "UpgradeCarScene.h"
#include "GameOverDialog.h"
#include "SelectMapScene.h"

using namespace CocosDenshion;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "MobClickCpp.h"
#endif

GameScene::GameScene()
{
    
    m_nTotalCoins = 0;
    m_nBestDistance =0 ;
    m_nCurrentDistance = 0;
    m_nCurrentCoins = 0;
    m_nGas = 100;
    m_nFourWheelDriveLevel = 0;
    m_nEngineLevel = 0;
    m_nSuspensionLevel = 0;
    m_nWheellLevel = 0;
    m_nMapId = 0;
    m_nCarId = 0;

    m_fPressPedalTime = 0.0f;
    m_fRearWheelSpeed = 0.0f;
    m_fTestMapOffset = 0.0f;

    m_bIsGameOver = false;
    m_bIsTips = false;

    m_overtype = DRIVER_DOWN;

    m_pGasProgessBar = NULL;
    m_pSpeedNeedle = NULL;
    m_pTerrainView = NULL;
    m_pBoostNeedle = NULL;
    m_pDistanceLable = NULL;
    m_pBestDistanceLable = NULL;
    m_pCoinLable = NULL;
    m_pFuelLowSprite = NULL;

    m_pCarBody = NULL;
    m_pRearWheelBody = NULL;
    m_pFrontWheelBody = NULL;
    m_pRearWheelJoint = NULL;
    m_pFrontWheelJoint = NULL;
    m_pBox2dWorld = NULL;
    m_pBox2dDebugDraw = NULL;
    m_pCarShapeCache = NULL;
    m_pBox2dContactListener = NULL;
}

GameScene::~GameScene()
{
    //CC_SAFE_DELETE(m_pBox2dWorld);
   // delete m_pBox2dDebugDraw;
}

Scene* GameScene::createScene()
{
    auto scene = Scene::create();

    auto layer = GameScene::create();

    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if ( !Base::init() )
    {
        return false;
    }
    
   // this->setScale(1);
    setupWorld();
    setupDebugDraw();
    initShape();
    initUI();
    initMap();
    initCar();
    setupStarAndEndPoint();
    scheduleUpdate();

    m_listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this); 
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_listener, this); 

    return true;
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    if(EventKeyboard::KeyCode::KEY_RETURN == keycode || EventKeyboard::KeyCode::KEY_ESCAPE == keycode || EventKeyboard::KeyCode::KEY_BACKSPACE == keycode)
    {
        if (!DataMgr::bIsPause && !DataMgr::modalShow)
        {
            DataMgr::modalShow = true;
            DataMgr::bIsPause = true;
            PauseDialog* dlg = new PauseDialog();
            dlg->setResumeCallbackFunc(this, callfunc_selector(GameScene::resume));
            dlg->setRestartCallbackFunc(this, callfunc_selector(GameScene::restart));
            dlg->setBackCallbackFunc(this, callfunc_selector(GameScene::back));
            dlg->init();
            this->addChild(dlg, 999);
            dlg->release();
        }
    }
}

void GameScene::setupWorld()
{
    b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
    m_pBox2dWorld = new b2World(gravity);
    bool doSleep = true;
    m_pBox2dWorld->SetAllowSleeping(doSleep);    
    m_pBox2dWorld->SetContinuousPhysics(true);
    
    m_pBox2dContactListener = new ContactListener();
    m_pBox2dWorld->SetContactListener(m_pBox2dContactListener);

}

void GameScene::setupDebugDraw()
{
     m_pBox2dDebugDraw = new GLESDebugDraw( PTM_RATIO );
     m_pBox2dWorld->SetDebugDraw(m_pBox2dDebugDraw);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
            flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    m_pBox2dDebugDraw->SetFlags(flags);
}

void GameScene::setupStarAndEndPoint()
{
    //地面空间刚体定义  
    b2BodyDef groupBodyDef;  
    groupBodyDef.position.Set(0.0f, 0.0f);  
    //根据刚体定义生成刚体  
    b2Body* groundBody = m_pBox2dWorld->CreateBody(&groupBodyDef);  
    //生成边缘型形状  
    b2EdgeShape groundBox;  
    //关联定义  
    b2FixtureDef boxShapeDef;  
    boxShapeDef.shape = &groundBox;  

    //起点  
    groundBox.Set(b2Vec2(10.0f, 0.0f), b2Vec2(10.0f, VisibleRect::top().y / PTM_RATIO));  
    //创建关联  
    groundBody->CreateFixture(&boxShapeDef);  

    //终点  
    groundBox.Set(b2Vec2(1020.0f, VisibleRect::top().y / PTM_RATIO), b2Vec2(1020.0f , 0.0f));  
    //创建关联  
    groundBody->CreateFixture(&boxShapeDef);  
}

void GameScene::initShape()
{
    GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("box2d/box2d.plist");
    m_pCarShapeCache = GB2ShapeCache::sharedGB2ShapeCache();
}

void GameScene::initUI()
{
    m_nMapId = DataMgr::mapIndex + 1;
    m_view = GUIReader::getInstance()->widgetFromJsonFile("UI/GameScene.ExportJson");
    m_view->setSize(getVisableSize());
    m_view->setPosition(VisibleRect::leftBottom());
    
    this->addChild(m_view);

    ImageView* gameBg = dynamic_cast<ImageView*>( Helper::seekWidgetByName(m_view, "Image_Bg"));
    gameBg->loadTexture("Terrain/map_bg" + String::createWithFormat("%d",m_nMapId)->_string + ".png");
    

    m_nTotalCoins = UtilHelper::getFromInteger(USER_GOLD);

    m_nBestDistance = DataMgr::getInstance()->getMapData()[DataMgr::mapIndex].bestDist;


    m_pCoinLable = Helper::seekWidgetByName(m_view, "BitmapLabel_Gold");
    setLabelText(m_pCoinLable, m_nTotalCoins);
    
    m_pDistanceLable = Helper::seekWidgetByName(m_view, "BitmapLabel_Dist");
    setLabelText(m_pDistanceLable, "0m/1000m");

    m_pBestDistanceLable = Helper::seekWidgetByName(m_view, "BitmapLabel_Best");
    setLabelText(m_pBestDistanceLable, String::createWithFormat("%d",m_nBestDistance)->_string + "m");


    //settings
    auto buttonPause = Helper::seekWidgetByName(m_view, "Button_Pause");
    buttonPause->addTouchEventListener(this, toucheventselector(GameScene::pauseCallback));

    auto break_view = Helper::seekWidgetByName(m_view, "Button_Break");
    break_view->addTouchEventListener(this, toucheventselector(GameScene::brakeCallback));

    auto gas_view = Helper::seekWidgetByName(m_view, "Button_Gas");
    gas_view->addTouchEventListener(this, toucheventselector(GameScene::gasCallback));

    m_pSpeedNeedle = Helper::seekWidgetByName(m_view, "Image_Needle_S");
    m_pSpeedNeedle->setRotation(-90);

    m_pBoostNeedle = Helper::seekWidgetByName(m_view, "Image_Needle_B");
    m_pBoostNeedle->setRotation(-90);

    m_pGasProgessBar = (LoadingBar*)Helper::seekWidgetByName(m_view, "ProgressBar_Oil");

    m_pFuelLowSprite = Helper::seekWidgetByName(m_view, "Image_Fuel_Low");

    // 固定时间减少油，不能放在update里面
    schedule( schedule_selector(GameScene::consumGas), 1.0f); 
}

void GameScene::initMap()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    umeng::MobClickCpp::startLevel(String::createWithFormat("%d",m_nMapId)->_string.c_str());
#endif    
    m_pTerrainView = TerrainView::createWithWorld(m_pBox2dWorld);
    m_view->addChild(m_pTerrainView , 1);
    
    //GL_CLAMP_TO_BORDER
    Texture2D::TexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, 0x812D}; //缩小 放大  水平 T垂直

    auto surface = Sprite::create("Terrain/surface"+  String::createWithFormat("%d",m_nMapId)->_string + ".png");
    surface->setTag(SPRITE_SURFACE);
    surface->getTexture()->setTexParameters(params);
    m_pTerrainView->setSurface(surface);

    auto ground = Sprite::create("Terrain/ground" +  String::createWithFormat("%d",m_nMapId)->_string + ".png");;
    ground->setTag(SPRITE_GROUND);
    ground->getTexture()->setTexParameters(params);
    m_pTerrainView->setGround(ground);
}

void GameScene::initCar()
{
    m_nCarId = DataMgr::carIndex + 1;

    m_nEngineLevel = DataMgr::getInstance()->getCarData()[m_nCarId-1].part1_level;
    m_nSuspensionLevel = DataMgr::getInstance()->getCarData()[m_nCarId-1].part2_level;
    m_nWheellLevel = DataMgr::getInstance()->getCarData()[m_nCarId-1].part3_level;
    m_nFourWheelDriveLevel = DataMgr::getInstance()->getCarData()[m_nCarId-1].part4_level;

    CarInfo carInfo= DataMgr::getInstance()->getCarInfoData()[m_nCarId - 1];
    
    Vec2 p = Vec2(VisibleRect::left().x + 400, VisibleRect::center().y);

    auto car_sprite = Sprite::create("box2d/car"+ String::createWithFormat("%d",m_nCarId)->_string + ".png");

    car_sprite->setTag(SPRITE_CAR);
    car_sprite->setPosition(p);
    m_pTerrainView->addChild(car_sprite);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.linearDamping = carInfo.linearDamping;
    bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
    bodyDef.userData = car_sprite;
    m_pCarBody = m_pBox2dWorld->CreateBody(&bodyDef);

    m_pCarShapeCache->addFixturesToBody(m_pCarBody, "car" + String::createWithFormat("%d",m_nCarId)->_string); 
    car_sprite->setAnchorPoint(m_pCarShapeCache->anchorPointForShape("car" + String::createWithFormat("%d",m_nCarId)->_string));

    auto wheel_sprite1 = Sprite::create("box2d/wheel" + String::createWithFormat("%d",m_nCarId)->_string + ".png");
    wheel_sprite1->setTag(SPRITE_WHEEL);
    wheel_sprite1->setPosition(p.x + carInfo.wheel1_x, p.y + carInfo.wheel1_y);
    m_pTerrainView->addChild(wheel_sprite1);


    b2CircleShape circle;
    circle.m_radius = wheel_sprite1->getContentSize().width/PTM_RATIO/2;

    b2FixtureDef fd;
    fd.shape = &circle;
    fd.density = DataMgr::getInstance()->getWheelDensity(m_nCarId, m_nSuspensionLevel);
   //fd.density = carInfo.wheel_density;
    fd.friction = DataMgr::getInstance()->getWheelFriction(m_nCarId, m_nWheellLevel);
    fd.restitution = carInfo.wheel_restitution;

    bodyDef.position.Set((p.x + carInfo.wheel1_x)/ PTM_RATIO, (p.y + carInfo.wheel1_y)/PTM_RATIO);
    bodyDef.userData = wheel_sprite1;
    m_pRearWheelBody = m_pBox2dWorld->CreateBody(&bodyDef);
    m_pRearWheelBody->CreateFixture(&fd);

    b2WheelJointDef wheel_jd;
    b2Vec2 axis(0.0f, 1.0f);

    wheel_jd.Initialize(m_pCarBody, m_pRearWheelBody, m_pRearWheelBody->GetPosition(), axis);
    wheel_jd.motorSpeed = carInfo.wheel1_motorSpeed;
    wheel_jd.maxMotorTorque = DataMgr::getInstance()->getRearSpeed(m_nCarId, m_nEngineLevel); //最大马达扭矩
    //wheel_jd.enableMotor = carInfo.wheel1_enableMotor; //马达开关
    wheel_jd.frequencyHz = carInfo.wheel1_frequencyHz; // 频率 f ＝ 1/T
    wheel_jd.dampingRatio = carInfo.wheel1_dampingRatio; // 阻尼系数 
    m_pRearWheelJoint = (b2WheelJoint*)m_pBox2dWorld->CreateJoint(&wheel_jd);

    if (m_nCarId != 30 )
    {
        auto wheel_sprite2 = Sprite::create("box2d/wheel" + String::createWithFormat("%d",m_nCarId)->_string + ".png");
        wheel_sprite2->setTag(SPRITE_WHEEL);
        wheel_sprite2->setPosition(p.x + carInfo.wheel2_x, p.y + carInfo.wheel2_y);
        m_pTerrainView->addChild(wheel_sprite2);

        bodyDef.position.Set((p.x + carInfo.wheel2_x)/ PTM_RATIO, (p.y + carInfo.wheel2_y)/PTM_RATIO);
        bodyDef.userData = wheel_sprite2;
        m_pFrontWheelBody = m_pBox2dWorld->CreateBody(&bodyDef);
        m_pFrontWheelBody->CreateFixture(&fd);

        wheel_jd.Initialize(m_pCarBody, m_pFrontWheelBody, m_pFrontWheelBody->GetPosition(), axis);
        wheel_jd.motorSpeed = carInfo.wheel2_motorSpeed;
        wheel_jd.maxMotorTorque = DataMgr::getInstance()->getRearSpeed(m_nCarId, m_nFourWheelDriveLevel);
        wheel_jd.frequencyHz = carInfo.wheel2_frequencyHz;
        wheel_jd.dampingRatio = carInfo.wheel2_dampingRatio;
        m_pFrontWheelJoint = (b2WheelJoint*)m_pBox2dWorld->CreateJoint(&wheel_jd);
    }

    if (m_nCarId == 30)
    {
        b2PolygonShape antenna_shape;
        antenna_shape.SetAsBox(0.125f, 0.25f);

        b2FixtureDef antenna_fd;
        antenna_fd.shape = &antenna_shape;
        antenna_fd.density =0.1f;

        b2WeldJointDef antenna_jd;
        antenna_jd.frequencyHz = 15.0f;
        antenna_jd.dampingRatio = 0.7f;
        b2Body* prevBody = m_pCarBody;
        for (int32 i = 0; i < 3; ++i)
        {
            auto antenna_sprite = Sprite::create("box2d/antenna.png");
            antenna_sprite->setTag(SPRITE_ANTENNA);
            antenna_sprite->setPosition(p.x + carInfo.antenna_x, p.y + 15.0f * i + carInfo.antenna_y);
            m_pTerrainView->addChild(antenna_sprite, -1);
            b2BodyDef antenna_bd;
            antenna_bd.type = b2_dynamicBody;
            antenna_bd.userData = antenna_sprite;
            antenna_bd.position.Set((p.x + carInfo.antenna_x)/PTM_RATIO, (p.y + 15.0f * i + carInfo.antenna_y)/PTM_RATIO);
            b2Body* antenna_body = m_pBox2dWorld->CreateBody(&antenna_bd);
            antenna_body->CreateFixture(&antenna_fd);

            b2Vec2 antenna_anchor((p.x + carInfo.antenna_x)/PTM_RATIO, (p.y + 15.0f * i - 7.5 + carInfo.antenna_y)/PTM_RATIO);
            antenna_jd.Initialize(prevBody, antenna_body, antenna_anchor);
            m_pBox2dWorld->CreateJoint(&antenna_jd);

            prevBody = antenna_body;
        }
    }

    b2WeldJointDef weld_jd;
    weld_jd.frequencyHz = 8.0f;
    weld_jd.dampingRatio = 0.7f;

    auto driver_body_sprite = Sprite::create("box2d/driver-body.png");
    driver_body_sprite->setTag(SPRITE_DRIVER_BODY);
    driver_body_sprite->setPosition(p.x + carInfo.driver_body_x, p.y + carInfo.driver_body_y);
    m_pTerrainView->addChild(driver_body_sprite, -1);

    b2BodyDef driver_body_bd;
    driver_body_bd.type = b2_dynamicBody;
    driver_body_bd.position.Set((p.x + carInfo.driver_body_x)/PTM_RATIO, (p.y + carInfo.driver_body_y )/PTM_RATIO);
    driver_body_bd.userData = driver_body_sprite;

    b2Body* driver_body = m_pBox2dWorld->CreateBody(&driver_body_bd);
    m_pCarShapeCache->addFixturesToBody(driver_body, "driver-body");

    driver_body_sprite->setAnchorPoint(m_pCarShapeCache->anchorPointForShape("driver-body"));

    b2Vec2 body_anchor((p.x + carInfo.driver_body_x)/PTM_RATIO, (p.y + carInfo.driver_body_y - 30)/PTM_RATIO);
    weld_jd.Initialize(m_pCarBody, driver_body, body_anchor);
    m_pBox2dWorld->CreateJoint(&weld_jd);

    weld_jd.frequencyHz = 9.0f;

    auto *driver_head_sprite = Sprite::create("box2d/driver-head.png");
    driver_head_sprite->setTag(SPRITE_DRIVER_HEAD);
    driver_head_sprite->setPosition(p.x + carInfo.driver_head_x, p.y + carInfo.driver_head_y);
    m_pTerrainView->addChild(driver_head_sprite, -2);

    b2BodyDef driver_head_bd;
    driver_head_bd.type = b2_dynamicBody;
    driver_head_bd.position.Set((p.x + carInfo.driver_head_x)/PTM_RATIO, (p.y + carInfo.driver_head_y)/PTM_RATIO);
    driver_head_bd.userData = driver_head_sprite;

    b2Body* driver_head = m_pBox2dWorld->CreateBody(&driver_head_bd);
    m_pCarShapeCache->addFixturesToBody(driver_head, "driver-head");
    driver_head_sprite->setAnchorPoint(m_pCarShapeCache->anchorPointForShape("driver-body"));

    b2Vec2 head_anchor((p.x + carInfo.driver_head_x)/PTM_RATIO, (p.y + carInfo.driver_head_y - 10)/PTM_RATIO);
    weld_jd.Initialize(driver_body, driver_head, head_anchor);
    m_pBox2dWorld->CreateJoint(&weld_jd);

    this->setWheelSpeed();
}

void GameScene::update(float dt)
{
    if (DataMgr::bIsPause)
    {
        return;
    }

    static double UPDATE_INTERVAL = 1.0f / 60.0f;
    static double MAX_CYCLES_PER_FRAME = 5;
    static double timeAccumulator = 0;
    
    timeAccumulator += dt;
    if (timeAccumulator > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL))
    {
        timeAccumulator = UPDATE_INTERVAL;
    }

    int32 velocityIterations = 3;
    int32 positionIterations = 2;
    while (timeAccumulator >= UPDATE_INTERVAL)
    {
        timeAccumulator -= UPDATE_INTERVAL;
        m_pBox2dWorld->Step(UPDATE_INTERVAL, velocityIterations, positionIterations);
        m_pBox2dWorld->ClearForces();
    }
    
    //Iterate over the bodies in the physics world
    for (b2Body* b = m_pBox2dWorld->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }	
    }

    std::vector<Box2dContact>::iterator iter;
    for(iter = m_pBox2dContactListener->m_contacts.begin(); iter != m_pBox2dContactListener->m_contacts.end(); ++ iter) 
    {
        Box2dContact contact = *iter;
        b2Body *bodyA = contact.fixtureA->GetBody();
        b2Body *bodyB = contact.fixtureB->GetBody();

        // 人碰到地面
        if(bodyA->GetUserData() == NULL &&bodyB->GetUserData() != NULL)
        {
            Sprite *spriteB = (Sprite*)bodyB->GetUserData();
            if(spriteB->getTag() == SPRITE_DRIVER_HEAD && bodyA->GetMass() == 0) 
            {
                log("GAME VORE");
                if (!m_bIsGameOver)
                {
                    AudioEnginMgr::getInstance()->playCrackEffect();
                }
                m_bIsGameOver = true;
                m_overtype = DRIVER_DOWN;
                scheduleOnce( schedule_selector(GameScene::gameOver), 2.0f);
                
            }
        }

        if(bodyB->GetUserData() == NULL &&bodyA->GetUserData() != NULL)
        {
            Sprite *spriteA = (Sprite*)bodyA->GetUserData();
            if(spriteA->getTag() == SPRITE_DRIVER_HEAD && bodyB->GetMass() == 0) 
            {
                log("GAME VORE");
                if (!m_bIsGameOver)
                {
                    AudioEnginMgr::getInstance()->playCrackEffect();
                }
            
                m_bIsGameOver = true;
                m_overtype = DRIVER_DOWN;
                scheduleOnce( schedule_selector(GameScene::gameOver), 2.0f);
            }
        }

        /*if(bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) 
        {
            Sprite *spriteA = (Sprite*)bodyA->GetUserData();
            Sprite *spriteB = (Sprite*)bodyB->GetUserData();
            log("spriteA tag===============================%d", spriteA->getTag());
            log("spriteB tag===============================%d", spriteB->getTag());
            if (spriteA->getTag() == SPRITE_CAR && spriteB->getTag() == SPRITE_GAS_STATION)
            {
            }
        }*/

    }
    m_nCurrentDistance = m_pCarBody->GetPosition().x - 10;

    /*float scale = (VisibleRect::top().y * 3 / 4) / m_pCarBody->GetPosition().y;
    if (scale > 1)
    {
        scale = 1;
    }
    m_pTerrainView->setScale(0.8); */
    //this->mapTestOffset();
    if (m_nCurrentDistance < 998)
    {
        m_pTerrainView->setOffsetX(Vec2(m_pCarBody->GetPosition().x - 5,m_pCarBody->GetPosition().y));
    }
    else
    {
        m_nCurrentDistance = 1000;
        m_overtype = MAP_COMPLETE;
        m_bIsGameOver = true;
        scheduleOnce( schedule_selector(GameScene::gameOver), 1.0f);
    }

    this->pickUpItems();
    this->updateLables();
    this->updateNeedleRotation();
}

void GameScene::brakeCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TouchEventType::TOUCH_EVENT_BEGAN:
        this->setWheelEnableMotor(true);
        AudioEnginMgr::getInstance()->playBrakeEffect(false);
        m_fPressPedalTime = 1.0f; 
        schedule( schedule_selector(GameScene::speedDown), 0.1f);
        break;  
    case TouchEventType::TOUCH_EVENT_MOVED:  
        break;  
    case TouchEventType::TOUCH_EVENT_ENDED: 
        m_fPressPedalTime = 0.0f; 
        this->setWheelEnableMotor(false);
        AudioEnginMgr::getInstance()->stopBrakeEffect();
        this->unschedule(schedule_selector(GameScene::speedDown));
        break;  
    case TouchEventType::TOUCH_EVENT_CANCELED:
        m_fPressPedalTime = 0.0f;
        this->setWheelEnableMotor(false);
        AudioEnginMgr::getInstance()->stopBrakeEffect();
        this->unschedule(schedule_selector(GameScene::speedDown));
         
        break;  
    }
}

void GameScene::gasCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TouchEventType::TOUCH_EVENT_BEGAN:  
        this->setWheelEnableMotor(true);
        m_fPressPedalTime = 1.0f;
        AudioEnginMgr::getInstance()->playGasEffect(false);
        schedule( schedule_selector(GameScene::speedUp), 0.1f);
        break;  
    case TouchEventType::TOUCH_EVENT_MOVED:  
        break;  
    case TouchEventType::TOUCH_EVENT_ENDED:
        m_fPressPedalTime = 0.0f;
        this->setWheelEnableMotor(false);
        AudioEnginMgr::getInstance()->stopGasEffect();
        this->unschedule(schedule_selector(GameScene::speedUp));
        break;  
    case TouchEventType::TOUCH_EVENT_CANCELED:
        m_fPressPedalTime = 0.0f;
        this->setWheelEnableMotor(false);
        AudioEnginMgr::getInstance()->stopGasEffect();
        this->unschedule(schedule_selector(GameScene::speedUp));
        break;  
    }
}

void GameScene::speedUp(float dt)
{
    m_fPressPedalTime += 0.1f;
    if(m_fRearWheelSpeed > 0)
    {
        m_fRearWheelSpeed = -10.0f;
    }
    else if (m_fRearWheelSpeed > -100.0f)
    {
         m_fRearWheelSpeed -= (5.0f + m_nEngineLevel * 2);
    }

    this->setWheelSpeed();
}

void GameScene::speedDown(float dt)
{
    m_fPressPedalTime += 0.1f;
    if(m_fRearWheelSpeed < 0)
    {
        m_fRearWheelSpeed = 0.0f;
    }
    else if (m_fRearWheelSpeed < 50.0f)
    {
        m_fRearWheelSpeed += (5.0f + m_nEngineLevel * 2);
    }

   
    this->setWheelSpeed();
}

void GameScene::pickUpItems()
{
    if (m_bIsGameOver)
    {
        return ;
    }
    Rect rCar = m_pTerrainView->getChildByTag(SPRITE_CAR)->getBoundingBox();

    auto gasStation = m_pTerrainView->getChildByTag(SPRITE_GAS_STATION);
    if (gasStation && rCar.intersectsRect(gasStation->getBoundingBox()))
    {
        Action* gas_action = Sequence::create(
        MoveBy::create(0.2, Vec2(200 / PTM_RATIO, 150 / PTM_RATIO)),
        FadeIn::create(0.1),
        CallFuncN::create(CC_CALLBACK_1(GameScene::addGas,this, 100)),
        RemoveSelf::create(),
        NULL);
        gasStation->runAction(gas_action);
    }
    
    auto coin1 = m_pTerrainView->getChildByTag(SPRITE_COIN1);

    if (coin1 && (rCar.intersectsRect(coin1->getBoundingBox()) || coin1->getPositionX() < rCar.getMidX()))
    {
        Action* coin_action1 = Sequence::create(
            MoveBy::create(0.2, Vec2(200 / PTM_RATIO, 150 / PTM_RATIO)),
            FadeIn::create(0.1),
            RemoveSelf::create(),
            CallFuncN::create(CC_CALLBACK_1(GameScene::addCoin,this, 5)),
            NULL);
        coin1->runAction(coin_action1);
    }

    auto coin2 = m_pTerrainView->getChildByTag(SPRITE_COIN2);
    if (coin2 && (rCar.intersectsRect(coin2->getBoundingBox()) || coin2->getPositionX() < rCar.getMidX()))
    {
        Action* coin_action2 = Sequence::create(
        MoveBy::create(0.2, Vec2(200 / PTM_RATIO, 150 / PTM_RATIO)),
        FadeIn::create(0.1),
        RemoveSelf::create(),
        CallFuncN::create(CC_CALLBACK_1(GameScene::addCoin,this, 10)),
        NULL);
        coin2->runAction(coin_action2);
    }

    auto coin3 = m_pTerrainView->getChildByTag(SPRITE_COIN3);
    if (coin3 && (rCar.intersectsRect(coin3->getBoundingBox()) || coin3->getPositionX() < rCar.getMidX()))
    {
        Action* coin_action3 = Sequence::create(
        MoveBy::create(0.2, Vec2(200 / PTM_RATIO, 150 / PTM_RATIO)),
        FadeIn::create(0.1),
        RemoveSelf::create(),
        CallFuncN::create(CC_CALLBACK_1(GameScene::addCoin,this, 50)),
        NULL);
        coin3->runAction(coin_action3);
    }

    auto coin4 = m_pTerrainView->getChildByTag(SPRITE_COIN4);
    if (coin4 && (rCar.intersectsRect(coin4->getBoundingBox()) || coin4->getPositionX() < rCar.getMidX()))
    {
        Action* coin_action4 = Sequence::create(
        MoveBy::create(0.2, Vec2(200 / PTM_RATIO, 150 / PTM_RATIO)),
        FadeIn::create(0.1),
        RemoveSelf::create(),
        CallFuncN::create(CC_CALLBACK_1(GameScene::addCoin,this, 100)),
        NULL);
        coin4->runAction(coin_action4);
    }

}

void GameScene::consumGas(float dt)
{
    if (DataMgr::bIsPause)
    {
        return;
    }

    m_nGas-=2;
    if (m_nGas < 0)
    {
        m_bIsGameOver = true;
        m_overtype = OUT_OF_FUEL;
        scheduleOnce( schedule_selector(GameScene::gameOver), 0.5f);
    }
    else if (m_nGas < 15 && !m_bIsTips)
    {
        m_bIsTips = true;
        this->gasLowTips();
    }
    m_pGasProgessBar->setPercent(m_nGas);
}

void GameScene::updateNeedleRotation()
{
    float speedRotation = -90;
    speedRotation +=  fabs(m_pRearWheelJoint->GetJointSpeed());
    if (speedRotation > 80)
    {
        speedRotation = 80;
    }
    auto speed_actionTo = RotateTo::create(0.2, speedRotation);
    m_pSpeedNeedle->runAction( Sequence::create(speed_actionTo, NULL));

    float boostRotation = -90;
    boostRotation +=  m_fPressPedalTime * 20;
    if (boostRotation > 50)
    {
        boostRotation = 50;
    }
    auto boost_actionTo = RotateTo::create(0.5, boostRotation);
    m_pBoostNeedle->runAction( Sequence::create(boost_actionTo, NULL));

}

void GameScene::updateLables()
{
    setLabelText(m_pDistanceLable, String::createWithFormat("%d",m_nCurrentDistance)->_string +"m/1000m");
    setLabelText(m_pCoinLable, m_nTotalCoins);
    if (m_nCurrentDistance > m_nBestDistance)
    {
        m_nBestDistance = m_nCurrentDistance;
        setLabelText(m_pBestDistanceLable, String::createWithFormat("%d",m_nBestDistance)->_string + "m");
    }
    
}

void GameScene::gameOver(float dt)
{
    UtilHelper::writeToInteger(OVER_COUNT, UtilHelper::getFromInteger(OVER_COUNT) + 1);

    // start app的广告
    if (UtilHelper::getFromInteger(OVER_COUNT) % UtilHelper::getFromInteger(START_APP) == 0)
    {
        UtilHelper::showStartAppAd(1);
    }
    
    if (UtilHelper::getFromInteger(OVER_COUNT) % 2 == 0 && !UtilHelper::getFromBool(RATE))
    {
        UtilHelper::showStartAppAd(3);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            umeng::MobClickCpp::event("user_rate");
#endif
    }

    this->unscheduleAllSelectors();
    UtilHelper::writeToInteger(USER_GOLD, m_nTotalCoins);
    UtilHelper::writeMapData(DataMgr::mapIndex + 1, e_bestDist, m_nBestDistance);
    DataMgr::getInstance()->setMapData(DataMgr::mapIndex , e_bestDist, m_nBestDistance);
    //not the last map and map complete, then unlock the next map
    if (strcmp(m_overtype.c_str(), MAP_COMPLETE) == 0 && DataMgr::mapIndex + 1 < DataMgr::getInstance()->getMapData().size())
    {
        UtilHelper::writeMapData(DataMgr::mapIndex + 2, e_map_open, 1);
        DataMgr::getInstance()->setMapData(DataMgr::mapIndex + 1, e_map_open, 1);
    }
    
    DataMgr::modalShow = true;
    GameOverDialog* dlg = new GameOverDialog();
    dlg->setDistance(m_nCurrentDistance);
    dlg->setCoins(m_nCurrentCoins);
    dlg->setOverType(m_overtype);
    dlg->setCallbackFunc(this, callfunc_selector(GameScene::playCallback));
    dlg->init();
    this->addChild(dlg, 999);
    dlg->release();
    AudioEnginMgr::getInstance()->playScreenShotEffect();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (m_overtype == MAP_COMPLETE)
    {
        umeng::MobClickCpp::finishLevel(String::createWithFormat("%d",m_nMapId)->_string.c_str());
    }
    else 
    {
        umeng::MobClickCpp::failLevel(String::createWithFormat("%d",m_nMapId)->_string.c_str());
    }

    umeng::eventDict dict;
    dict["type"] = m_overtype;
    dict["mapId"] = String::createWithFormat("%d",m_nMapId)->_string;
    dict["carId"] = String::createWithFormat("%d",m_nCarId)->_string;

    dict["engineLevel"] = String::createWithFormat("%d",m_nEngineLevel)->_string;
    dict["suspensionLevel"] = String::createWithFormat("%d",m_nSuspensionLevel)->_string;
    dict["wheelLevel"] = String::createWithFormat("%d",m_nWheellLevel)->_string;
    dict["fourWheelDrivLevel"] = String::createWithFormat("%d",m_nFourWheelDriveLevel)->_string;

    dict["distance"] = String::createWithFormat("%d",m_nCurrentDistance)->_string;
    dict["coin"] = String::createWithFormat("%d",m_nCurrentCoins)->_string;
    dict["total_coin"] = String::createWithFormat("%d",m_nTotalCoins)->_string;
    umeng::MobClickCpp::event("game_over", &dict);
#endif
}

void GameScene::playCallback()
{
    if (strcmp(m_overtype.c_str(), MAP_COMPLETE) == 0)
    {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, SelectMapScene::createScene()));
    }
    else
    {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, UpgradeCarScene::createScene()));
    }
}

void GameScene::pauseCallback(Ref* sender,TouchEventType type)
{
    switch (type)
    {
    case TOUCH_EVENT_ENDED:
        {
            
            AudioEnginMgr::getInstance()->playBtnEffect();
            AudioEnginMgr::getInstance()->pauseBackgroundMusic();
            DataMgr::bIsPause =true;
            PauseDialog* dlg = new PauseDialog();
            dlg->setResumeCallbackFunc(this, callfunc_selector(GameScene::resume));
            dlg->setRestartCallbackFunc(this, callfunc_selector(GameScene::restart));
            dlg->setBackCallbackFunc(this, callfunc_selector(GameScene::back));
            dlg->init();
            this->addChild(dlg, 999);
            dlg->release();
        }
        break;
    default:
        break;
    }
}

void GameScene::resume()
{
    DataMgr::bIsPause = false;
    AudioEnginMgr::getInstance()->playBtnEffect();
    AudioEnginMgr::getInstance()->resumeBackgroundMusic();
}

void GameScene::restart()
{
    AudioEnginMgr::getInstance()->playBtnEffect();
    AudioEnginMgr::getInstance()->resumeBackgroundMusic();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameScene::createScene()));
}

void GameScene::back()
{
    AudioEnginMgr::getInstance()->playBtnEffect();
    AudioEnginMgr::getInstance()->resumeBackgroundMusic();
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, UpgradeCarScene::createScene()));
}

void GameScene::addCoin(Ref* sender, int coins)
{
    AudioEnginMgr::getInstance()->playPickCoinEffect();
    m_nCurrentCoins += coins;
    m_nTotalCoins += coins;
}

void GameScene::addGas(Ref* sender, int gas)
{
    m_nGas = gas;
    AudioEnginMgr::getInstance()->playPickFuelEffect();
    if (m_pFuelLowSprite)
    {
        m_pFuelLowSprite->setVisible(false);
    }
    m_bIsTips = false;
    
}

void GameScene::mapTestOffset()
{
    m_fTestMapOffset += 0.2;
    m_nGas += 1;
    m_nCurrentDistance = m_fTestMapOffset;
    if (m_fTestMapOffset < 1000)
    {
        m_pTerrainView->setOffsetX(Vec2(m_fTestMapOffset,0));
    }
}

void GameScene::setWheelSpeed()
{
     m_pRearWheelJoint->SetMotorSpeed(m_fRearWheelSpeed);
     m_pFrontWheelJoint->SetMotorSpeed(m_fRearWheelSpeed / 2.0);
}

void GameScene::setWheelEnableMotor(bool bIsPress)
{
    m_pRearWheelJoint->EnableMotor(bIsPress);
    if (bIsPress == true && m_nFourWheelDriveLevel > 1)
    {
        m_pFrontWheelJoint->EnableMotor(true);
    }
    else
    {
        m_pFrontWheelJoint->EnableMotor(false);
    }
    
}

void GameScene::onEnterTransitionDidFinish()
{
    DataMgr::bIsGameScene = true;
    AudioEnginMgr::getInstance()->stopBackgroundMusic();
    AudioEnginMgr::getInstance()->playBackgroundMusic();
    Layer::onEnter();
}

void GameScene::onExit()
{
    Layer::onExit();
    DataMgr::bIsGameScene = false;
    DataMgr::bIsPause = false;
    AudioEnginMgr::getInstance()->stopBackgroundMusic();
    AudioEnginMgr::getInstance()->playBackgroundMusic();
}

void GameScene::gasLowTips()
{
    m_pFuelLowSprite->setVisible(true);

    auto fuel_low_action = Sequence::create( 
                                               FadeIn::create(1.0f),
                                               FadeOut::create(1.0f),
                                               FadeIn::create(1.0f),
                                               FadeOut::create(1.0f),
                                               FadeIn::create(1.0f),
                                               FadeOut::create(1.0f),
                                               //RemoveSelf::create(),
                                               NULL);
    m_pFuelLowSprite->runAction(fuel_low_action);
}