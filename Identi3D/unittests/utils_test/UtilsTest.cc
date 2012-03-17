//
// File: UtilsTest.cc
// ==================
// A unit test for utils project.
//

#include <src/identi3d/Identi3D.h>
#include <src/utils/CPU.h>
#include <src/utils/DebugManager.h>
#include <src/utils/OptionTree.h>
#include <src/utils/SettingsManager.h>
using namespace Identi3D;

#include <iostream>
using namespace std;

int main()
{
	DebugManager *dbgmgr = new DebugManager();

	_DebugPrint(dbgmgr, __T("This is a test message."));
	_DebugPrintV(dbgmgr, __T("This is a verbose message."));
	_DebugPrint(dbgmgr, __T("This is a test message with arguments %.3f."), 2 * 3.14159);

	CpuInfo *cinfo = new CpuInfo();

	_DebugPrint(dbgmgr, __T("CPU INFORMATION:"));
	_DebugPrint(dbgmgr, __T("\tVENDOR ID: %d"), cinfo->getVendor());
	_DebugPrint(dbgmgr, __T("\tSSE SUPPORT: %s"), (cinfo->isSSESupported()) ? __T("YES") : __T("NO"));
	_DebugPrint(dbgmgr, __T("\tSSE2 SUPPORT: %s"), (cinfo->isSSE2Supported()) ? __T("YES") : __T("NO"));
	_DebugPrint(dbgmgr, __T("\tSSE3 SUPPORT: %s"), (cinfo->isSSE3Supported()) ? __T("YES") : __T("NO"));
	_DebugPrint(dbgmgr, __T("\t3DNOW! SUPPORT: %s"), (cinfo->is3DNowSupported()) ? __T("YES") : __T("NO"));
	_DebugPrint(dbgmgr, __T("\tMMX SUPPORT: %s"), (cinfo->isMMXSupported()) ? __T("YES") : __T("NO"));

	delete cinfo;

	OptionTree *option = new OptionTree();
	option->setDebugManager(dbgmgr);

	OptionElement *graphics, *directx, *opengl, *spec;

	graphics = option->addElement(NULL, __T("Graphics"), OPTIONELEMENT_GROUPVALUE);
	if(graphics) {
		directx = option->addElement(graphics, __T("DirectX"), OPTIONELEMENT_GROUPVALUE);
		if(directx) {
			option->addElement(directx, __T("item1"), __T("true"));
			option->addElement(directx, __T("item2"), __T("123"));
			option->addElement(directx, __T("item3"), __T("string"));
		}
		opengl = option->addElement(graphics, __T("OpenGL"), OPTIONELEMENT_GROUPVALUE);
		if(opengl) {
			option->addElement(opengl, __T("item1"), __T("fuckin hell"));
			spec = option->addElement(opengl, __T("item2"), __T("awesome"));
		}
	}

	TCHAR location[256], value[256];
	HRESULT hr;
	
	hr = option->getLocation(spec, location, 256);
	if(FAILED(hr)) {
		cout << "OptionTree: Failed to get element location." << endl;
	} else {
		cout << "Location: ";
		_putts(location);
	}

	hr = option->setValue(__T("Graphics.OpenGL.item2"), __T("string_value_here"));
	if(FAILED(hr)) {
		cout << "OptionTree: Failed to set keys." << endl;
	}
	
	hr = option->getValue(__T("Graphics.OpenGL.item1"), value, 256);
	if(FAILED(hr)) {
		cout << "OptionTree: Failed to get keys." << endl;
	} else {
		_putts(value);
	}

	delete option;

	SettingsManager *conf = new SettingsManager();
	conf->setDebugManager(dbgmgr);
	conf->load(__T("default.conf"));
	conf->save(__T("secondary.conf"));
	delete conf;

	_DebugPrint(dbgmgr, __T("Bye."));

	system("pause");
	delete dbgmgr;
	return 0;
}
