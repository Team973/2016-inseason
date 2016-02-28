/*
 * TestJoystickHelper.cpp
 *
 *  Created on: Jan 26, 2016
 *      Author: andrew
 */

#include "TestJoystickHelper.h"
#include "lib/JoystickHelper.h"
#include <vector>
#include "MockTaskMgr.h"

using namespace frc973;

struct StateChangeCall {
	uint32_t port;
	uint32_t button;
	bool state;
	bool operator==(const StateChangeCall &that) {
		return port == that.port && button == that.button && state == that.state;
	}
	void print() {
		printf("port %d, button 0x%x (%d), state %d\n", port, button, button, state);
	}
};

/**
 * Implements JoystickObserver interface and just keeps track of calls.
 */
class TestJSObserver : public JoystickObserver {
public:

	std::vector<StateChangeCall> calls;
	TestJSObserver() : JoystickObserver() {}
	~TestJSObserver() {}

	void ObserveJoystickStateChange(uint32_t port, uint32_t button,
				bool newState) {
		printf("Observing state change %d %d %d\n", port, button, newState);
		calls.push_back(StateChangeCall {port, button, newState});
	}

	bool Contains(const StateChangeCall o) {
		return std::find(calls.begin(), calls.end(), o) != calls.end();
	}
};

/**
 * Show that pushing a button and waiting for a TaskPrePeriodic call will
 * call the observer callback.
 * Show that callbacks are called with the right perameters
 * Show that releasing the buttons has a similar effect.
 */
void TestJoystickHelper::TestButtonEventCallback() {
	AllPublicTaskMgr tasker = AllPublicTaskMgr();
	TestJSObserver obs = TestJSObserver();
	DriverStation ds = DriverStation();

	ObservableJoystick jsa = ObservableJoystick(0, &obs, &tasker, &ds);
	ObservableJoystick jsb = ObservableJoystick(1, &obs, &tasker, &ds);

	tasker.TaskPrePeriodicAll(MODE_DISABLED);
	for (int i = 0; i < 4; i++) {
		obs.calls[i].print();
	}
	ASSERT_EQUAL(0, obs.calls.size());

	printf("setting just button (0,1)\n");
	ds.SetStickButtons_T(0, 0x00000001);
	tasker.TaskPrePeriodicAll(MODE_DISABLED);
	ASSERT_EQUAL(1, obs.calls.size());
	ASSERT(obs.calls[0] == (StateChangeCall {0, 1, true}));
	obs.calls.clear();

	printf("setting just button (1, 2)\n");
	ds.SetStickButtons_T(1, 0x00000002);
	tasker.TaskPrePeriodicAll(MODE_DISABLED);
	ASSERT_EQUAL(1, obs.calls.size());
	ASSERT(obs.calls[0] == (StateChangeCall {1, 2, true}));
	obs.calls.clear();

	printf("unsetting both\n");
	ds.SetStickButtons_T(0, 0x00000000);
	ds.SetStickButtons_T(1, 0x00000000);
	tasker.TaskPrePeriodicAll(MODE_DISABLED);
	ASSERT_EQUAL(2, obs.calls.size());
	ASSERT(obs.Contains(StateChangeCall {0, 1, false}));
	ASSERT(obs.Contains(StateChangeCall {1, 2, false}));
}

/**
 * Show that virtual axises behaive correctly.
 * See it read 0 at start, read 1 on push, read 1 on leave, and read 0 on pull
 */
void TestJoystickHelper::TestVirtualJoystickButtons() {
	DriverStation ds = DriverStation();
	ObservableJoystick js = ObservableJoystick(0, nullptr, nullptr, &ds);

	ASSERTM("Button should start at false", !js.GetLXVirtButton());

	ds.SetStickAxis_T(0, 0, 0.3);
	ASSERTM("Virtual button should not trigger at 0.3", !js.GetLXVirtButton());

	ds.SetStickAxis_T(0, 0, 0.7);
	ASSERTM("Virtual button should trigger by 0.5", js.GetLXVirtButton());

	ds.SetStickAxis_T(0, 0, 0.0);
	ASSERTM("Virtual button should stay pressed until we see -0.5", js.GetLXVirtButton());

	ds.SetStickAxis_T(0, 0, -0.3);
	ASSERTM("Virtual button should stay pressed until we see -0.5", js.GetLXVirtButton());

	ds.SetStickAxis_T(0, 0, -0.7);
	ASSERTM("Virtual button should anti-trigger by -0.5", !js.GetLXVirtButton());
}

cute::suite TestJoystickHelper::MakeSuite(){
	cute::suite s;
	s.push_back(CUTE_SMEMFUN(TestJoystickHelper, TestButtonEventCallback));
	s.push_back(CUTE_SMEMFUN(TestJoystickHelper, TestVirtualJoystickButtons));
	return s;
}
