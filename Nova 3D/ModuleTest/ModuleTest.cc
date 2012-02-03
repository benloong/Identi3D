//
// File: ModuleTest.cc
// ===================
//

#include "stdafx.h"
#include "ModuleTest.h"

#include "App.h"
#include <src/math/Matrix.h>
using namespace Nova3D;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	//int retval;
	HRESULT hr;

	App *my_app;
	my_app = new (std::nothrow) App();
	if(my_app == NULL) return -255;
	
	hr = my_app->init();
	if(FAILED(hr)) {
		delete my_app;
		return -255;
	}

	//retval = my_app->run();
	Matrix m = {1, 0, 0, 0,
				1, 2, 0, 0,
				1, 3, 2, 0,
				2, 4, 1, 2};
	m.inverse(m);
	_DebugPrint(L"\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f\n%.2f\t%.2f\t%.2f\t%.2f", 
		m(0,0), m(0,1), m(0,2), m(0,3),
		m(1,0), m(1,1), m(1,2), m(1,3),
		m(2,0), m(2,1), m(2,2), m(2,3),
		m(3,0), m(3,1), m(3,2), m(3,3));
	delete my_app;
	return 0;
}
