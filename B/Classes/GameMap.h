/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-05
Description:游戏主菜单页的关于页面
**************************************************/

#ifndef _GAMEMAP_H_
#define _GAMEMAP_H_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;


class GameMap : public TMXTiledMap
{
    CC_PROPERTY_READONLY(TMXLayer*, landLayer, LandLayer);
    // 金币层
    CC_PROPERTY_READONLY(TMXLayer*, coinLayer, CoinLayer);
    // 对象层
    CC_PROPERTY_READONLY(TMXObjectGroup*, objectLayer, ObjectLayer);

public:
    GameMap();
    ~GameMap();
    static GameMap* create(const char *tmxFile);
    TileType tileTypeforPos(Point tileCoord);// 返回地图指定位置上的图块类型，用作碰撞检测
    Point positionToTileCoord(Point pos); // 游戏图层坐标系与tilemap图块坐标系之间的转换
    Point tilecoordToPosition(Point tileCoord);
    
    bool crossedHeadBlock(CCPoint tileCoord, BodyType _bodyType); // 地图上的block图层上的某些砖块是可以被主角顶的
    void showBlockBroken(Point tileCoord);// 展示block图层上的某些砖块被大马里奥顶碎成四块的动画
    void clearSpriteArray();
	void launchEnemyInMap();// 启动地图上的所有怪物，开启动画
    void enemyVSHero();
    void update(float dt);

    void createNewBullet();  // 产生一个新的子弹，并启动起来
    
    void bulletVSEnemy();   // 子弹与怪物的碰撞检测
    Point getHeroBirthPos();
    static GameMap* getGameMap();
    Size m_tileSize; // 地图中的图块尺寸
    Size m_mapSize;  // 地图中图块的个数

    
     

private:
    void extraInit();
    void initObjects();

    Point m_heroBirthPos; // 每张地图中马里奥的出生点，GL坐标系下的坐标，GameLayer获取后直接可用的
    Array *m_pBreakSpriteArray;
    
    
    Array *m_pEnemyArray;   // 地图上保存的所有怪物的信息数组
    Array *m_pBulletArray;  // 地图上所有的子弹的信息
    
    static GameMap* m_pGameMap;

    

};

#endif