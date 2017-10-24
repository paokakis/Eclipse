#include "Log.hpp"
#include "SoundQueue.hpp"

SoundQueue::SoundQueue() :
	mPlayerObj(NULL), mPlayer(NULL),
	mPlayerQueue() 
{}

/*Write initialize(), beginning with SLDataSource and SLDataSink to
describe the input and output channel. Use a SLDataFormat_PCM data format
(instead of SLDataFormat_MIME), which includes sampling, encoding, and
endianness information. Sounds need to be mono (that is, only one sound channel
for both left and right speakers when available). The queue is created with the
Android-specific extension SLDataLocator_AndroidSimpleBufferQueue()*/
status SoundQueue::initialize(SLEngineItf pEngine, SLObjectItf pOutputMixObj) 
{
	Log::info("Starting sound player.");
	SLresult result;
	// Set-up sound audio source.
	SLDataLocator_AndroidSimpleBufferQueue dataLocatorIn;
	dataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
	// At most one buffer in the queue.
	dataLocatorIn.numBuffers = 1;
	SLDataFormat_PCM dataFormat;
	dataFormat.formatType = SL_DATAFORMAT_PCM;
	dataFormat.numChannels = 1; // Mono sound.
	dataFormat.samplesPerSec = SL_SAMPLINGRATE_44_1;
	dataFormat.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
	dataFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
	dataFormat.channelMask = SL_SPEAKER_FRONT_CENTER;
	dataFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;
	SLDataSource dataSource;
	dataSource.pLocator = &dataLocatorIn;
	dataSource.pFormat = &dataFormat;
	SLDataLocator_OutputMix dataLocatorOut;
	dataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
	dataLocatorOut.outputMix = pOutputMixObj;
	SLDataSink dataSink;
	dataSink.pLocator = &dataLocatorOut;
	dataSink.pFormat = NULL;
	/*Then, create and realize the sound player. We are going to need its SL_IID_
	PLAY and SL_IID_BUFFERQUEUE interface, available thanks to the data locator
	configured in the previous step*/
	const SLuint32 soundPlayerIIDCount = 2;
	const SLInterfaceID soundPlayerIIDs[] = { SL_IID_PLAY, SL_IID_BUFFERQUEUE };
	const SLboolean soundPlayerReqs[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
	result = (*pEngine)->CreateAudioPlayer(pEngine, &mPlayerObj, &dataSource, &dataSink, soundPlayerIIDCount, soundPlayerIIDs, soundPlayerReqs);
	if (result != SL_RESULT_SUCCESS) 
		goto ERROR;
	result = (*mPlayerObj)->Realize(mPlayerObj, SL_BOOLEAN_FALSE);
	if (result != SL_RESULT_SUCCESS) 
		goto ERROR;
	result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_PLAY, &mPlayer);
	if (result != SL_RESULT_SUCCESS) 
		goto ERROR;
	result = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_BUFFERQUEUE, &mPlayerQueue);
	if (result != SL_RESULT_SUCCESS) 
		goto ERROR;
	/*Finally, start the queue by setting it in the playing state. This does not actually mean
	that a sound is played. The queue is empty so that would not be possible. However,
	if a sound gets enqueued, it is automatically played*/
	result = (*mPlayer)->SetPlayState(mPlayer, SL_PLAYSTATE_PLAYING);
	if (result != SL_RESULT_SUCCESS) 
		goto ERROR;

	return STATUS_OK;
ERROR:
	Log::error("Error while starting SoundQueue");

	return STATUS_KO;
}

void SoundQueue::finalize() 
{
	Log::info("Stopping SoundQueue.");
	if (mPlayerObj != NULL) 
	{
		(*mPlayerObj)->Destroy(mPlayerObj);
		mPlayerObj = NULL; 
		mPlayer = NULL; 
		mPlayerQueue = NULL;
	}
}

void SoundQueue::playSound(Sound* pSound) 
{
	SLresult result;
	SLuint32 playerState;
	(*mPlayerObj)->GetState(mPlayerObj, &playerState);
	if (playerState == SL_OBJECT_STATE_REALIZED) 
	{
		int16_t* buffer = (int16_t*)pSound->getBuffer();
		off_t length = pSound->getLength();
		// Removes any sound from the queue.
		result = (*mPlayerQueue)->Clear(mPlayerQueue);
		if (result != SL_RESULT_SUCCESS) 
			goto ERROR;
		// Plays the new sound.
		result = (*mPlayerQueue)->Enqueue(mPlayerQueue, buffer, length);
		if (result != SL_RESULT_SUCCESS) 
			goto ERROR;
	}

	return;
ERROR:
	Log::error("Error trying to play sound");
}