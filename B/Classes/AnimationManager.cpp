/*************************************************
Copyright:bro7
Author:ben
Date:2014-07-08
Description:游戏动画预加载以及管理
**************************************************/


#include "AnimationManager.h"

USING_NS_CC;

AnimationManager *AnimationManager::_instance = nullptr;


AnimationManager::AnimationManager()
{
    allTypes.push_back(eAniLeftSmall);
    allTypes.push_back(eAniRightSmall);
    allTypes.push_back(eAniSmallDie);
    allTypes.push_back(eAniLeftFire);
    allTypes.push_back(eAniRightFire);
    allTypes.push_back(eAniRotatedFireBall);
    allTypes.push_back(eAniFireActionR);
    allTypes.push_back(eAniFireActionL);
    allTypes.push_back(eAniBoomedFireBall);
    allTypes.push_back(eAniTortoiseLeft);
    allTypes.push_back(eAniTortoiseRight);
    allTypes.push_back(eAniflower);
}

AnimationManager::~AnimationManager()
{
}

AnimationManager * AnimationManager::getInstance()
{
    if ( _instance == nullptr )
    {
        _instance = new AnimationManager();
    }
    return _instance;
}

void AnimationManager::addAllAnimationCache()
{
    addAnimationCache(allTypes);
}

void AnimationManager::addAnimationCache(std::vector<GameAnimationType> vecAniTypes)
{
    for ( auto aniType : vecAniTypes)
    {
        std::string animationName = StringUtils::format("%d", aniType);
        AnimationCache::getInstance()->addAnimation(createAnimation(aniType), animationName);
    }
}

cocos2d::Animation *  AnimationManager::createAnimation(GameAnimationType type)
{
    Vector<SpriteFrame*> vecFrames;
    SpriteFrame * frame = nullptr;
    Texture2D *pTexture = nullptr;
    Animation *pAnimation = nullptr;
    switch (type)
    {

    case eAniLeftSmall:
    {
        pTexture = TextureCache::getInstance()->addImage("image/hero/playerLeft.png");
        for (int i = 1; i < 3; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * i, 60, 80));
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
        break;
    }
    case eAniRightSmall:
    {
        pTexture = TextureCache::getInstance()->addImage("image/hero/playerRight.png");
        for (int i = 1; i < 3; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * i, 60, 80));
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
        break;
    }
    case eAniSmallDie:
    {
        pTexture = TextureCache::getInstance()->addImage("image/hero/playerDied.png");
        for (int i = 0; i < 2; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 0, 60, 80));
            vecFrames.pushBack(frame);
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
        break;
    }
    case eAniLeftFire:
    {
        pTexture = TextureCache::getInstance()->addImage("image/hero/playerLeft.png");
        for (int i = 1; i < 3; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * i, 60, 80));
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
        break;
    }
    case eAniRightFire:
    {
        pTexture = TextureCache::getInstance()->addImage("image/hero/playerRight.png");
        for (int i = 1; i < 3; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * i, 60, 80));
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
        break;
    }
    case eAniFireActionR:
    {
        pTexture = TextureCache::getInstance()->addImage("image/hero/playerRight.png");
        for (int i = 1; i < 3; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * i, 60, 80));
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
        break;
    }
    case eAniFireActionL:
    {
        pTexture = TextureCache::getInstance()->addImage("image/hero/playerLeft.png");
        for (int i = 1; i < 3; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(0, 80 * i, 60, 80));
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
        break;
    }
    case eAniRotatedFireBall:
    {
        pTexture = TextureCache::getInstance()->addImage("image/Tannenzapfen.png");
        for (int i = 0; i < 8; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(40 * i, 0, 40, 40));
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
        break;
    }
    case eAniBoomedFireBall:
    {
        pTexture = TextureCache::getInstance()->addImage("image/Tannenzapfen.png");
        for(int i = 8; i < 12 ; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(40 * i, 0, 40, 40));
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.1f);
        break;
    }
    case eAniTortoiseLeft:
    {
        pTexture = TextureCache::getInstance()->addImage("image/enemy/tortoise0.png");
        for (int i = 1; i < 5; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(80 * i, 0, 80, 80));
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.3f);
        break;
    }
    case eAniTortoiseRight:
    {
        pTexture = TextureCache::getInstance()->addImage("image/enemy/tortoise1.png");
        for (int i = 0; i < 4; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(80 * i, 0, 80, 80));
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.3f);
        break;
    }
    case eAniflower:
    {
        pTexture = TextureCache::getInstance()->addImage("image/enemy/flower0.png");
        for (int i = 0; i < 4; ++i)
        {
            frame = SpriteFrame::createWithTexture(pTexture, CCRectMake(80 * i, 0, 80, 80));
            vecFrames.pushBack(frame);
        }
        pAnimation = Animation::createWithSpriteFrames(vecFrames, 0.3f);
        break;
    }
    default:
        break;
    }

    return pAnimation;
}

cocos2d::Animation* AnimationManager::getAnimation(GameAnimationType key)
{
    std::string aniName = StringUtils::format("%d", key);
    return AnimationCache::getInstance()->getAnimation(aniName);
}

cocos2d::Animate *AnimationManager::createAnimate(GameAnimationType key)
{
    Animation *animation = getAnimation(key);
    if (animation)
    {
        return Animate::create(animation);
    }
    return nullptr;
}