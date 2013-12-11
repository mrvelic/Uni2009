#ifndef _TIMER_H_
#define _TIMER_H_

#include "common.h"

class Timer {
public:
	static double GetCurrentMilli() {
		return (double) (clock());
	}

	static double GetClockDouble() {
		return (double)GetTickCount() / 1000.0f;
	}

};

#endif