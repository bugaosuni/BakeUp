#include "AudioEnginMgr.h"
#include "SimpleAudioEngine.h"
#include "UtilHelper.h"
#include "StringConst.h"

using namespace CocosDenshion;

void AudioEnginMgr::initMusic()
{
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MAIN_MENU);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(WORLD1);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(WORLD2);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(WORLD3);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(WORLD4);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(WORLD5);

    SimpleAudioEngine::getInstance()->preloadEffect(BUTTON);
    SimpleAudioEngine::getInstance()->preloadEffect(BACK);
    SimpleAudioEngine::getInstance()->preloadEffect(LOCKED);
    SimpleAudioEngine::getInstance()->preloadEffect(JUMP);
    SimpleAudioEngine::getInstance()->preloadEffect(SECONDS);
    SimpleAudioEngine::getInstance()->preloadEffect(COINCOLLECT);

    SimpleAudioEngine::getInstance()->setEffectsVolume(1);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
}

void AudioEnginMgr::playBgMusic(const char* music)
{
	if (!UtilHelper::getFromBool(MUSIC_OFF))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(music, true);
	}
}

void AudioEnginMgr::resumeBgMusic()
{
	if (!UtilHelper::getFromBool(MUSIC_OFF))
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
}

void AudioEnginMgr::pauseBgMusic()
{
	if (UtilHelper::getFromBool(MUSIC_OFF))
	{
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}
}

void AudioEnginMgr::stopBgMusic()
{
	if (!UtilHelper::getFromBool(MUSIC_OFF))
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	}
}

void AudioEnginMgr::playEffect(const char* effect)
{
	if (!UtilHelper::getFromBool(SOUND_OFF))
	{
		SimpleAudioEngine::getInstance()->playEffect(effect, false);
	}
}

bool AudioEnginMgr::isMusicPlaying()
{
	return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}

void AudioEnginMgr::endAudioEngine()
{
	SimpleAudioEngine::getInstance()->end();
}