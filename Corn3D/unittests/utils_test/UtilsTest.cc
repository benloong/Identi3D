//
// File: UtilsTest.cc
// ==================
// A unit test for utils project.
//

#include <src/corn3d/Corn3D.h>
#include <src/utils/CPU.h>
#include <src/utils/DebugManager.h>
#include <src/utils/OptionTree.h>
using namespace Corn3D;

#include <iostream>
using namespace std;

void recursive_print_optiontree(OptionElement *root, int tab = 0);
const TCHAR *optiontype_to_string(OptionType type);

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
	OptionElement *graphics, *directx, *opengl, *spec;

	option->addElement(NULL, __T("Graphics"), OptionType_Group, &graphics);
	option->addElement(graphics, __T("DirectX"), OptionType_Group, &directx);
	option->addElement(directx, __T("bool_item1"), OptionType_Bool);
	option->addElement(directx, __T("int_item2"), OptionType_Int, &spec);
	option->addElement(directx, __T("string_item3"), OptionType_String);
	option->addElement(graphics, __T("OpenGL"), OptionType_Group, &opengl);
	option->addElement(opengl, __T("just_another_bool"), OptionType_Bool);
	option->addElement(opengl, __T("just_another_string"), OptionType_String);

	TCHAR location[256], *value;
	HRESULT hr;
	
	hr = option->getElementLocation(spec, location, 256);
	if(FAILED(hr)) {
		cout << "OptionTree: Failed to get element location." << endl;
	} else {
		cout << "Location: ";
		_putts(location);
	}

	hr = option->setKey(__T("Graphics.DirectX.bool_item1"), __T("TRUE"));
	hr |= option->setKey(__T("Graphics.OpenGL.just_another_string"), __T("string_value_here"));
	hr |= option->setKey(__T("Graphics.DirectX.int_item2"), __T("123"));
	if(FAILED(hr)) {
		cout << "OptionTree: Failed to set keys." << endl;
	}
	
	hr = option->getKey(__T("Graphics.OpenGL.just_another_string"), OptionType_String, (void **)&value);
	if(FAILED(hr)) {
		cout << "OptionTree: Failed to get keys." << endl;
	} else {
		_putts(value);
	}

	recursive_print_optiontree(graphics);
	delete option;

	_DebugPrint(dbgmgr, __T("Bye."));

	system("pause");
	delete dbgmgr;
	return 0;
}

void recursive_print_optiontree(OptionElement *root, int tab)
{
	OptionElement *p = root;
	
	while(p->prev) p = p->prev;
	while(p) {
		for(int i = 0; i < tab; i++) _tprintf(__T("\t"));
		_tprintf(__T("%s"), p->name);
		if(p->type == OptionType_Group && p->value != NULL) {
			puts(" [GROUP]");
			recursive_print_optiontree((OptionElement *)p->value, tab + 1);
		} else {
			switch(p->type)
			{
			case OptionType_Bool:
				if(p->value == NULL) _tprintf(__T(" [BOOL] : empty\n"));
				else _tprintf(__T(" [BOOL] : %s\n"), (*((bool *)p->value)) ? __T("true") : __T("false"));
				break;
			case OptionType_String:
				if(p->value == NULL) _tprintf(__T(" [STRING] : empty\n"));
				else _tprintf(__T(" [STRING] : %s\n"), (TCHAR *)p->value);
				break;
			case OptionType_Int:
				if(p->value == NULL) _tprintf(__T(" [INT] : empty\n"));
				else _tprintf(__T(" [INT] : %d\n"), *((int *)p->value));
				break;
			}
		}
		p = p->next;
	}
}