#ifndef __AUDIOENGINMGR_H__
#define __AUDIOENGINMGR_H__

#define MAIN_MENU		"music/MainMenu.mp3"
#define WORLD1			"music/World_1.mp3"
#define WORLD2			"music/World_2.mp3"
#define WORLD3			"music/World_3.mp3"
#define WORLD4			"music/World_4.mp3"
#define WORLD5			"music/World_5.mp3"
#define LOCKED			"music/Locked.mp3"
#define BUTTON			"music/Button.mp3"
#define BACK			"music/Back.mp3"
#define JUMP			"music/Jump.mp3"
#define SECONDS			"music/Seconds.mp3"
#define COINCOLLECT		"music/CoinCollect.mp3"

class AudioEnginMgr
{
public:
    static void initMusic();

	static void playBgMusic(const char* music);
	static void resumeBgMusic();
	static void pauseBgMusic();
	static void stopBgMusic();
	static void playEffect(const char* effect);
	static bool isMusicPlaying();
	static void endAudioEngine();
};
#endif /* __AUDIOENGINMGR_H__ */