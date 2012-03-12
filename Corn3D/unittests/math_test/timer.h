//
// File: timer.h
// =============
// Compare the performance of functions.
//

#ifndef UNITTEST_MATHTEST_TIMER_H
#define UNITTEST_MATHTEST_TIMER_H

#include <src/corn3d/General.h>
#include <iostream>

namespace Corn3D
{

	class TimerTest
	{
	public:
		TimerTest(void);
		virtual ~TimerTest(void);

		void runTest(unsigned int cycle = 1);

		virtual int func1(void) = 0;
		virtual int func2(void) = 0;
	};

};

#endif
