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

#ifndef OpenAphid_TextCodecICU_h
#define OpenAphid_TextCodecICU_h

#include <unicode/utypes.h>
#include "TextCodec.h"
#include "TextEncoding.h"

#include "OAConfig.h"

struct UConverter;

namespace Aphid {
	//
	
	class TextCodecICU : public TextCodec {
	public:
		static void registerBaseEncodingNames(EncodingNameRegistrar);
		static void registerBaseCodecs(TextCodecRegistrar);
		
		TextCodecICU(const TextEncoding&);
		virtual ~TextCodecICU();
		
		virtual Aphid::String decode(const char*, size_t length, bool flush, bool stopOnError, bool& sawError);
		virtual ATF::CString encode(const UChar*, size_t length, UnencodableHandling);
		
	private:
		void createICUConverter() const;
		void releaseICUConverter() const;
		bool needsGBKFallbacks() const { return m_needsGBKFallbacks; }
		void setNeedsGBKFallbacks(bool needsFallbacks) { m_needsGBKFallbacks = needsFallbacks; }
		
		int decodeToBuffer(UChar* buffer, UChar* bufferLimit, const char*& source,
											 const char* sourceLimit, int32_t* offsets, bool flush, UErrorCode& err);
		
		TextEncoding m_encoding;
		unsigned m_numBufferedBytes;
		unsigned char m_bufferedBytes[16]; // bigger than any single multi-byte character
		mutable UConverter* m_converterICU;
		mutable bool m_needsGBKFallbacks;
	};
	
	struct ICUConverterWrapper {
		ICUConverterWrapper()
		: converter(0)
		{
		}
		~ICUConverterWrapper();
		
		UConverter* converter;
	};
	
	PassOwnPtr<TextCodec> newTextCodecICU(const TextEncoding& encoding, const void*);

}

#endif
