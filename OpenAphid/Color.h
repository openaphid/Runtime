/*
Copyright 2012 Aphid Mobile

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
 
   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

 */

#ifndef OpenAphid_Color_h
#define OpenAphid_Color_h

#include <wtf/FastAllocBase.h>
#include <runtime/UString.h>

#include "OAConfig.h"

#include "ccTypes.h"

namespace Aphid {
	typedef unsigned RGBA;
	
	//TODO: make these methods inline
	
	int colorFloatToByte(float f);
	
	RGBA makeRGB(int r, int g, int b);
	
	RGBA makeRGBA(int r, int g, int b, int a);
	
	RGBA makeRGBA(int r, int g, int b, float a);
	
	RGBA colorWithAlpha(RGBA color, int a);
	
	class WebColors : public Noncopyable {
	public:
		static const RGBA black = 0xFF000000;
    static const RGBA white = 0xFFFFFFFF;
    static const RGBA darkGray = 0xFF808080;
    static const RGBA gray = 0xFFA0A0A0;
    static const RGBA lightGray = 0xFFC0C0C0;
    static const RGBA transparent = 0x00000000;
    static const RGBA tap = 0x4D1A1A1A;
    static const RGBA compositionFill = 0x3CAFC0E3;
    static const RGBA compositionFrame = 0x3C4D80B4;
    static const RGBA cyan = 0xFF00FFFF;
		
		static const RGBA red = 0xff0000;
		static const RGBA darkred = 0x8b0000;
		static const RGBA tan = 0xd2b48c;
		static const RGBA linen = 0xfaf0e6;
		static const RGBA sienna = 0xa0522d;
		static const RGBA indianred = 0xcd5c5c;
		static const RGBA teal = 0x008080;
		static const RGBA grey = 0x808080;
		static const RGBA green = 0x008000;
		static const RGBA darkgrey = 0xa9a9a9;
		static const RGBA darkgreen = 0x006400;
		static const RGBA beige = 0xf5f5dc;
		static const RGBA orange = 0xffa500;
		static const RGBA darkgray = 0xa9a9a9;
		static const RGBA orangered = 0xff4500;
		static const RGBA khaki = 0xf0e68c;
		static const RGBA seagreen = 0x2e8b57;
		static const RGBA gold = 0xffd700;
		static const RGBA darkorange = 0xff8c00;
		static const RGBA darkkhaki = 0xbdb76b;
		static const RGBA indigo = 0x4b0082;
		static const RGBA goldenrod = 0xdaa520;
		static const RGBA maroon = 0x800000;
		static const RGBA gainsboro = 0xdcdcdc;
		static const RGBA lime = 0x00ff00;
		static const RGBA greenyellow = 0xadff2f;
		static const RGBA darkgoldenrod = 0xb8860b;
		static const RGBA slategrey = 0x708090;
		static const RGBA slategray = 0x708090;
		static const RGBA salmon = 0xfa8072;
		static const RGBA darkseagreen = 0x8fbc8f;
		static const RGBA seashell = 0xfff5ee;
		static const RGBA darksalmon = 0xe9967a;
		static const RGBA tomato = 0xff6347;
		static const RGBA thistle = 0xd8bfd8;
		static const RGBA darkslategrey = 0x2f4f4f;
		static const RGBA forestgreen = 0x228b22;
		static const RGBA dimgrey = 0x696969;
		static const RGBA darkslategray = 0x2f4f4f;
		static const RGBA mistyrose = 0xffe4e1;
		static const RGBA dimgray = 0x696969;
		static const RGBA darkcyan = 0x008b8b;
		static const RGBA magenta = 0xff00ff;
		static const RGBA limegreen = 0x32cd32;
		static const RGBA coral = 0xff7f50;
		static const RGBA darkmagenta = 0x8b008b;
		static const RGBA azure = 0xf0ffff;
		static const RGBA blue = 0x0000ff;
		static const RGBA oldlace = 0xfdf5e6;
		static const RGBA cornsilk = 0xfff8dc;
		static const RGBA darkblue = 0x00008b;
		static const RGBA skyblue = 0x87ceeb;
		static const RGBA firebrick = 0xb22222;
		static const RGBA orchid = 0xda70d6;
		static const RGBA lightgrey = 0xd3d3d3;
		static const RGBA lightgreen = 0x90ee90;
		static const RGBA lightyellow = 0xffffe0;
		static const RGBA lightgray = 0xd3d3d3;
		static const RGBA darkorchid = 0x9932cc;
		static const RGBA royalblue = 0x4169e1;
		static const RGBA aqua = 0x00ffff;
		static const RGBA steelblue = 0x4682b4;
		static const RGBA bisque = 0xffe4c4;
		static const RGBA crimson = 0xdc143c;
		static const RGBA slateblue = 0x6a5acd;
		static const RGBA dodgerblue = 0x1e90ff;
		static const RGBA blanchedalmond = 0xffebcd;
		static const RGBA lightseagreen = 0x20b2aa;
		static const RGBA lightslategrey = 0x778899;
		static const RGBA lightslategray = 0x778899;
		static const RGBA brown = 0xa52a2a;
		static const RGBA lightsalmon = 0xffa07a;
		static const RGBA snow = 0xfffafa;
		static const RGBA lightcyan = 0xe0ffff;
		static const RGBA rosybrown = 0xbc8f8f;
		static const RGBA sandybrown = 0xf4a460;
		static const RGBA darkslateblue = 0x483d8b;
		static const RGBA yellow = 0xffff00;
		static const RGBA lightcoral = 0xf08080;
		static const RGBA mintcream = 0xf5fffa;
		static const RGBA aquamarine = 0x7fffd4;
		static const RGBA saddlebrown = 0x8b4513;
		static const RGBA honeydew = 0xf0fff0;
		static const RGBA pink = 0xffc0cb;
		static const RGBA lightblue = 0xadd8e6;
		static const RGBA cadetblue = 0x5f9ea0;
		static const RGBA wheat = 0xf5deb3;
		static const RGBA lawngreen = 0x7cfc00;
		static const RGBA aliceblue = 0xf0f8ff;
		static const RGBA chocolate = 0xd2691e;
		static const RGBA yellowgreen = 0x9acd32;
		static const RGBA moccasin = 0xffe4b5;
		static const RGBA navy = 0x000080;
		static const RGBA chartreuse = 0x7fff00;
		static const RGBA ivory = 0xfffff0;
		static const RGBA palegreen = 0x98fb98;
		static const RGBA lavender = 0xe6e6fa;
		static const RGBA hotpink = 0xff69b4;
		static const RGBA olive = 0x808000;
		static const RGBA fuchsia = 0xff00ff;
		static const RGBA mediumseagreen = 0x3cb371;
		static const RGBA silver = 0xc0c0c0;
		static const RGBA olivedrab = 0x6b8e23;
		static const RGBA darkturquoise = 0x00ced1;
		static const RGBA turquoise = 0x40e0d0;
		static const RGBA violet = 0xee82ee;
		static const RGBA violetred = 0xd02090;
		static const RGBA darkviolet = 0x9400d3;
		static const RGBA palegoldenrod = 0xeee8aa;
		static const RGBA whitesmoke = 0xf5f5f5;
		static const RGBA springgreen = 0x00ff7f;
		static const RGBA burlywood = 0xdeb887;
		static const RGBA peru = 0xcd853f;
		static const RGBA floralwhite = 0xfffaf0;
		static const RGBA lightpink = 0xffb6c1;
		static const RGBA darkolivegreen = 0x556b2f;
		static const RGBA ghostwhite = 0xf8f8ff;
		static const RGBA mediumblue = 0x0000cd;
		static const RGBA mediumorchid = 0xba55d3;
		static const RGBA lightsteelblue = 0xb0c4de;
		static const RGBA lightslateblue = 0x8470ff;
		static const RGBA deepskyblue = 0x00bfff;
		static const RGBA lightskyblue = 0x87cefa;
		static const RGBA lightgoldenrodyellow = 0xfafad2;
		static const RGBA plum = 0xdda0dd;
		static const RGBA mediumaquamarine = 0x66cdaa;
		static const RGBA mediumslateblue = 0x7b68ee;
		static const RGBA blueviolet = 0x8a2be2;
		static const RGBA midnightblue = 0x191970;
		static const RGBA deeppink = 0xff1493;
		static const RGBA lemonchiffon = 0xfffacd;
		static const RGBA antiquewhite = 0xfaebd7;
		static const RGBA paleturquoise = 0xafeeee;
		static const RGBA powderblue = 0xb0e0e6;
		static const RGBA navajowhite = 0xffdead;
		static const RGBA mediumspringgreen = 0x00fa9a;
		static const RGBA cornflowerblue = 0x6495ed;
		static const RGBA palevioletred = 0xd87093;
		static const RGBA mediumvioletred = 0xc71585;
		static const RGBA purple = 0x800080;
		static const RGBA lavenderblush = 0xfff0f5;
		static const RGBA mediumturquoise = 0x48d1cc;
		static const RGBA peachpuff = 0xffdab9;
		static const RGBA mediumpurple = 0x9370d8;
		static const RGBA papayawhip = 0xffefd5;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class Color : public ATF::FastAllocBase {
	public:
		static bool parseHexColor(const AJ::UString& hex, RGBA& color);
		
		Color()
		: m_color(0)
		, m_valid(false)
		{}
		
		Color(RGBA col)
		: m_color(col)
		, m_valid(true)
		{}
		
		Color(int r, int g, int b)
		: m_color(makeRGB(r, g, b))
		, m_valid(true)
		{}
		
		Color(int r, int g, int b, int a)
		: m_color(makeRGBA(r, g, b, a))
		, m_valid(true)
		{}
		
		Color(int r, int g, int b, float a)
		: m_color(makeRGBA(r, g, b, a))
		, m_valid(true)
		{}
		
		Color(const ccColor3B& c)
		: m_color(makeRGB(c.r, c.g, c.b))
		, m_valid(true)
		{
		}
		
		Color(const ccColor3B& c, GLubyte opacity)
		: m_color(makeRGBA(c.r, c.g, c.b, opacity))
		, m_valid(true)
		{
		}
		
		explicit Color(const AJ::UString& string);
		explicit Color(const char* chars);
		
		bool isValid() const {return m_valid;}
		RGBA rgba() const {return m_color;}
		void setRGBA(RGBA color) {m_color = color; m_valid = true;}
		
		int red() const { return (m_color >> 16) & 0xFF; }
    int green() const { return (m_color >> 8) & 0xFF; }
    int blue() const { return m_color & 0xFF; }
    int alpha() const { return (m_color >> 24) & 0xFF;}
		bool hasAlpha() const {return alpha() < 255;}
		
		static int red(RGBA color) { return (color >> 16) & 0xFF; }
		static int green(RGBA color) { return (color >> 8) & 0xFF; }
		static int blue(RGBA color) { return color & 0xFF; }
		static int alpha(RGBA color) { return (color >> 24) & 0xFF;}
		
		AJ::UString name() const;
		void setNamedColor(const AJ::UString& colorString);
		
		Color blend(const Color& color) const;
		
	private:
		RGBA m_color;
		bool m_valid;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	/// Overrides operators
	inline bool operator==(const Color& c1, const Color& c2)
	{
		return c1.rgba() == c2.rgba() && c1.isValid() == c2.isValid();
	}
	
	inline bool operator!=(const Color& c1, const Color& c2)
	{
		return !(c1==c2);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// Conversion methods
	static ccColor3B toCCColor3B(RGBA color) 
	{
		ccColor3B c = {
			(color >> 16) & 0xFF, 
			(color >> 8) & 0xFF,
			color & 0xFF
		};
		return c;
	}
	
	static ccColor4B toColor4B(RGBA color)
	{
		ccColor4B c = {
			(color >> 16) & 0xFF, 
			(color >> 8) & 0xFF,
			color & 0xFF,
			(color >> 24) & 0xFF
		};
		return c;
	}
	
	static ccColor4F toColor4F(RGBA color)
	{
		ccColor4F c = {
			(color >> 16) & 0xFF, 
			(color >> 8) & 0xFF,
			color & 0xFF,
			(color >> 24) & 0xFF
		};
		return c;
	}
	
	static RGBA toRGBA(const ccColor3B& color, GLubyte opacity=255)
	{
		return makeRGBA(color.r, color.g, color.b, opacity);
	}
	
	static RGBA toRGBA(const ccColor4B& color)
	{
		return makeRGBA(color.r, color.g, color.b, color.a);
	}
}

#endif
