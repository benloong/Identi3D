//
// File: SettingsWindow.cc
// =======================
//

#include <src/system/SettingsWindow.h>
#include <src/ext_resources/resource.h>
#include <src/utils/OptionTree.h>

namespace Identi3D
{

#if defined(_DEBUG)
	const wchar_t *external_resources = L"ext_resources_d.dll";
#else
	const wchar_t *external_resources = L"ext_resources.dll";
#endif // defined(_DEBUG)

	const wchar_t *available_resolution[] = {L"640x480x16@60",
											 L"640x480x32@60",
											 L"800x600x16@60",
											 L"800x600x32@60",
											 L"1024x768x16@60",
											 L"1024x768x32@60",
											 L"1280x1024x16@60",
											 L"1280x1024x32@60",
											 L"1280x720x16@60",
											 L"1280x720x32@60",
											 L"1366x768x16@60",
											 L"1366x768x32@60"};
	const UINT resolution_count = 12;
	const UINT default_resolution_index = 9;	// 1280x720x32@60


	const wchar_t *available_render_plugin[] = {L"Direct3D9",
												L"OpenGL"};
	const UINT render_plugin_count = 2;
	const UINT default_render_plugin_index = 0;

	bool SettingsWindow::show(OptionTree &tree)
	{
		HMODULE hExtRes = LoadLibrary(external_resources);
		if(hExtRes == NULL) {
			MessageBoxA(NULL, E_SYSTEM_LOAD_RESOURCES_FAILURE, "Error", MB_ICONERROR | MB_OK);
			return false;
		}
		if(-1 == DialogBoxParam(
			hExtRes, 
			MAKEINTRESOURCE(IDD_CONFDIALOG), 
			NULL, 
			DlgProc, 
			(LPARAM)&tree)) {
				FreeLibrary(hExtRes);
				return false;
		}
		FreeLibrary(hExtRes);
		return true;
	}

	INT_PTR SettingsWindow::DlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		static OptionTree *tree = NULL;
		static HWND plugin = NULL, adapter = NULL, 
			fscreen = NULL, windowed = NULL, resolution = NULL,
			software = NULL;

		switch(Msg)
		{
		case WM_INITDIALOG:
			tree = (OptionTree *)lParam;

			plugin = GetDlgItem(hDlg, IDC_PLUGIN);
			adapter = GetDlgItem(hDlg, IDC_ADAPTER);
			fscreen = GetDlgItem(hDlg, IDC_FULLSCREEN);
			windowed = GetDlgItem(hDlg, IDC_WINDOWED);
			resolution = GetDlgItem(hDlg, IDC_RESOLUTION);
			software = GetDlgItem(hDlg, IDC_SOFTWARE);

			DWORD ret;
			for(int i = 0; i < render_plugin_count; i++) {
				ret = SendMessage(plugin, CB_ADDSTRING, 0, (LPARAM)available_render_plugin[i]);
			}
			ret = SendMessage(plugin, CB_SETCURSEL, default_render_plugin_index, 0);
			
			SendMessage(adapter, CB_ADDSTRING, 0, (LPARAM)L"Default");
			SendMessage(adapter, CB_ADDSTRING, 0, (LPARAM)L"Secondary (if any)");
			SendMessage(adapter, CB_SETCURSEL, 0, 0);

			SendMessage(fscreen, BM_SETCHECK, 1, 0);

			for(int i = 0; i < resolution_count; i++) {
				ret = SendMessage(resolution, CB_ADDSTRING, 0, (LPARAM)available_resolution[i]);
			}
			SendMessage(resolution, CB_SETCURSEL, default_resolution_index, 0);

			return TRUE;

		case WM_COMMAND:
			switch(wParam)
			{
			case IDOK:
				EndDialog(hDlg, 0);
				return TRUE;
			case IDCANCEL:
				EndDialog(hDlg, -1);
				return TRUE;
			}
			break;
		}

		return FALSE;
	}

}