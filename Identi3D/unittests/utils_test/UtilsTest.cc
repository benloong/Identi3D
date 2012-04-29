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
#include <src/utils/ResourceManager.h>
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
	DebugManager *dbgmgr = ntnew DebugManager();
	if(dbgmgr == NULL) return -255;

	dbgmgr->createDebugConsole();
	dbgmgr->setFlag(dbgmgr->getFlag() | DebugFlag_ConsoleOutput);

	// Basic test for debug message output.
	dbgmgr->print(__FILE__, __LINE__, false, "This is a test message.");
	dbgmgr->print(__FILE__, __LINE__, false, "This is a verbose message.");
	dbgmgr->print(__FILE__, __LINE__, false, "This is a test message with arguments %.3f.", 2 * 3.14159);

	dbgmgr->dumpCallStack();

	// Basic test for cpu information.
	dbgmgr->print(__FILE__, __LINE__, false, "CPU INFORMATION:");
	dbgmgr->print(__FILE__, __LINE__, false, "\tVENDOR ID: %d", CpuInfo::instance().getVendor());
	dbgmgr->print(__FILE__, __LINE__, false, "\tSSE SUPPORT: %s", (CpuInfo::instance().isSSESupported()) ? "YES" : "NO");
	dbgmgr->print(__FILE__, __LINE__, false, "\tSSE2 SUPPORT: %s", (CpuInfo::instance().isSSE2Supported()) ? "YES" : "NO");
	dbgmgr->print(__FILE__, __LINE__, false, "\tSSE3 SUPPORT: %s", (CpuInfo::instance().isSSE3Supported()) ? "YES" : "NO");
	dbgmgr->print(__FILE__, __LINE__, false, "\t3DNOW! SUPPORT: %s", (CpuInfo::instance().is3DNowSupported()) ? "YES" : "NO");
	dbgmgr->print(__FILE__, __LINE__, false, "\tMMX SUPPORT: %s", (CpuInfo::instance().isMMXSupported()) ? "YES" : "NO");

	// Basic test for option tree.
	OptionTree *option = ntnew OptionTree(dbgmgr);
	if(option == NULL) return -255;

	OptionElement *graphics, *directx, *opengl;

	graphics = option->addElement(NULL, L"Graphics", OPTIONTREE_GROUPVALUE);
	if(graphics) {
		directx = option->addElement(graphics, L"DirectX", OPTIONTREE_GROUPVALUE);
		if(directx) {
			option->addElement(directx, L"item1", L"true");
			option->addElement(directx, L"item2", L"123");
			option->addElement(directx, L"item3", L"string");
		}
		opengl = option->addElement(graphics, L"OpenGL", OPTIONTREE_GROUPVALUE);
		if(opengl) {
			option->addElement(opengl, L"item1", L"fuckin hell");
			option->addElement(opengl, L"item2", L"awesome");
		}
	}

	option->setValue(L"Graphics.OpenGL.item2", L"awesome+1");
	wcout << L"Graphics.OpenGL.item2 = " << 
		option->getValue(L"Graphics.OpenGL.item2") << endl;
	
	option->addElement(L"Graphics.OpenGL.customed", L"520");
	wcout << L"Graphics.OpenGL.customed = " << 
		option->getValue(L"Graphics.OpenGL.customed") << endl;
	
	OptionElement *p = option->getElement(L"Graphics.DirectX.item1");
	if(p) wcout << p->name << L" = " << p->value << endl;

	option->addElement(L"System.ResourceLocation", L"D:\\testdata;D:\\testdata\\subfolder");
	ResourceManager *resmgr;
	resmgr = ntnew ResourceManager();
	if(resmgr) {
		resmgr->addLocationGroup(*option);
		Resource *res1 = resmgr->load(L"texture.png");
		//Resource *res2 = resmgr->load(L"some_text.txt");
		if(res1) {
			putchar((res1->getData())[1]);
			putchar((res1->getData())[2]);
			putchar((res1->getData())[3]);
			puts("");
		}
		//if(res2) {
		//	for(size_t i = 0; i < res2->getSize(); i++) {
		//		putchar((res2->getData())[i]);
		//	}
		//	puts("");
		//}
		delete res1;
		//delete res2;
		delete resmgr;
	} else {
		puts("Error: Create ResourceManager failed.");
	}

#define UNSIGN	unsigned

				  UNSIGN        CXXN[]
				={0x2A,/**/   /**/43690,
			  32853,1*21845 ,15,169,31597
			 ,1543      *19,+3*      +6473
			 ,324         *5*         17*1
			 ,3*5		              ,+1*
			 (677)					 ,5521
			  *1*11					,3*41
			    *347,			  +3*19
			      *5*67			,4*7*
				    1531,	  18679
				      *(+3)	,0x0F
				        ,1355,2
						 *(9)*
						  269
			
	*1*11,1777*  31,2207*1*13  ,+719*33,     +1*     +1*
	1213         ,29,    1024  *+31   ,672   *59,	 0xF
	,+1*         +00+	 1213  ,0x0  ,309,   49167   ,1*
	1867	     ,13*	 4096  ,0x4D,1*	     29583,  933
	,29*	     2048	 ,154  ,+3* (1)*     +19717, 233
	,245	     *128    ,346  ,+3*  (1)*    +19717,1*58
	,757	     *16*	 2,19  *73,   193*   9*23,0x00E,
	0x97A8,5550  ,0x700F,0x03  ,2*3    *7079 ,149*149*1,
	
	 0xC00F,0x0000,0xEBD3,0x5F0F,0x0000,0x75EB,0xE00F};

		
	char *mod = " ***:*O***.o**'\n";
	UINT pattern = 0;
	for(int i = 0; i < 62; i++) {
		for(int j = 15; j >= 0; j--) {
			pattern = (pattern << 1) + ((CXXN[i] & (1 << j)) >> j);
			if(mod[pattern] == '*') continue;
			putchar(mod[pattern]);
			pattern = 0;
		}
	}

	delete option;

	MyTest *test = ntnew MyTest(dbgmgr);
	if(test == NULL) return -255;
	test->helloworld();
	delete test;

	dbgmgr->print(__FILE__, __LINE__, false, "Bye.");

	system("pause");
	delete dbgmgr;
	return 0;
}
