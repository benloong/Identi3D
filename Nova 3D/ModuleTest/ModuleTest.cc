//
// File: ModuleTest.cc
// ===================
//

#include "stdafx.h"
#include "ModuleTest.h"

#include "App.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	int retval;
	HRESULT hr;

	App *my_app;
	my_app = new (std::nothrow) App();
	if(my_app == NULL) return -255;
	
	hr = my_app->init();
	if(FAILED(hr)) {
		delete my_app;
		return -255;
	}

	retval = my_app->run();
	delete my_app;
	return retval;
}
