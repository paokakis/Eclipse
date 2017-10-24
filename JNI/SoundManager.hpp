#ifndef _PACKT_SoundManager_HPP_
#define _PACKT_SoundManager_HPP_

#include "Sound.hpp"
#include "SoundQueue.hpp"
#include "../Types.hpp"
#include <al.h>
#include <alc.h>

class SoundManager {
public:
	SoundManager(android_app* pApplication);
	~SoundManager();
	status start();
	void stop();
	// backGround music
	status playBGM(Resource& pResource);
	void stopBGM();

	Sound* registerSound(Resource& pResource);
	void playSound(Sound* pSound);
private: 
	android_app* mApplication;
	ALObjectItf mEngineObj; SLEngineItf mEngine;
	SLObjectItf mOutputMixObj;

	SLObjectItf mBGMPlayerObj; SLPlayItf mBGMPlayer;
	SLSeekItf mBGMPlayerSeek;

	static const int32_t QUEUE_COUNT = 4;
	SoundQueue mSoundQueues[QUEUE_COUNT]; int32_t mCurrentQueue;
	Sound* mSounds[32]; int32_t mSoundCount;
};

#endif