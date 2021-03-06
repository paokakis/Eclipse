#ifndef PACKT_PHYSICSMANAGER_HPP
#define PACKT_PHYSICSMANAGER_HPP

#include "GraphicsManager.hpp"
#include "TimeManager.hpp"
#include "../Types.hpp"
#include <vector>

#define PHYSICS_SCALE 32.0f

struct PhysicsCollision {
	bool collide;
	PhysicsCollision() : collide(false) {}
};

class PhysicsManager : private b2ContactListener {
public:
	PhysicsManager(TimeManager& pTimeManager, GraphicsManager& pGraphicsManager);
	~PhysicsManager();
	b2Body* loadBody(sLocation& pLocation, uint16 pCategory, uint16 pMask, int32_t pSizeX, int32_t pSizeY, float pRestitution);
	b2MouseJoint* loadTarget(b2Body* pBodyObj);
	void start();
	void update();
private:
	TimeManager& mTimeManager;
	GraphicsManager& mGraphicsManager;

	void BeginContact(b2Contact* pContact);
	b2World mWorld;
	std::vector<b2Body*> mBodies;
	std::vector<sLocation*> mLocations;
	b2Body* mBoundsBodyObj;
};
#endif