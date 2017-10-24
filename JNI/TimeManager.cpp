#include "Log.hpp"
#include "TimeManager.hpp"
#include <cstdlib>
#include <time.h>

TimeManager::TimeManager() :
	mFirstTime(0.0f),
	mLastTime(0.0f),
	mElapsed(0.0f),
	mElapsedTotal(0.0f) 
{
	srand(time(NULL));
}

void TimeManager::reset() 
{
	Log::info("Resetting TimeManager.");
	mElapsed = 0.0f;
	mFirstTime = now();
	mLastTime = mFirstTime;
}

void TimeManager::update() 
{
	double currentTime = now();
	mElapsed = (currentTime - mLastTime);
	mElapsedTotal = (currentTime - mFirstTime);
	mLastTime = currentTime;
}

double TimeManager::now() 
{
	timespec timeVal;
	clock_gettime(CLOCK_MONOTONIC, &timeVal);

	return timeVal.tv_sec + (timeVal.tv_nsec * 1.0e-9);
}