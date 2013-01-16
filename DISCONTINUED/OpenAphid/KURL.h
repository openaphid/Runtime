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

/*
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef OpenAphid_KURL_h
#define OpenAphid_KURL_h

#include "OAConfig.h"
#include <wtf/text/WTFString.h>
#include <runtime/UString.h>

namespace Aphid {
	class TextEncoding;
	struct KURLHash;
	
	enum ParsedURLStringTag { ParsedURLString };
	
	class KURL {
	public:
    // Generates a URL which contains a null string.
    KURL() { invalidate(); }
		
    // The argument is an absolute URL string. The string is assumed to be output of KURL::string() called on a valid
    // KURL object, or indiscernible from such.
    // It is usually best to avoid repeatedly parsing a string, unless memory saving outweigh the possible slow-downs.
    KURL(ParsedURLStringTag, const char*);
    KURL(ParsedURLStringTag, const Aphid::String&);
		
    // Resolves the relative URL with the given base URL. If provided, the
    // TextEncoding is used to encode non-ASCII characers. The base URL can be
    // null or empty, in which case the relative URL will be interpreted as
    // absolute.
    // FIXME: If the base URL is invalid, this always creates an invalid
    // URL. Instead I think it would be better to treat all invalid base URLs
    // the same way we treate null and empty base URLs.
    KURL(const KURL& base, const Aphid::String& relative);
    KURL(const KURL& base, const Aphid::String& relative, const TextEncoding&);
		
    // FIXME: The above functions should be harmonized so that passing a
    // base of null or the empty string gives the same result as the
    // standard String constructor.
		
    // Makes a deep copy. Helpful only if you need to use a KURL on another
    // thread.  Since the underlying StringImpl objects are immutable, there's
    // no other reason to ever prefer copy() over plain old assignment.
    KURL copy() const;
		
    bool isNull() const;
    bool isEmpty() const;
    bool isValid() const;
		
    // Returns true if this URL has a path. Note that "http://foo.com/" has a
    // path of "/", so this function will return true. Only invalid or
    // non-hierarchical (like "javascript:") URLs will have no path.
    bool hasPath() const;
		
    // Returns true if you can set the host and port for the URL.
    // Non-hierarchical URLs don't have a host and port.
    bool canSetHostOrPort() const { return isHierarchical(); }
		
    bool canSetPathname() const { return isHierarchical(); }
    
    const Aphid::String& string() const { return m_string; }
		
    Aphid::String protocol() const;
    Aphid::String host() const;
    unsigned short port() const;
    bool hasPort() const;
    Aphid::String user() const;
    Aphid::String pass() const;
    Aphid::String path() const;
    Aphid::String lastPathComponent() const;
    Aphid::String query() const;
    Aphid::String fragmentIdentifier() const;
    bool hasFragmentIdentifier() const;
		
    Aphid::String baseAsString() const;
		
    Aphid::String prettyURL() const;
    Aphid::String fileSystemPath() const;
		
    // Returns true if the current URL's protocol is the same as the null-
    // terminated ASCII argument. The argument must be lower-case.
    bool protocolIs(const char*) const;
    bool protocolInHTTPFamily() const;
    bool isLocalFile() const;
		
    bool setProtocol(const Aphid::String&);
    void setHost(const Aphid::String&);
		
    void removePort();
    void setPort(unsigned short);
		
    // Input is like "foo.com" or "foo.com:8000".
    void setHostAndPort(const Aphid::String&);
		
    void setUser(const Aphid::String&);
    void setPass(const Aphid::String&);
		
    // If you pass an empty path for HTTP or HTTPS URLs, the resulting path
    // will be "/".
    void setPath(const Aphid::String&);
		
    // The query may begin with a question mark, or, if not, one will be added
    // for you. Setting the query to the empty string will leave a "?" in the
    // URL (with nothing after it). To clear the query, pass a null string.
    void setQuery(const Aphid::String&);
		
    void setFragmentIdentifier(const Aphid::String&);
    void removeFragmentIdentifier();
		
    friend bool equalIgnoringFragmentIdentifier(const KURL&, const KURL&);
		
    friend bool protocolHostAndPortAreEqual(const KURL&, const KURL&);
		
    unsigned hostStart() const;
    unsigned hostEnd() const;
		
    unsigned pathStart() const;
    unsigned pathEnd() const;
    unsigned pathAfterLastSlash() const;
    operator const Aphid::String&() const { return string(); }
		
#if OA_DEV
    void print() const;
#endif
		
	private:
    void invalidate();
    bool isHierarchical() const;
    static bool protocolIs(const Aphid::String&, const char*);

    void init(const KURL&, const Aphid::String&, const TextEncoding&);
    void copyToBuffer(Vector<char, 512>& buffer) const;
		
    // Parses the given URL. The originalString parameter allows for an
    // optimization: When the source is the same as the fixed-up string,
    // it will use the passed-in string instead of allocating a new one.
    void parse(const Aphid::String&);
    void parse(const char* url, const Aphid::String* originalString);
		
    Aphid::String m_string;
    bool m_isValid : 1;
    bool m_protocolInHTTPFamily : 1;
		
    int m_schemeEnd;
    int m_userStart;
    int m_userEnd;
    int m_passwordEnd;
    int m_hostEnd;
    int m_portEnd;
    int m_pathAfterLastSlash;
    int m_pathEnd;
    int m_queryEnd;
    int m_fragmentEnd;
	};
	
	bool operator==(const KURL&, const KURL&);
	bool operator==(const KURL&, const Aphid::String&);
	bool operator==(const Aphid::String&, const KURL&);
	bool operator!=(const KURL&, const KURL&);
	bool operator!=(const KURL&, const Aphid::String&);
	bool operator!=(const Aphid::String&, const KURL&);
	
	bool equalIgnoringFragmentIdentifier(const KURL&, const KURL&);
	bool protocolHostAndPortAreEqual(const KURL&, const KURL&);
	
	const KURL& blankURL();
	
	// Functions to do URL operations on strings.
	// These are operations that aren't faster on a parsed URL.
	// These are also different from the KURL functions in that they don't require the string to be a valid and parsable URL.
	// This is especially important because valid javascript URLs are not necessarily considered valid by KURL.
	
	bool protocolIs(const Aphid::String& url, const char* protocol);
	bool protocolIsAJ(const Aphid::String& url);
	
	bool isDefaultPortForProtocol(unsigned short port, const Aphid::String& protocol);
	bool portAllowed(const KURL&); // Blacklist ports that should never be used for Web resources.
	
	Aphid::String mimeTypeFromDataURL(const Aphid::String& url);
	
	// Unescapes the given string using URL escaping rules, given an optional
	// encoding (defaulting to UTF-8 otherwise). DANGER: If the URL has "%00"
	// in it, the resulting string will have embedded null characters!
	Aphid::String decodeURLEscapeSequences(const Aphid::String&);
	Aphid::String decodeURLEscapeSequences(const Aphid::String&, const TextEncoding&);
	
	Aphid::String encodeWithURLEscapeSequences(const Aphid::String&);
	
	// Inlines.
	
	inline bool operator==(const KURL& a, const KURL& b)
	{
    return a.string() == b.string();
	}
	
	inline bool operator==(const KURL& a, const Aphid::String& b)
	{
    return a.string() == b;
	}
	
	inline bool operator==(const Aphid::String& a, const KURL& b)
	{
    return a == b.string();
	}
	
	inline bool operator!=(const KURL& a, const KURL& b)
	{
    return a.string() != b.string();
	}
	
	inline bool operator!=(const KURL& a, const Aphid::String& b)
	{
    return a.string() != b;
	}
	
	inline bool operator!=(const Aphid::String& a, const KURL& b)
	{
    return a != b.string();
	}
	
	// Inline versions of some non-GoogleURL functions so we can get inlining
	// without having to have a lot of ugly ifdefs in the class definition.
	
	inline bool KURL::isNull() const
	{
    return m_string.isNull();
	}
	
	inline bool KURL::isEmpty() const
	{
    return m_string.isEmpty();
	}
	
	inline bool KURL::isValid() const
	{
    return m_isValid;
	}
	
	inline bool KURL::hasPort() const
	{
    return m_hostEnd < m_portEnd;
	}
	
	inline bool KURL::protocolInHTTPFamily() const
	{
    return m_protocolInHTTPFamily;
	}
	
	inline unsigned KURL::hostStart() const
	{
    return (m_passwordEnd == m_userStart) ? m_passwordEnd : m_passwordEnd + 1;
	}
	
	inline unsigned KURL::hostEnd() const
	{
    return m_hostEnd;
	}
	
	inline unsigned KURL::pathStart() const
	{
    return m_portEnd;
	}
	
	inline unsigned KURL::pathEnd() const
	{
    return m_pathEnd;
	}
	
	inline unsigned KURL::pathAfterLastSlash() const
	{
    return m_pathAfterLastSlash;
	}
}

namespace ATF {
	
	// KURLHash is the default hash for String
	template<typename T> struct DefaultHash;
	template<> struct DefaultHash<Aphid::KURL> {
		typedef Aphid::KURLHash Hash;
	};
	
} // namespace ATF

#endif
