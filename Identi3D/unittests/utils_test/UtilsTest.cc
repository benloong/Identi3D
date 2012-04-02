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

	dbgmgr->createDebugConsole();
	dbgmgr->setFlag(dbgmgr->getFlag() | DebugFlag_ConsoleOutput);

	// Basic test for debug message output.
	_DebugPrint(dbgmgr, "This is a test message.");
	_DebugPrintV(dbgmgr, "This is a verbose message.");
	_DebugPrint(dbgmgr, "This is a test message with arguments %.3f.", 2 * 3.14159);

	dbgmgr->dumpCallStack();

	// Basic test for cpu information.
	CpuInfo *cinfo = new CpuInfo();

	_DebugPrint(dbgmgr, "CPU INFORMATION:");
	_DebugPrint(dbgmgr, "\tVENDOR ID: %d", cinfo->getVendor());
	_DebugPrint(dbgmgr, "\tSSE SUPPORT: %s", (cinfo->isSSESupported()) ? "YES" : "NO");
	_DebugPrint(dbgmgr, "\tSSE2 SUPPORT: %s", (cinfo->isSSE2Supported()) ? "YES" : "NO");
	_DebugPrint(dbgmgr, "\tSSE3 SUPPORT: %s", (cinfo->isSSE3Supported()) ? "YES" : "NO");
	_DebugPrint(dbgmgr, "\t3DNOW! SUPPORT: %s", (cinfo->is3DNowSupported()) ? "YES" : "NO");
	_DebugPrint(dbgmgr, "\tMMX SUPPORT: %s", (cinfo->isMMXSupported()) ? "YES" : "NO");

	delete cinfo;

	// Basic test for option tree.
	OptionTree *option = new OptionTree();
	option->setDebugManager(dbgmgr);

	OptionElement *graphics, *directx, *opengl;

	graphics = option->addElement(NULL, wstring(__T("Graphics")), OPTIONTREE_GROUPVALUE);
	if(graphics) {
		directx = option->addElement(graphics, wstring(__T("DirectX")), OPTIONTREE_GROUPVALUE);
		if(directx) {
			option->addElement(directx, wstring(__T("item1")), wstring(__T("true")));
			option->addElement(directx, wstring(__T("item2")), wstring(__T("123")));
			option->addElement(directx, wstring(__T("item3")), wstring(__T("string")));
		}
		opengl = option->addElement(graphics, wstring(__T("OpenGL")), OPTIONTREE_GROUPVALUE);
		if(opengl) {
			option->addElement(opengl, wstring(__T("item1")), wstring(__T("fuckin hell")));
			option->addElement(opengl, wstring(__T("item2")), wstring(__T("awesome")));
		}
	}

	option->setValue(wstring(__T("Graphics.OpenGL.item2")), wstring(__T("awesome+1")));
	wcout << __T("Graphics.OpenGL.item2 = ") << option->getValue(wstring(__T("Graphics.OpenGL.item2")));
	
	option->addElement(wstring(__T("Graphics.OpenGL.customed")), wstring(__T("520")));
	wcout << __T("Graphics.OpenGL.customed = ") << option->getValue(wstring(__T("Graphics.OpenGL.customed")));
	
	OptionElement *p = option->getElement(wstring(__T("Graphics.DirectX.item1")));
	if(p) wcout << p->name << __T(" = ") << p->value << endl;
	delete option;

	_DebugPrint(dbgmgr, ("Bye."));

	system("pause");
	delete dbgmgr;
	return 0;
}
