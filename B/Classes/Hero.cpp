#include "Hero.h"
#include "AnimationManager.h"
#include "Global.h"
#include "SimpleAudioEngine.h"

Hero* Hero::m_pHeroInstance;
Hero* Hero::getInstance()
{
    return m_pHeroInstance;
}

Hero::Hero()
{
    m_pHeroInstance = NULL;
    m_pMainBody = NULL;
    m_pSmallRight = NULL;
    m_pSmallLeft = NULL;
    m_pSmallJumpLeft = NULL;
    m_pSmallJumpRight = NULL;
    m_pSmallDownLeft = NULL;
    m_pSmallDownRight = NULL;
    m_pLifeOverSmall = NULL;
    
    m_smallSize = Size(40, 80);
    m_currentSize = m_smallSize;

    m_eState = eNormalRight;
    m_eStatePre = eNormalRight;
    m_eBodyType = eBody_Small;
    m_eFace = eRight;
    
    m_bIsSky = false;
    m_bIsDied = false;
    m_bBulletable = true;
    m_bIsSafeTime = false;

    CCTexture2D *pTexture = TextureCache::getInstance()->addImage("image/hero/playerRight.png");
    m_pSmallRight = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 60, 80));
    m_pSmallRight->retain();

    m_pSmallJumpRight = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * 3, 60, 80));
    m_pSmallJumpRight->retain();

    m_pSmallDownRight = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * 4, 60, 80));
    m_pSmallDownRight->retain();

    // 可以现在发射字段的相关图片
    m_pSmallRightFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 60, 80));
    m_pSmallRightFire->retain();
    m_pJumpRightFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * 3, 60, 80));
    m_pJumpRightFire->retain();

    pTexture = TextureCache::getInstance()->addImage("image/hero/playerLeft.png");
    m_pSmallLeft = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 60, 80));
    m_pSmallLeft->retain();
    m_pSmallJumpLeft = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * 3, 60, 80));
    m_pSmallJumpLeft->retain();
    m_pSmallDownLeft = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * 4, 60, 80));
    m_pSmallDownLeft->retain();

    // 可以现在发射字段的相关图片
    m_pSmallLeftFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 60, 80));
    m_pSmallLeftFire->retain();
    m_pJumpLeftFire = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * 3, 60, 80));
    m_pJumpLeftFire->retain();

   

    pTexture = TextureCache::getInstance()->addImage("image/hero/playerDied.png");
    m_pLifeOverSmall = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 60, 80));
    m_pLifeOverSmall->retain();

}

Hero::~Hero()
{
    this->unscheduleAllSelectors();
}

void Hero::setHeroState(marioDirection _state)
{
    if (m_bIsDied)
    {
        return;
    }
    if (m_eState == _state)
    {
        return;
    }
    m_eStatePre = m_eState;
    m_eState = _state;

    m_pMainBody->stopAllActions();

    switch (_state)
    {
    case eNormalRight:
        if (m_bBulletable)
        {
            m_pMainBody->setDisplayFrame(m_pSmallRightFire);
        }
        else
        {
            m_pMainBody->setDisplayFrame(m_pSmallRight);
        }
        m_eFace = eRight;
        break;
    case eNormalLeft:
        if (m_bBulletable)
        {
            m_pMainBody->setDisplayFrame(m_pSmallLeftFire);
        }
        else
        {
            m_pMainBody->setDisplayFrame(m_pSmallLeft);
        }
        m_eFace = eLeft;
        break;
    case eRight: //走的过程中，播放动画
        if (!m_bIsSky)
        {
            if(m_bBulletable)
            {
                m_pMainBody->runAction(RepeatForever::create(
                            AnimationManager::getInstance()->createAnimate(eAniRightFire)));
            }
            else
            {
                m_pMainBody->runAction(RepeatForever::create(
                    AnimationManager::getInstance()->createAnimate(eAniRightSmall)));
            }
        }
        m_eFace = eRight;
        break;
    case eLeft:
        if (!m_bIsSky)
        {
            if(m_bBulletable)
            {
                m_pMainBody->runAction(RepeatForever::create(
                    AnimationManager::getInstance()->createAnimate(eAniLeftFire)));
            }
            else
            {
                 m_pMainBody->runAction(RepeatForever::create(
                    AnimationManager::getInstance()->createAnimate(eAniLeftSmall)));
            }
        }
        m_eFace = eLeft;
        break;
    case eJumpLeft:
        if (m_bBulletable)
        {
            m_pMainBody->setDisplayFrame(m_pJumpLeftFire);
        }
        else
        {
            m_pMainBody->setDisplayFrame(m_pSmallJumpLeft);
        }
        m_eFace = eLeft;
        break;
    case eJumpRight:
        if (m_bBulletable)
        {
            m_pMainBody->setDisplayFrame(m_pJumpRightFire);
        }
        else
        {
            m_pMainBody->setDisplayFrame(m_pSmallJumpRight);
        }
        m_eFace = eRight;
        break;
    case eDownLeft:
        m_pMainBody->setDisplayFrame(m_pSmallDownLeft);
        m_eFace = eLeft;
        break;
    case eDownRight:
        m_pMainBody->setDisplayFrame(m_pSmallDownRight);
        m_eFace = eRight;
        break;
    default:
        break;
    }
}

marioDirection Hero::getHeroState()
{
    return m_eState;
}

void Hero::setHeroDie(bool _die)
{
    m_bIsDied = _die;
    Global::getInstance()->currentHeroType = eBody_Small;
    Global::getInstance()->lifeNumCutOne();
}

bool Hero::isHeroDied()
{
    return m_bIsDied;
}

bool Hero::heroInit()
{
    this->setContentSize(m_smallSize);
    // 最初加载的是小型马里奥
    m_pMainBody = Sprite::create("image/hero/playerRight.png", CCRectMake(0, 0, 60, 80));
    m_pMainBody->setAnchorPoint(Point(0, 0));
    this->addChild(m_pMainBody);
    m_eState = eNormalRight;
    m_pHeroInstance = this;
    return true;
}

Hero* Hero::create()
{
    Hero *pHero = new Hero();
    if (pHero && pHero->heroInit())
    {
        pHero->autorelease();
        return pHero;
    }
    CC_SAFE_DELETE(pHero);
    return NULL;
}

void Hero::setBodyType(BodyType _bodytype)
{
    m_eBodyType = _bodytype;
    switch (_bodytype)
    {

    case eBody_Small:
        m_currentSize = m_smallSize;
        m_pMainBody->setDisplayFrame(m_pSmallRight);
        break;
    case eBody_Fireable:
        m_currentSize = m_smallSize;
        m_bBulletable = true;
        m_pMainBody->setDisplayFrame(m_pSmallRightFire);
        break;
    default:
        break;
    }
    this->setContentSize(m_currentSize);
}

Size Hero::getCurrentSize()
{
    return m_currentSize;
}

BodyType Hero::getCurrentBodyType()
{
    return m_eBodyType;
}

bool Hero::isBulletable()
{
    return m_bBulletable;
}

void Hero::dieForTrap()
{
    m_pMainBody->stopAllActions();

    switch (m_eBodyType)
    {
    case eBody_Small:
        if (m_bBulletable)
        {
            //m_pMainBody->setDisplayFrame(_lifeOverFire);
            //m_pMainBody->runAction(AnimationManager::getInstance()->createAnimate(eAniFireDie));
        }
        else
        {
            m_pMainBody->setDisplayFrame(m_pLifeOverSmall);
            m_pMainBody->runAction(AnimationManager::getInstance()->createAnimate(eAniSmallDie));
        }
        break;
    default:
        break;
    }

    ActionInterval *pMoveUp = MoveBy::create(0.6f, Point(0, 32));
    ActionInterval *pMoveDown = MoveBy::create(0.6f, Point(0, -32));
    ActionInterval *pDeley = DelayTime::create(0.2f);

    this->runAction(Sequence::create(pMoveUp, pDeley, pMoveDown,
        CallFunc::create(this, callfunc_selector(Hero::reSetNonVisible)), NULL));
}

void Hero::reSetNonVisible()
{
    m_pMainBody->stopAllActions();
    this->setVisible(false);
}

void Hero::fireAction()
{
    ActionInterval *pAction = NULL;
    this->setHeroState(eFireTheHole);
    switch (m_eFace)
    {
    case eRight:
        pAction = AnimationManager::getInstance()->createAnimate(eAniFireActionR);
        break;
    case eLeft:
        pAction = AnimationManager::getInstance()->createAnimate(eAniFireActionL);
        break;
    default:
        break;
    }
    m_pMainBody->runAction(Sequence::create(pAction,
        CallFunc::create(this, callfunc_selector(Hero::reSetStateForFired)), NULL));
}

void Hero::reSetStateForFired()
{
    this->setHeroState(m_eStatePre);
}

void Hero::changeForGotEnemy()
{
    m_bIsSafeTime = true;
    ActionInterval *pDelay = DelayTime::create(3.0f);
    this->runAction(Sequence::create(pDelay,
        CallFunc::create(this, callfunc_selector(Hero::reSetSafeTime)), NULL));

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("YuDaoGuaiWuSi.ogg");
    m_pMainBody->stopAllActions();
    m_pMainBody->setDisplayFrame(m_pLifeOverSmall);
    this->setHeroDie(true);
}

void Hero::setSafeTime(bool _issafe)
{
    m_bIsSafeTime = _issafe;
}

bool Hero::getIsSafeTime()
{
    return m_bIsSafeTime;
}

void Hero::reSetSafeTime()
{
    m_bIsSafeTime = false;
}