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

#include <config.h>
#include <wtf/MathExtras.h>
#include <algorithm>
#include "Color.h"
#include "AJOABinding.h"

namespace Aphid {
	using namespace std;
	using namespace AJ;
	
#include "Color+Data.h"
	
	int colorFloatToByte(float f)
	{
		return max(
							 0, 
							 min(static_cast<int>(lroundf(255.0f * f)), 255)
							 );
	}
	
	RGBA makeRGB(int r, int g, int b)
	{
		return (
						0xFF000000 |
						max(0, min(r, 255)) << 16 | 
						max(0, min(g, 255)) << 8 | 
						max(0, min(b, 255))
						);
	}
	
	RGBA makeRGBA(int r, int g, int b, int a)
	{
		return (
						max(0, min(a, 255)) << 24 | 
						max(0, min(r, 255)) << 16 | 
						max(0, min(g, 255)) << 8 | 
						max(0, min(b, 255))
						);
	}
	
	RGBA makeRGBA(int r, int g, int b, float a)
	{
		return makeRGBA(r, g, b, colorFloatToByte(a));
	}
	
	RGBA colorWithAlpha(RGBA color, int a)
	{
		return (color & 0x00FFFFFF) | (max(0, min(a, 255)) << 24);
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	Color::Color(const AJ::UString& string)
	{
		if (string[0] == '#')
			m_valid = parseHexColor(string.substr(1), m_color);
		else 
			setNamedColor(string);
	}
	
	Color::Color(const char* chars)
	{
		if (chars && strlen(chars) >= 1) {
			if (chars[0] == '#')
				m_valid = parseHexColor(&chars[1], m_color);
			else {
				const NamedColor* foundColor = findColor(chars, strlen(chars));
        m_color = foundColor ? foundColor->RGBValue : 0;
        m_color |= 0xFF000000;
        m_valid = foundColor;
			}
		}
	}
	
	//TODO: improve it to handle '#'
	bool Color::parseHexColor(const AJ::UString &hex, RGBA &color)
	{
		unsigned length = hex.size();
		if (length != 3 && length != 6)
			return false;
    unsigned value = 0;
    for (unsigned i = 0; i < length; ++i) {
			if (!isASCIIHexDigit(hex[i]))
				return false;
			value <<= 4;
			value |= toASCIIHexValue(hex[i]);
    }
    if (length == 6) {
			color = 0xFF000000 | value;
			return true;
    }
    // #abc converts to #aabbcc
    color = 0xFF000000
		| (value & 0xF00) << 12 | (value & 0xF00) << 8
		| (value & 0xF0) << 8 | (value & 0xF0) << 4
		| (value & 0xF) << 4 | (value & 0xF);
    return true;
	}
	
	UString Color::name() const {
		if (hasAlpha())
			return formatUString("#%02X%02X%02X%02X", red(), green(), blue(), alpha());
		else 
			return formatUString("#%02X%02X%02X", red(), green(), blue());
	}
	
	static OA_RELEASE_INLINE const NamedColor* findNamedColor(const AJ::UString& name)
	{
		char buffer[64];
    unsigned length = name.size();
    if (length > sizeof(buffer) - 1)
			return 0;
    for (unsigned i = 0; i < length; ++i) {
			UChar c = name[i];
			if (!c || c > 0x7F)
				return 0;
			buffer[i] = toASCIILower(static_cast<char>(c));
    }
    buffer[length] = '\0';
		return findColor(buffer, length);
	}
	
	void Color::setNamedColor(const AJ::UString &colorName)
	{
		const NamedColor* foundColor = findNamedColor(colorName);
    m_color = foundColor ? foundColor->RGBValue : 0;
    m_color |= 0xFF000000;
    m_valid = foundColor;
	}
	
	Color Color::blend(const Aphid::Color &source) const
	{
		if (!alpha() || !source.hasAlpha())
			return source;
		
    if (!source.alpha())
			return *this;
		
    int d = 255 * (alpha() + source.alpha()) - alpha() * source.alpha();
    int a = d / 255;
    int r = (red() * alpha() * (255 - source.alpha()) + 255 * source.alpha() * source.red()) / d;
    int g = (green() * alpha() * (255 - source.alpha()) + 255 * source.alpha() * source.green()) / d;
    int b = (blue() * alpha() * (255 - source.alpha()) + 255 * source.alpha() * source.blue()) / d;
    return Color(r, g, b, a);
	}
	
}
