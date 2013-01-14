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
#include "TextEncoding.h"

#include <unicode/unorm.h>
#include <wtf/text/CString.h>
#include <wtf/HashSet.h>
#include <wtf/OwnPtr.h>
#include <wtf/StdLibExtras.h>

#include "TextCodecICU.h"

namespace Aphid {
	using Aphid::String;
	
	static void addEncodingName(HashSet<const char*>& set, const char* name)
	{
    const char* atomicName = atomicCanonicalTextEncodingName(name);
    if (atomicName)
			set.add(atomicName);
	}
	
	TextEncoding::TextEncoding(const char* name)
	: m_name(atomicCanonicalTextEncodingName(name))
	, m_backslashAsCurrencySymbol(backslashAsCurrencySymbol())
	{
	}
	
	TextEncoding::TextEncoding(const String& name)
	: m_name(atomicCanonicalTextEncodingName(name.characters(), name.length()))
	, m_backslashAsCurrencySymbol(backslashAsCurrencySymbol())
	{
	}
	
	String TextEncoding::decode(const char* data, size_t length, bool stopOnError, bool& sawError) const
	{
    if (!m_name)
			return String();
		
    return newTextCodec(*this)->decode(data, length, true, stopOnError, sawError);
	}
	
	CString TextEncoding::encode(const UChar* characters, size_t length, UnencodableHandling handling) const
	{
    if (!m_name)
			return CString();
		
    if (!length)
			return "";
		
    // FIXME: What's the right place to do normalization?
    // It's a little strange to do it inside the encode function.
    // Perhaps normalization should be an explicit step done before calling encode.
		
    const UChar* source = characters;
    size_t sourceLength = length;
		
    Vector<UChar> normalizedCharacters;
		
    UErrorCode err = U_ZERO_ERROR;
    if (unorm_quickCheck(source, sourceLength, UNORM_NFC, &err) != UNORM_YES) {
			// First try using the length of the original string, since normalization to NFC rarely increases length.
			normalizedCharacters.grow(sourceLength);
			int32_t normalizedLength = unorm_normalize(source, length, UNORM_NFC, 0, normalizedCharacters.data(), length, &err);
			if (err == U_BUFFER_OVERFLOW_ERROR) {
				err = U_ZERO_ERROR;
				normalizedCharacters.resize(normalizedLength);
				normalizedLength = unorm_normalize(source, length, UNORM_NFC, 0, normalizedCharacters.data(), normalizedLength, &err);
			}
			ASSERT(U_SUCCESS(err));
			
			source = normalizedCharacters.data();
			sourceLength = normalizedLength;
    }
    return newTextCodec(*this)->encode(source, sourceLength, handling);
	}
	
	const char* TextEncoding::domName() const
	{
    if (noExtendedTextEncodingNameUsed())
			return m_name;
		
    // We treat EUC-KR as windows-949 (its superset), but need to expose 
    // the name 'EUC-KR' because the name 'windows-949' is not recognized by
    // most Korean web servers even though they do use the encoding
    // 'windows-949' with the name 'EUC-KR'. 
    // FIXME: This is not thread-safe. At the moment, this function is
    // only accessed in a single thread, but eventually has to be made
    // thread-safe along with usesVisualOrdering().
    static const char* const a = atomicCanonicalTextEncodingName("windows-949");
    if (m_name == a)
			return "EUC-KR";
    return m_name;
	}
	
	bool TextEncoding::usesVisualOrdering() const
	{
    if (noExtendedTextEncodingNameUsed())
			return false;
		
    static const char* const a = atomicCanonicalTextEncodingName("ISO-8859-8");
    return m_name == a;
	}
	
	bool TextEncoding::isJapanese() const
	{
    if (noExtendedTextEncodingNameUsed())
			return false;
		
    DEFINE_STATIC_LOCAL(HashSet<const char*>, set, ());
    if (set.isEmpty()) {
			addEncodingName(set, "x-mac-japanese");
			addEncodingName(set, "cp932");
			addEncodingName(set, "JIS_X0201");
			addEncodingName(set, "JIS_X0208-1983");
			addEncodingName(set, "JIS_X0208-1990");
			addEncodingName(set, "JIS_X0212-1990");
			addEncodingName(set, "JIS_C6226-1978");
			addEncodingName(set, "Shift_JIS_X0213-2000");
			addEncodingName(set, "ISO-2022-JP");
			addEncodingName(set, "ISO-2022-JP-2");
			addEncodingName(set, "ISO-2022-JP-1");
			addEncodingName(set, "ISO-2022-JP-3");
			addEncodingName(set, "EUC-JP");
			addEncodingName(set, "Shift_JIS");
    }
    return m_name && set.contains(m_name);
	}
	
	UChar TextEncoding::backslashAsCurrencySymbol() const
	{
    if (noExtendedTextEncodingNameUsed())
			return '\\';
		
    // The text encodings below treat backslash as a currency symbol.
    // See http://blogs.msdn.com/michkap/archive/2005/09/17/469941.aspx for more information.
    static const char* const a = atomicCanonicalTextEncodingName("Shift_JIS_X0213-2000");
    static const char* const b = atomicCanonicalTextEncodingName("EUC-JP");
    return (m_name == a || m_name == b) ? 0x00A5 : '\\';
	}
	
	bool TextEncoding::isNonByteBasedEncoding() const
	{
    if (noExtendedTextEncodingNameUsed()) {
			/*
			return *this == UTF16LittleEndianEncoding()
			|| *this == UTF16BigEndianEncoding();
			 */
			return false;
    }
		
    /*return *this == UTF16LittleEndianEncoding()
		|| *this == UTF16BigEndianEncoding()
		|| *this == UTF32BigEndianEncoding()
		|| *this == UTF32LittleEndianEncoding();
		 */
		return false;
	}
	
	bool TextEncoding::isUTF7Encoding() const
	{
    if (noExtendedTextEncodingNameUsed())
			return false;
		
    //return *this == UTF7Encoding();
		return false;
	}
	
	const TextEncoding& TextEncoding::closestByteBasedEquivalent() const
	{
    if (isNonByteBasedEncoding())
			return UTF8Encoding();
    return *this; 
	}
	
	// HTML5 specifies that UTF-8 be used in form submission when a form is 
	// is a part of a document in UTF-16 probably because UTF-16 is not a 
	// byte-based encoding and can contain 0x00. By extension, the same
	// should be done for UTF-32. In case of UTF-7, it is a byte-based encoding,
	// but it's fraught with problems and we'd rather steer clear of it.
	const TextEncoding& TextEncoding::encodingForFormSubmission() const
	{
    if (isNonByteBasedEncoding() || isUTF7Encoding())
			return UTF8Encoding();
    return *this;
	}
	
	const TextEncoding& UTF8Encoding()
	{
    static TextEncoding globalUTF8Encoding("UTF-8");
    ASSERT(globalUTF8Encoding.isValid());
    return globalUTF8Encoding;
	}
	
	PassOwnPtr<TextCodec> newTextCodec(const TextEncoding& encoding)
	{
		return newTextCodecICU(encoding, 0);
	}
}