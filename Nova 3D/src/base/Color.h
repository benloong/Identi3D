//
// File: Color.h
// =============
// Color structure for Nova3D.
//

#ifndef NOVA3D_SRC_BASE_COLOR_H
#define NOVA3D_SRC_BASE_COLOR_H

#include <src/nova-3d/NovaGeneral.h>

namespace Nova3D
{

	typedef DWORD D3DCOLOR;
	struct D3DCOLORVALUE;

	class Color
	{
	private:
		__declspec(align(16)) union
		{
			struct
			{
				float _r, _g, _b, _a;
			};
			__m128 _value;
		};

	public:
		Color(void) {} ;
		Color(float red, float green, float blue, float alpha)
			{ setValue(alpha, red, green, blue); }
		virtual ~Color(void) {};

		inline void setValue(float red, float green, float blue, float alpha)
			{ _r = red, _g = green, _b = blue, _a = alpha; }
		inline void setRed(float red)
			{ _r = red; }
		inline void setGreen(float green)
			{ _g = green; }
		inline void setBlue(float blue)
			{ _b = blue; }
		inline void setAlpha(float alpha)
			{ _a = alpha; }

		inline float getRed(void) const
			{ return _r; }
		inline float getGreen(void) const
			{ return _g; }
		inline float getBlue(void) const
			{ return _b; }
		inline float getAlpha(void) const
			{ return _a; }

		Color &operator =(Color &c)
			{ _value = c._value; return *this; }

#if defined(_TARGET_D3D_VERSION)
		inline D3DCOLOR getD3DColor(void) const
			{ return D3DCOLOR_COLORVALUE(_r, _g, _b, _a); }
		inline void fromD3DColor(D3DCOLOR c)
			{ _a = (c & 0xFF000000) >> 24, _r = (c & 0xFF0000) >> 16, _g = (c & 0xFF00) >> 8, _b = c & 0xFF; }
		inline const D3DCOLORVALUE &getD3DColorValue(void) const
			{ return reinterpret_cast<D3DCOLORVALUE>(*this); }
		inline void fromD3DColorValue(const D3DCOLORVALUE &c)
			{ _r = c.r, _g = c.g, _b = c.b, _a = c.a; }

		operator D3DCOLOR () const
			{ return getD3DColor(); }
		operator const D3DCOLORVALUE &() const
			{ return getD3DColorValue(); }
		Color &operator =(D3DCOLOR c)
			{ fromD3DColor(c); return *this; }
		Color &operator =(const D3DCOLORVALUE &c)
			{ fromD3DColorValue(c); return *this; }
#endif

	};

};

#endif
