#ifndef _PACKT_SOUND_HPP_
#define _PACKT_SOUND_HPP_

#include "Resource.hpp"
#include "Types.hpp"

class SoundManager;

class Sound {
public:
	Sound(android_app* pApplication, Resource* pResource);
	const char* getPath();
	uint8_t* getBuffer() { return mBuffer; };
	off_t getLength() { return mLength; };
	status load();
	status unload();
private:
	friend class SoundManager;
	Resource* mResource;
	uint8_t* mBuffer; off_t mLength;
};
#endif