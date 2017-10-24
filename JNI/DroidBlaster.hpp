#ifndef _PACKT_DROIDBLASTER_HPP_
#define _PACKT_DROIDBLASTER_HPP_

#include "ActivityHandler.hpp"
#include "Asteroid.hpp"
#include "EventLoop.hpp"
#include "GraphicsManager.hpp"
#include "InputManager.hpp"
#include "MoveableBody.hpp"
#include "PhysicsManager.hpp"
#include "Resource.hpp"
#include "Ship.hpp"
#include "SoundManager.hpp"
#include "SpriteBatch.hpp"
#include "StarField.hpp"
#include "Background.hpp"
#include "TimeManager.hpp"
#include "Types.hpp"

class DroidBlaster : public ActivityHandler {
public:
	DroidBlaster(android_app* pApplication);
	void run();
protected:
	status onActivate();
	void onDeactivate();
	status onStep();
	void onStart();
private:
	TimeManager mTimeManager;
	GraphicsManager mGraphicsManager;
	PhysicsManager mPhysicsManager;
	SoundManager mSoundManager;
	InputManager mInputManager;
	EventLoop mEventLoop;
	Resource mAsteroidTexture;
	Resource mShipTexture;
	Resource mBGTexture;
	Resource mStarTexture;
	Resource mBGM;
	Resource mCollisionSound;
	Asteroid mAsteroids;
	Ship mShip;
	Background mBackground;
	StarField mStarField;
	SpriteBatch mSpriteBatch;
	MoveableBody mMoveableBody;
};

#endif