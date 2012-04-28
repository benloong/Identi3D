//
// File: SettingsWindow.h
// ======================
// Display a settings window and set up system.
//

#ifndef IDENTI3D_SRC_SYSTEM_SETTINGSWINDOW_H
#define IDENTI3D_SRC_SYSTEM_SETTINGSWINDOW_H

#include <src/identi3d/General.h>

namespace Identi3D
{

	class SettingsWindow
	{
	public:
		static bool show(OptionTree &tree);
		static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
	};

}

#endif // IDENTI3D_SRC_SYSTEM_SETTINGSWINDOW_H