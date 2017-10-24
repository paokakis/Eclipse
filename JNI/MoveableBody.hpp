#ifndef _PACKT_MOVEABLEBODY_HPP_
#define _PACKT_MOVEABLEBODY_HPP_

#include "PhysicsManager.hpp"
#include "../Types.hpp"

class MoveableBody {
public:
	MoveableBody(PhysicsManager& pPhysicsManager);
	b2Body* registerMoveableBody(sLocation& pLocation, int32_t pSizeX, int32_t pSizeY);
	void initialize();
	void update();
private:
	PhysicsManager& mPhysicsManager;
	
	b2Body* mBody;
	b2MouseJoint* mTarget;
};
#endif