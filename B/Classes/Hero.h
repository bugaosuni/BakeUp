/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-05
Description:主角
**************************************************/

#ifndef _HERO_H_
#define _HERO_H_

#include "cocos2d.h"
#include "GameEnum.h"
using namespace cocos2d;

class Hero : public Node
{
public:
    Hero();
    ~Hero();

    static Hero* getInstance();
    static Hero* create();
    void setHeroState(marioDirection state);
    marioDirection getHeroState();
    void setBodyType(BodyType _bodytype);
    Size getCurrentSize();
    BodyType getCurrentBodyType();
    void setHeroDie(bool _die);
    bool isHeroDied();
    void dieForTrap();   // 主角掉入陷阱里要执行一段上移和下移的动画后消失掉
    void reSetNonVisible();
    bool isBulletable(); // 判断主角是否可以发射子弹
    bool getIsSafeTime();
    void changeForGotEnemy();
    void reSetSafeTime();
    void setSafeTime(bool _issafe);
    
    void fireAction();
    void reSetStateForFired();

    static Hero *m_pHeroInstance;
    Sprite *m_pMainBody;  // 主要的主角精灵
    
    SpriteFrame *m_pSmallRight;  // 对应着小号主角状态下的
    SpriteFrame *m_pSmallLeft;
    SpriteFrame *m_pSmallJumpLeft;
    SpriteFrame *m_pSmallJumpRight;
    SpriteFrame *m_pSmallDownLeft;
    SpriteFrame *m_pSmallDownRight;

    SpriteFrame *m_pSmallRightFire;
    SpriteFrame *m_pJumpRightFire;
    SpriteFrame *m_pSmallLeftFire;
    SpriteFrame *m_pJumpLeftFire;

    SpriteFrame *m_pLifeOverSmall;

    Size m_smallSize;    // 小号身材
    Size m_currentSize;  // 当前的身材

    marioDirection m_eState;  // 标示当前主角的状态
    marioDirection m_eStatePre;  // 标识主角的上一个状态
    BodyType m_eBodyType;   // 当前的身体状态
    marioDirection m_eFace;   // 主角的朝向

    bool m_bIsSky;   // 主角是否在空中飞行状态
    bool m_bIsDied;   // 是否已经死亡
    bool m_bIsSafeTime;

private:
    bool heroInit();

    bool m_bBulletable;
    
};

#endif