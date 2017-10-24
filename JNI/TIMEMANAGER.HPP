#ifndef _PACKT_TIMEMANAGER_HPP_
#define _PACKT_TIMEMANAGER_HPP_

#include "../Types.hpp"
#include <ctime>

class TimeManager 
{
public:
	TimeManager();
	void reset();
	void update();
	double now();
	float elapsed() { return mElapsed; };
	float elapsedTotal() { return mElapsedTotal; };
private:
	double mFirstTime;
	double mLastTime;
	float mElapsed;
	float mElapsedTotal;
};

#endif