//
// File: KeyDefinition.h
// =====================
// Redefine virtual keys.
//
// Copyright (c) Sparkfire, 2011 - 2012
//

#ifndef NOVA3D_UTILS_KEYDEFINITION_H
#define NOVA3D_UTILS_KEYDEFINITION_H

#include "src/nova-3d/NovaGeneral.h"

namespace Nova3D
{

	enum KeyType
	{
		KeyType_LeftMouseButton		= VK_LBUTTON,
		KeyType_RightMouseButton	= VK_RBUTTON,
		KeyType_MiddleMouseButton	= VK_MBUTTON,
		KeyType_X1MouseButton		= VK_XBUTTON1,
		KeyType_X2MouseButton		= VK_XBUTTON2,

		KeyType_ControlC			= VK_CANCEL,
		KeyType_Backspace			= VK_BACK,
		KeyType_Tab					= VK_TAB,
		KeyType_Enter				= VK_RETURN,
		KeyType_Shift				= VK_SHIFT,
		KeyType_Control				= VK_CONTROL,
		KeyType_Alt					= VK_MENU,
		KeyType_Pause				= VK_PAUSE,
		KeyType_CapsLock			= VK_CAPITAL,
		KeyType_Escape				= VK_ESCAPE,
		KeyType_Space				= VK_SPACE,
		KeyType_PageUp				= VK_PRIOR,
		KeyType_PageDown			= VK_NEXT,
		KeyType_End					= VK_END,
		KeyType_Home				= VK_HOME,
		KeyType_Left				= VK_LEFT,
		KeyType_Right				= VK_RIGHT,
		KeyType_Up					= VK_UP,
		KeyType_Down				= VK_DOWN,
		KeyType_PrintScreen			= VK_SNAPSHOT,
		KeyType_Insert				= VK_INSERT,
		KeyType_Delete				= VK_DELETE,
		KeyType_LeftWinKey			= VK_LWIN,
		KeyType_RightWinKey			= VK_RWIN,
		KeyType_Sleep				= VK_SLEEP,
		KeyType_ScrollLock			= VK_SCROLL,
		KeyType_NumLock				= VK_NUMLOCK,

		KeyType_0					= 0x30,
		KeyType_1,
		KeyType_2,
		KeyType_3,
		KeyType_4,
		KeyType_5,
		KeyType_6,
		KeyType_7,
		KeyType_8,
		KeyType_9,

		KeyType_A					= 0x41,
		KeyType_B,
		KeyType_C,
		KeyType_D,
		KeyType_E,
		KeyType_F,
		KeyType_G,
		KeyType_H,
		KeyType_I,
		KeyType_J,
		KeyType_K,
		KeyType_L,
		KeyType_M,
		KeyType_N,
		KeyType_O,
		KeyType_P,
		KeyType_Q,
		KeyType_R,
		KeyType_S,
		KeyType_T,
		KeyType_U,
		KeyType_V,
		KeyType_W,
		KeyType_X,
		KeyType_Y,
		KeyType_Z,

		KeyType_Numpad0				= VK_NUMPAD0,
		KeyType_Numpad1				= VK_NUMPAD1,
		KeyType_Numpad2				= VK_NUMPAD2,
		KeyType_Numpad3				= VK_NUMPAD3,
		KeyType_Numpad4				= VK_NUMPAD4,
		KeyType_Numpad5				= VK_NUMPAD5,
		KeyType_Numpad6				= VK_NUMPAD6,
		KeyType_Numpad7				= VK_NUMPAD7,
		KeyType_Numpad8				= VK_NUMPAD8,
		KeyType_Numpad9				= VK_NUMPAD9,
		
		KeyType_NumpadAdd			= VK_ADD,
		KeyType_NumpadSubtract		= VK_SUBTRACT,
		KeyType_NumpadMultiply		= VK_MULTIPLY,
		KeyType_NumpadDivide		= VK_DIVIDE,
		KeyType_NumpadDecimal		= VK_DECIMAL,

		KeyType_F1					= VK_F1,
		KeyType_F2					= VK_F2,
		KeyType_F3					= VK_F3,
		KeyType_F4					= VK_F4,
		KeyType_F5					= VK_F5,
		KeyType_F6					= VK_F6,
		KeyType_F7					= VK_F7,
		KeyType_F8					= VK_F8,
		KeyType_F9					= VK_F9,
		KeyType_F10					= VK_F10,
		KeyType_F11					= VK_F11,
		KeyType_F12					= VK_F12,

		KeyType_ForceUintPtr		= 0xFFFF
	};

};

#endif
