#ifndef _PACKT_EVENTLOOP_HPP_
#define _PACKT_EVENTLOOP_HPP_

#include "ActivityHandler.hpp"
#include "InputHandler.hpp"
#include "../Types.hpp"

class EventLoop {
public:
	EventLoop(ActivityHandler& pActivityHandler, InputHandler& pInputHandler);
	void run();

private:
	void activate();
	void deactivate();

	void processAppEvent(int32_t pCommand);
	int32_t processInputEvent(AInputEvent* pEvent);
	void processSensorEvent();

	static void callback_appEvent(int32_t pCommand);
	static int32_t callback_input(AInputEvent* pEvent);
private:
	bool mEnabled;
	bool mQuit;
	ActivityHandler& mActivityHandler;
	InputHandler& mInputHandler;
};

#endif