#include "DroidBlaster.hpp"
#include "Sound.hpp"
#include "Log.hpp"
#include <unistd.h>
#include <string>

static const int32_t SHIP_SIZE_W = 170;
static const int32_t SHIP_SIZE_H = 102;
static const int32_t SHIP_FRAME_1 = 0;
static const int32_t SHIP_FRAME_COUNT = 1;
static const float SHIP_ANIM_SPEED = 8.0f;

static const int32_t ASTEROID_COUNT = 10;
static const int32_t ASTEROID_SIZE = 70;
static const int32_t ASTEROID_FRAME_1 = 0;
static const int32_t ASTEROID_FRAME_COUNT = 16;
static const float ASTEROID_MIN_ANIM_SPEED = 8.0f;
static const float ASTEROID_ANIM_SPEED_RANGE = 16.0f;

static const int32_t STAR_COUNT = 60;

DroidBlaster::DroidBlaster(android_app* pApplication) :
	mTimeManager(),
	mGraphicsManager(pApplication),
	mPhysicsManager(mTimeManager, mGraphicsManager),
	mSoundManager(pApplication),
	mInputManager(pApplication, mGraphicsManager),
	mEventLoop(pApplication, *this, mInputManager),
	mAsteroidTexture(pApplication, "droidblaster/animated_asteroid_0.png"),
	mShipTexture(pApplication, "droidblaster/ship.png"),
	mStarTexture(pApplication, "droidblaster/star.png"),
	mBGTexture(pApplication, "droidblaster/background.png"),
	mBGM(pApplication, "droidblaster/bgm.mp3"),
	mCollisionSound(pApplication, "droidblaster/collision.wav"),
	mAsteroids(pApplication, mTimeManager, mGraphicsManager, mPhysicsManager),
	mShip(pApplication, mGraphicsManager, mSoundManager),
	mBackground(pApplication, mTimeManager, mGraphicsManager, mBGTexture),
	mStarField(pApplication, mTimeManager, mGraphicsManager, STAR_COUNT, mStarTexture),
	mSpriteBatch(mTimeManager, mGraphicsManager),
	mMoveableBody(pApplication, mInputManager, mPhysicsManager)
{
	Log::info("Creating DroidBlaster");
	Sprite* shipGraphics = mSpriteBatch.registerSprite(mShipTexture, SHIP_SIZE_H, SHIP_SIZE_W);
	shipGraphics->setAnimation(SHIP_FRAME_1, SHIP_FRAME_COUNT, SHIP_ANIM_SPEED, false);
	Sound* collisionSound = mSoundManager.registerSound(mCollisionSound);
	b2Body* shipBody = mMoveableBody.registerMoveableBody(shipGraphics->location, SHIP_SIZE_W, SHIP_SIZE_H);
	mShip.registerShip(shipGraphics, collisionSound, shipBody);

	// Creates asteroids.
	for (int32_t i = 0; i < ASTEROID_COUNT; ++i) 
	{
		Sprite* asteroidGraphics = mSpriteBatch.registerSprite(mAsteroidTexture, ASTEROID_SIZE, ASTEROID_SIZE);
		float animSpeed = ASTEROID_MIN_ANIM_SPEED + RAND(ASTEROID_ANIM_SPEED_RANGE);
		asteroidGraphics->setAnimation(ASTEROID_FRAME_1, ASTEROID_FRAME_COUNT, animSpeed, true);
		mAsteroids.registerAsteroid(asteroidGraphics->location, ASTEROID_SIZE, ASTEROID_SIZE);
	}
}

void DroidBlaster::run() 
{
	mEventLoop.run();
}

status DroidBlaster::onActivate() 
{
	Log::info("Activating DroidBlaster");
	// Starts managers.
	if (mGraphicsManager.start() != STATUS_OK) 
		return STATUS_KO;
	if (mSoundManager.start() != STATUS_OK) 
		return STATUS_KO;

	mInputManager.start();
	mPhysicsManager.start();
	mSoundManager.playBGM(mBGM);

	// Initializes game objects.
	mAsteroids.initialize();
	mShip.initialize();
	mMoveableBody.initialize();

	mTimeManager.reset();

	return STATUS_OK;
}

void DroidBlaster::onDeactivate() 
{
	Log::info("Deactivating DroidBlaster");
	mGraphicsManager.stop();
	mSoundManager.stop();
}

status DroidBlaster::onStep() 
{
	mTimeManager.update();
	mPhysicsManager.update();
	mAsteroids.update();
	mMoveableBody.update();
	mShip.update();

	if (mShip.isDestroyed()) 
		return STATUS_EXIT;

	return mGraphicsManager.update();
}

void DroidBlaster::onStart() 
{
	Log::info("onStart");
}