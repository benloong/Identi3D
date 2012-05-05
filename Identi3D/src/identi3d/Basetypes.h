//
// File: Basetypes.h
// =================
// Base types for Identi3D.
//

#ifndef IDENTI3D_SRC_IDENTI3D_BASETYPES_H
#define IDENTI3D_SRC_IDENTI3D_BASETYPES_H

#include <src/identi3d/General.h>
#include <src/math/Basetypes.h>

#if defined (_TARGET_DIRECTX)
#include <d3d9types.h>
#else
struct D3DCOLORVALUE;
typedef DWORD D3DCOLOR;
#endif // defined (_TARGET_DIRECTX)

namespace Identi3D
{

	class Color
	{
	public:

		/*
		 * Constructor: Do nothing.
		 */
		Color(void) : _a(0.0f), _r(0.0f), _g(0.0f), _b(0.0f) {} ;

		/*
		 * Constructor: Set alpha, red, green and blue values.
		 */
		Color(float alpha, float red, float green, float blue) 
			: _a(alpha), _r(red), _g(green), _b(blue) {} ;

		/*
		 * Constructor: Make a copy.
		 */
		Color(const Color &c) { _value = c._value; }

		/*
		 * Destructor: Do nothing.
		 */
		~Color(void) {} ;

		/*
		 * Set color values (ARGB).
		 */
		inline void setValue(float alpha, float red, float green, float blue)
		{
			_r = red, _g = green, _b = blue, _a = alpha;
		}
		
		/*
		 * Set transparency (0.0 - 1.0).
		 */
		inline void setAlpha(float alpha)
		{
			_a = alpha;
		}

		/*
		 * Set red value (0.0 - 1.0).
		 */
		inline void setRed(float red) { _r = red; }

		/*
		 * Set green value (0.0 - 1.0).
		 */
		inline void setGreen(float green) { _g = green; }
		
		/*
		 * Set blue value (0.0 - 1.0).
		 */
		inline void setBlue(float blue) { _b = blue; }
		
		/*
		 * Get transparency.
		 */
		inline float getAlpha(void) const
		{
			return _a;
		}

		/*
		 * Get red value.
		 */
		inline float getRed(void) const
		{
			return _r;
		}

		/*
		 * Get green value.
		 */
		inline float getGreen(void) const 
		{
			return _g;
		}

		/*
		 * Get blue value.
		 */
		inline float getBlue(void) const
		{
			return _b;
		}

		/*
		 * Assignment operator.
		 */
		Color &operator =(const Color &c)
		{
			if(&c != this) _value = c._value;
			return *this;
		}

		/*
		 * Is equal.
		 */
		bool operator==(const Color &c) const
		{
			return (_fequal(_a, c._a) && 
					_fequal(_r, c._r) &&
					_fequal(_g, c._g) &&
					_fequal(_b, c._b));
		}

		/*
		 * Is inequal.
		 */
		bool operator !=(const Color &c) const
		{
			return !((*this) == c);
		}

#if defined (_TARGET_DIRECTX)	// For Direct3D colors.

		/*
		 * Convert to Direct3D color type.
		 */
		inline D3DCOLOR getD3DColor(void) const
		{
			return D3DCOLOR_ARGB(
				(BYTE)(_a * 255.0f), 
				(BYTE)(_r * 255.0f),
				(BYTE)(_g * 255.0f),
				(BYTE)(_b * 255.0f));
		}

		/*
		 * Convert from Direct3D colors.
		 */
		inline void fromD3DColor(const D3DCOLOR c)
		{ 
			_a = (float)((c & 0xFF000000) >> 24) / 255.0f;
			_r = (float)((c & 0xFF0000) >> 16) / 255.0f;
			_g = (float)((c & 0xFF00) >> 8) / 255.0f;
			_b = (float)(c & 0xFF) / 255.0f;
		}

		/*
		 * Convert to Direct3D COLORVALUE type.
		 */
		inline const D3DCOLORVALUE getD3DColorValue(void) const
		{
			D3DCOLORVALUE result = {_r, _g, _b, _a};
			return result;
		}

		/*
		 * Convert from Direct3D COLORVALUE type.
		 */
		inline void fromD3DColorValue(const D3DCOLORVALUE &c)
		{
			_a = c.a;
			_r = c.r;
			_g = c.g;
			_b = c.b;
		}

		/*
		 * D3DCOLOR type cast.
		 */
		operator D3DCOLOR () const
		{
			return getD3DColor();
		}

		/*
		 * D3DCOLORVALUE type cast.
		 */
		operator D3DCOLORVALUE () const
		{
			return getD3DColorValue();
		}

		/*
		 * Assignment operator.
		 */
		Color &operator =(D3DCOLOR c)
		{
			fromD3DColor(c);
			return *this;
		}
		
		/*
		 * Assignment operator.
		 */
		Color &operator =(const D3DCOLORVALUE &c)
		{
			fromD3DColorValue(c);
			return *this;
		}

#endif // defined (_TARGET_DIRECTX)
		
	private:
		_DATA_ALIGN union
		{
			struct
			{
				float _a, _r, _g, _b;
			};
			__m128 _value;
		};

	};

	/*
	 * Specify a clear declaration for keyboard inputs.
	 */
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

#endif // IDENTI3D_SRC_IDENTI3D_BASETYPES_H
