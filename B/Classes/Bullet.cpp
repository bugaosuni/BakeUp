﻿#include "Bullet.h"
#include "AnimationManager.h"
#include "Hero.h"
#include "GameMap.h"
#include "GameScene.h"

// ****************** Bullet ******************** //
Bullet::Bullet():
bulletBody(NULL)
{
    Point heroPos = Hero::getInstance()->getPosition();
    Size heroSize = Hero::getInstance()->getContentSize();
    startPos = Point(heroPos.x, heroPos.y + heroSize.height/2 - 20);
}

Bullet::~Bullet()
{

}

void Bullet::setBodySize(Size _size)
{
    bodySize = _size;
}
Size Bullet::getBodySize()
{
    return bodySize;
}

void Bullet::setStartPos(Point pos)
{
    startPos = pos;
}
Point Bullet::getStartPos()
{
    return startPos;
}

BulletType Bullet::getBulletType()
{
    return bulletType;
}

BulletState Bullet::getBulletState()
{
    return bulletState;
}

void Bullet::checkBulletState()
{

}

Rect Bullet::getBulletRect()
{
    Point pos = this->getPosition();
    return CCRectMake(pos.x - bodySize.width/2, pos.y, bodySize.width, bodySize.height);
}

void Bullet::forKilledEnemy()
{

}

// ****************** BulletCommon ******************** //
BulletCommon::BulletCommon()
{
    bulletType = eBullet_common;
    bulletState = eBulletState_active;
    isLand = false;

    bodySize = CCSizeMake(40, 40);
    bulletBody = Sprite::create("image/Tannenzapfen.png", CCRectMake(0, 0, 40, 40));
    bulletBody->setAnchorPoint(Point(0, 0));
    this->setContentSize(bodySize);
    this->addChild(bulletBody);
    this->setAnchorPoint(Point(0.5f, 0.0f));

    moveOffset = 0.0f;
    ccMoveOffset = 5.0f;
    jumpOffset = 0.0f;
    ccJumpOffset = 0.0f;

    switch (Hero::getInstance()->m_eFace)
    {
    case eRight:
        moveOffset = ccMoveOffset;
        break;
    case eLeft:
        moveOffset = -ccMoveOffset;
        break;
    default:
        break;
    }
}

BulletCommon::~BulletCommon()
{
    this->unscheduleAllSelectors();
}

void BulletCommon::launchBullet()
{
    bulletBody->runAction(RepeatForever::create(AnimationManager::getInstance()->createAnimate(eAniRotatedFireBall)));
    
    this->scheduleUpdate();
}

// 子弹的两侧碰撞检测
// 如果两侧碰到了land，block，pipe等会自动爆炸掉
void BulletCommon::commonBulletCollisionH()
{
    Point currentPos = this->getPosition();

    // 判断子弹是否会射出屏幕
    float leftSide = currentPos.x - bodySize.width/2;
    float rightSide = currentPos.x + bodySize.width/2;
    float mapMaxH = GameScene::getMapMaxH();
    Size winSize = Director::getInstance()->getWinSize();
    if (fabs(leftSide - mapMaxH) <= 8)
    {
        this->showBoom();
        return ;
    }

    // 判断是否射出地图右侧
    float mapRightSide = GameMap::getGameMap()->m_mapSize.width*
        GameMap::getGameMap()->m_tileSize.width;
    if (fabs(rightSide - mapRightSide) <= 8)
    {
        this->showBoom();
        return ;
    }

    if (leftSide - mapMaxH >= winSize.width)
    {
        bulletBody->stopAllActions();
        this->unscheduleUpdate();
        this->autoClear();
    }

    // 右侧
    Point rightCollision = Point(currentPos.x + bodySize.width/2, currentPos.y + bodySize.height/2);
    Point rightTilecoord = GameMap::getGameMap()->positionToTileCoord(rightCollision);
    TileType tileType = GameMap::getGameMap()->tileTypeforPos(rightTilecoord);
    switch (tileType)
    {
    case eTile_Land:
    case eTile_Pipe:
    case eTile_Block:
    case eTile_Flagpole:
        this->showBoom();
        return ;
        break;
    }

    // 左侧
    Point leftCollision = Point(currentPos.x - bodySize.width/2, currentPos.y + bodySize.height/2);
    Point leftTilecoord = GameMap::getGameMap()->positionToTileCoord(leftCollision);
    tileType = GameMap::getGameMap()->tileTypeforPos(leftTilecoord);
    switch (tileType)
    {
    case eTile_Land:
    case eTile_Pipe:
    case eTile_Block:
    case eTile_Flagpole:
        this->showBoom();
        return ;
        break;
    }
}

// 因为子弹只会往下落，所以这里仅仅做脚底的判断
// 脚底如果接触到land，block，会自动反弹，但是脚底碰到pipe也会爆炸
void BulletCommon::commonBulletCollisionV()
{
    Point currentPos = this->getPosition();

    Point downCollision = currentPos;
    Point downTilecoord = GameMap::getGameMap()->positionToTileCoord(downCollision);
    downTilecoord.y += 1;
    Point downPos = GameMap::getGameMap()->positionToTileCoord(downTilecoord);
    downPos = Point(currentPos.x, downPos.y + GameMap::getGameMap()->getTileSize().height);

    TileType tileType = GameMap::getGameMap()->tileTypeforPos(downTilecoord);
    switch (tileType)
    {
    case eTile_Land:
        isLand = true;
        jumpOffset = 3.0f;
        this->setPosition(downPos);
        return ;
        break;
    case eTile_Pipe:
    case eTile_Block:
        this->showBoom();
        return ;
        break;
    case eTile_Trap:
        {
            bulletBody->stopAllActions();
            this->unscheduleUpdate();
            this->autoClear();
            return ;
            break;
        }
    default:
        break;
    }

    jumpOffset -= ccJumpOffset;
}

void BulletCommon::update(float dt)
{
    if (bulletState == eBulletState_active)
    {
        Point currentPos = this->getPosition();
        currentPos.x += moveOffset;
        currentPos.y += jumpOffset;
        this->setPosition(currentPos);

        this->commonBulletCollisionH();
       // this->commonBulletCollisionV();
    }
}

void BulletCommon::showBoom()
{	
    bulletBody->stopAllActions();
    this->stopAllActions();
    this->unscheduleUpdate();
    bulletBody->runAction(Sequence::create(AnimationManager::getInstance()->createAnimate(eAniBoomedFireBall), 
        CallFunc::create(this, callfunc_selector(BulletCommon::autoClear)), NULL));
}

void BulletCommon::autoClear()
{	
    bulletState = eBulletState_nonactive;
    this->setVisible(false);
}

void BulletCommon::forKilledEnemy()
{
    this->showBoom();
}


// ****************** BulletArrow ******************** //
BulletArrow::BulletArrow()
{
    bulletType = eBullet_arrow;
    bulletState = eBulletState_active;

    bodySize = CCSizeMake(16, 16);
    bulletBody = Sprite::create("image/props/arrow.png");
    bulletBody->setAnchorPoint(Point(0, 0));
    this->setContentSize(bodySize);
    this->addChild(bulletBody);
    this->setAnchorPoint(Point(0.5f, 0.5f));

    ccMoveOffset = 6.0f;
    CCActionInstant *flipX = CCFlipX::create(true);

    switch (Hero::getInstance()->m_eFace)
    {
    case eRight:
        moveOffset = ccMoveOffset;
        break;
    case eLeft:
        moveOffset = -ccMoveOffset;
        bulletBody->runAction(flipX);
        break;
    default:
        break;
    }

}

BulletArrow::~BulletArrow()
{
    this->unscheduleAllSelectors();
}

void BulletArrow::launchBullet()
{
    this->scheduleUpdate();
}

void BulletArrow::arrowBulletCollisionH()
{
    Point currentPos = this->getPosition();
    
    // 判断子弹是否会射出屏幕
    float leftSide = currentPos.x - bodySize.width/2;
    float rightSide = currentPos.x + bodySize.width/2;
    float mapMaxH = GameScene::getMapMaxH();
    Size winSize = Director::getInstance()->getWinSize();
    if (fabs(leftSide - mapMaxH) <= 8)
    {
        this->broken();
        return ;
    }
    // 判断是否射出地图右侧
    float mapRightSide = GameMap::getGameMap()->m_mapSize.width*
        GameMap::getGameMap()->m_tileSize.width;
    if (fabs(rightSide - mapRightSide) <= 8)
    {
        this->broken();
        return ;
    }
    
    if (leftSide - mapMaxH >= winSize.width)
    {
        bulletBody->stopAllActions();
        this->stopAllActions();
        this->unscheduleUpdate();
        this->autoClear();
    }

    // 右侧
    Point rightCollision = Point(currentPos.x + bodySize.width/2, currentPos.y);
    Point rightTilecoord = GameMap::getGameMap()->positionToTileCoord(rightCollision);
    TileType tileType = GameMap::getGameMap()->tileTypeforPos(rightTilecoord);
    switch (tileType)
    {
    case eTile_Land:
    case eTile_Pipe:
    case eTile_Block:
    case eTile_Flagpole:
        this->broken();
        return ;
        break;
    }

    // 左侧
    Point leftCollision = Point(currentPos.x - bodySize.width/2, currentPos.y);
    Point leftTilecoord = GameMap::getGameMap()->positionToTileCoord(leftCollision);
    tileType = GameMap::getGameMap()->tileTypeforPos(leftTilecoord);
    switch (tileType)
    {
    case eTile_Land:
    case eTile_Pipe:
    case eTile_Block:
    case eTile_Flagpole:
        this->broken();
        return ;
        break;
    }
}

void BulletArrow::forKilledEnemy()
{
    bulletState = eBulletState_nonactive;
    bulletBody->stopAllActions();
    this->stopAllActions();
    this->unscheduleUpdate();
    this->setVisible(false);
}

void BulletArrow::update(float dt)
{
    if (bulletState == eBulletState_active)
    {
        Point currentPos = this->getPosition();
        currentPos.x += moveOffset;

        this->setPosition(currentPos);
        this->arrowBulletCollisionH();
    }
}

Rect BulletArrow::getBulletRect()
{
    Point pos = this->getPosition();
    return CCRectMake(pos.x - 6, pos.y - 5, 12, 10);
}

void BulletArrow::autoClear()
{
    bulletState = eBulletState_nonactive;
    this->setVisible(false);
}

void BulletArrow::broken()
{
    bulletBody->stopAllActions();
    this->stopAllActions();
    this->unscheduleUpdate();

    bulletBody->runAction(Sequence::create(AnimationManager::getInstance()->createAnimate(eAniArrowBroken),
        CallFunc::create(this, callfunc_selector(BulletArrow::autoClear)), NULL));
}
