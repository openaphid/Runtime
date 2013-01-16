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

#ifndef OpenAphid_Font_h
#define OpenAphid_Font_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>
#include <runtime/UString.h>

#include "OAConfig.h"

#include "DirectWrapper.h"

namespace Aphid {
	class AJFont;
	
	class Font : public ATF::RefCounted<Font>, public DirectWrapper<AJFont> {
	public:
		virtual ~Font();
		
		static ATF::PassRefPtr<Font> create(const AJ::UString& fontName, float fontSize)
		{
			return ATF::adoptRef(new Font(fontName, fontSize));
		}
		
		static ATF::PassRefPtr<Font> create(float fontSize=18)
		{
			return create(AJ::UString("Arial"), fontSize);
		}
		
		AJ::UString fontName() const {return m_fontName;}
		float fontSize() const {return m_fontSize;}
		
		float fontSizeInPixels() const;
		
	protected:
		Font(const AJ::UString& fontName, float fontSize);
		
		AJ::UString m_fontName;
		float m_fontSize;
		//TODO add more font attributes, may adopt CSS font specifier
	};
}

#endif
