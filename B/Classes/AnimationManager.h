#ifndef __ANIMATION_MANAGER_H__
#define __ANIMATION_MANAGER_H__

#include "GameEnum.h"
#include <vector>
#include "cocos2d.h"


class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

private:
	static AnimationManager *_instance;

public:
	static AnimationManager *getInstance();
	void addAllAnimationCache();
	cocos2d::Animate *createAnimate(GameAnimationType key);

private:
	cocos2d::Animation * createAnimation(GameAnimationType);
	void addAnimationCache( std::vector<GameAnimationType> vecAniTypes);
	std::vector < GameAnimationType > allTypes;

	cocos2d::Animation* getAnimation(GameAnimationType key);

};



#endif // !__ANIMATION_MANAGER_H__


