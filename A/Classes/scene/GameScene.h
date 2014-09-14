#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "Base.h"
#include "Box2D/Box2D.h"
#include "utils/GLES-Render.h"
#include "view/TerrainView.h"
#include "utils/GB2ShapeCache-x.h"
#include "utils/ContactListener.h"
#include "DataMgr.h"

#define PTM_RATIO 32

typedef enum
{
    SPRITE_GROUND = 10000,
    SPRITE_SURFACE,
    SPRITE_CAR,
    SPRITE_WHEEL,
    SPRITE_DRIVER_BODY,
    SPRITE_DRIVER_HEAD,
    SPRITE_GAS_STATION,
    SPRITE_COIN1,
    SPRITE_COIN2,
    SPRITE_COIN3,
    SPRITE_COIN4,
    SPRITE_ANTENNA
}SPRITE_TAG;

class GameScene: public Base
{
public:
    GameScene();
    ~GameScene();

    static Scene* createScene();
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();

private:
    void initUI();
    void initMap();
    void initCar();
    void initShape();

    void update(float dt);
    void setupWorld();
    void setupDebugDraw();
    void setupStarAndEndPoint();

    void brakeCallback(Ref* sender,TouchEventType type);
    void gasCallback(Ref* sender,TouchEventType type);
    void pauseCallback(Ref* sender,TouchEventType type);

    void resume();
    void restart();
    void back();


    void setWheelSpeed();
    void setWheelEnableMotor(bool bIsPress);
    
    void speedUp(float dt);
    void speedDown(float dt);
    void gameOver(float dt);

    void consumGas(float dt);
    void pickUpItems();
    
    void updateNeedleRotation();
    void updateLables();
    
    void addCoin(Ref* sender, int coins);
    
    void addGas(Ref* sender, int gas);

    void playCallback();

    void mapTestOffset();
    
    void gasLowTips();
    
    CREATE_FUNC(GameScene);

    virtual void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

private:
    int m_nTotalCoins;
    int m_nBestDistance;
    int m_nCurrentDistance;
    int m_nCurrentCoins;
    int m_nGas;
    int m_nMapId;
    int m_nCarId;
    int m_nFourWheelDriveLevel;
    int m_nEngineLevel;
    int m_nSuspensionLevel;
    int m_nWheellLevel;


    float m_fPressPedalTime;
    float m_fRearWheelSpeed;
    float m_fTestMapOffset;

    bool m_bIsGameOver;
    bool m_bIsTips;

    string m_overtype;

    LoadingBar* m_pGasProgessBar;
    Widget* m_pSpeedNeedle;
    TerrainView* m_pTerrainView;
    Widget* m_pBoostNeedle;
    Widget* m_pGameBg;
    Widget* m_pDistanceLable;
    Widget* m_pBestDistanceLable;
    Widget* m_pCoinLable;
    Widget* m_pFuelLowSprite;

    b2Body* m_pCarBody;
    b2Body* m_pRearWheelBody;
    b2Body* m_pFrontWheelBody;
    b2WheelJoint* m_pRearWheelJoint;
    b2WheelJoint* m_pFrontWheelJoint;
    b2World *m_pBox2dWorld;
    GLESDebugDraw* m_pBox2dDebugDraw;
    GB2ShapeCache* m_pCarShapeCache;
    ContactListener* m_pBox2dContactListener;

};
#endif // __GAME_SCENE_H__