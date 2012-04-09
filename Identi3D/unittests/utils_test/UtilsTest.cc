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

class MyTest : public DebugFrame
{
public:
	MyTest(DebugManager *debugger) : DebugFrame(debugger) 
	{
		_debugger->printRawString("MyTest class loaded.\n");
	}

	~MyTest(void)
	{
		_debugger->printRawString("MyTest class unloaded.\n");
	}
	
	void helloworld(void)
	{
		clog << "Hello world!" << endl;
	}
};

int main()
{
	DebugManager *dbgmgr = new DebugManager();

	dbgmgr->createDebugConsole();
	dbgmgr->setFlag(dbgmgr->getFlag() | DebugFlag_ConsoleOutput);

	// Basic test for debug message output.
	dbgmgr->print(__FILE__, __LINE__, false, "This is a test message.");
	dbgmgr->print(__FILE__, __LINE__, false, "This is a verbose message.");
	dbgmgr->print(__FILE__, __LINE__, false, "This is a test message with arguments %.3f.", 2 * 3.14159);

	dbgmgr->dumpCallStack();

	// Basic test for cpu information.
	CpuInfo *cinfo = new CpuInfo(dbgmgr);

	dbgmgr->print(__FILE__, __LINE__, false, "CPU INFORMATION:");
	dbgmgr->print(__FILE__, __LINE__, false, "\tVENDOR ID: %d", cinfo->getVendor());
	dbgmgr->print(__FILE__, __LINE__, false, "\tSSE SUPPORT: %s", (cinfo->isSSESupported()) ? "YES" : "NO");
	dbgmgr->print(__FILE__, __LINE__, false, "\tSSE2 SUPPORT: %s", (cinfo->isSSE2Supported()) ? "YES" : "NO");
	dbgmgr->print(__FILE__, __LINE__, false, "\tSSE3 SUPPORT: %s", (cinfo->isSSE3Supported()) ? "YES" : "NO");
	dbgmgr->print(__FILE__, __LINE__, false, "\t3DNOW! SUPPORT: %s", (cinfo->is3DNowSupported()) ? "YES" : "NO");
	dbgmgr->print(__FILE__, __LINE__, false, "\tMMX SUPPORT: %s", (cinfo->isMMXSupported()) ? "YES" : "NO");

	delete cinfo;

	// Basic test for option tree.
	OptionTree *option = new OptionTree(dbgmgr);

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
	wcout << __T("Graphics.OpenGL.item2 = ") << 
		option->getValue(wstring(__T("Graphics.OpenGL.item2"))) << endl;
	
	option->addElement(wstring(__T("Graphics.OpenGL.customed")), wstring(__T("520")));
	wcout << __T("Graphics.OpenGL.customed = ") << 
		option->getValue(wstring(__T("Graphics.OpenGL.customed"))) << endl;
	
	OptionElement *p = option->getElement(wstring(__T("Graphics.DirectX.item1")));
	if(p) wcout << p->name << __T(" = ") << p->value << endl;
	delete option;

	MyTest *test = new MyTest(dbgmgr);
	test->helloworld();
	delete test;
	
	dbgmgr->print(__FILE__, __LINE__, false, "Bye.");

	system("pause");
	delete dbgmgr;
	return 0;
}
