//
// File: timer.cc
// ==============
//

#include "timer.h"

using namespace std;

namespace Corn3D
{

	TimerTest::TimerTest(void)
	{
	}

	TimerTest::~TimerTest(void)
	{
	}

	void TimerTest::runTest(unsigned int cycle)
	{
		int k = 0, t;
		unsigned i;

		cout << "TT> Timer test begin." << endl;
		cout << "TT> Testcase 1:";
		t = GetTickCount();
		for(i = 0; i < cycle; i++) {
			k += func1();
		}
		cout << endl << "TT> Tick cost " << GetTickCount() - t << ", factor: " << k << endl;
		cout << "TT> Testcase 2:";
		k = 0;
		t = GetTickCount();
		for(i = 0; i < cycle; i++) {
			k += func2();
		}
		cout << endl << "TT> Tick cost " << GetTickCount() - t << ", factor: " << k << endl;
		cout << endl;
	}

};
