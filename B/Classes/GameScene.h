/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-05
Description:这个类是游戏内容图层，所有的游戏内容都在这一个图层中进行
这个类中要加载一张地图，加载一个主角
在这个图层之上负责碰撞检测
再折耳根图层之上负责实时更新
持有一些和主角位置变化相关的变量，这些变量用于模拟出物理效果
**************************************************/

#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "GameMap.h"
#include "Hero.h"
#include "Base.h"

using namespace cocos2d;

class GameScene : public Base
{
public:
     GameScene();
    ~GameScene();
    static Scene *createScene();
    virtual bool init();
    void initHeroAndMap();
    void update(float dt);
    void singleJump(float dt);
    void updateControl();
    void collistionV();  // 水平方向的碰撞检测
    void collistionH();  // 垂直方向的碰撞检测
    void setSceneScrollPosition();  // 根据主角的当前位置移动场景至合适的位置
    static float getMapMaxH();
    void initControlUI();
    void reSetHeroFireable();

    void leftCallback(Ref* sender,TouchEventType type);
    void rightCallback(Ref* sender,TouchEventType type);
    void jumpCallback(Ref* sender,TouchEventType type);
    void shootCallback(Ref* sender,TouchEventType type);

private:
    Layer *m_pMainLayer;  // 主游戏图层
    GameMap *m_pGameMap;  // 主游戏地图
    Hero *m_pHero;        // 游戏中的主角
    Size m_heroSize;     // 主角的尺寸
    Size m_mapSize;      // 地图的尺寸
    Point m_mapMoveAnchor;  // 主角走到屏幕的该点后地图就开始跟随
    Point m_mapPosition; // 地图层位于主游戏图层中的位置
    Point m_heroCurrentPos;  // 主角当前的位置

    Button* m_pLeftButton;
    Button* m_pRightButton;
    Button* m_pJumpButton;
    Button* m_pShootButton;

    static float m_fMapMaxH;// 水平方向上游戏主地图gameMap缩进的X最大分量，防止地图后退出屏幕
    float m_fCcMoveOffset;  // 每帧要移动的距离
    float m_fCcJumpOffset;
    float m_fMoveOffset;  // 水平移动偏移量
    float m_fJumpOffset;  // 垂直跳跃的偏移量

    
    bool m_bIsLeftPress;  // 标识左键是否被按下
    bool m_bIsRightPress; // 标识右键是否被按下
    bool m_bIsJumpPress;  // 标识跳跃键是否被按下
    bool m_bIsFirePress;  // 标识开火建是否被按下

    bool m_bIsSky;        // 标识主角是否在空中状态
    bool m_bJumpClick;    // 用来判断连跳的变量
    bool m_bHeroFireable; // 用来判断是否可以发生子弹

    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

    CREATE_FUNC(GameScene);
};

#endif