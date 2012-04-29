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

	enum SettingsWindowResult
	{
		SettingsWindowResult_Cancelled		= -1,
		SettingsWindowResult_NoModification	= 0,
		SettingsWindowResult_Modified		= 1,

		SettingsWindowResult_ForceInt		= 0xFFFF
	};

	class SettingsWindow
	{
	public:
		static SettingsWindowResult show(OptionTree &tree);
		static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
	};

}

#endif // IDENTI3D_SRC_SYSTEM_SETTINGSWINDOW_H