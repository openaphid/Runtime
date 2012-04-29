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

#ifndef OpenAphid_HttpDownload_h
#define OpenAphid_HttpDownload_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/HashMap.h>
#include <wtf/RefCounted.h>
#include <WTFString.h>
#include <StringHash.h>

#ifdef __OBJC__
@class NSOperationQueue;
@class NSMutableData;
@class NSData;
@class OAHttpDownloadOperation;
#else
class NSOperationQueue;
class NSMutableData;
class NSData;
class OAHttpDownloadOperation;
#endif

namespace Aphid {	
	class XMLHttpRequest;
	
	typedef HashMap<Aphid::String, Aphid::String, Aphid::CaseFoldingHash> HttpHeaderMap;
	
	class HttpResponse : public ATF::RefCounted<HttpResponse> {
	public:
		virtual ~HttpResponse();
		
		static RefPtr<HttpResponse> create()
		{
			return ATF::adoptRef(new HttpResponse());
		}
		
		int statusCode() const {return m_statusCode;}
		void setStatusCode(int code) {m_statusCode = code;}
		
		Aphid::String statusString() const {return m_statusString;}
		void setStatusString(const Aphid::String& string) {m_statusString = string;}
		
		long long expectedContentLength() const {return m_expectedContentLength;}
		void setExpectedContentLength(long long length) {m_expectedContentLength = length;}
		
		Aphid::String mimeType() const {return m_mimeType;}
		void setMimeType(const Aphid::String& type) {m_mimeType = type;}
		
		Aphid::String encoding() const {return m_encoding;}
		void setEncoding(const Aphid::String& encoding) {m_encoding = encoding;}
		
		HttpHeaderMap& responseHeaders() {return m_responseHeaders;}
		
		void appendData(NSData* data);
		unsigned dataLength() const;
		
		Aphid::String responseText();
		
		void seal();
		
	protected:
		HttpResponse();
		
		void convertText();
		
		HttpHeaderMap m_responseHeaders;
		int m_statusCode;
		Aphid::String m_statusString;
		long long m_expectedContentLength;
		Aphid::String m_mimeType;
		Aphid::String m_encoding;
		NSMutableData* m_data; //TODO: use a platform independent data type		
		Aphid::String m_text;
		bool m_sealed;
	};
	
	class HttpDownload : public ATF::RefCounted<HttpDownload> {
	public:
		virtual ~HttpDownload();
		
		static ATF::PassRefPtr<HttpDownload> create()
		{
			return ATF::adoptRef(new HttpDownload());
		}
		
		void cancel();
		
		XMLHttpRequest* request() const {return m_request;}
		void setRequest(XMLHttpRequest* request) {m_request = request;}
		
		HttpResponse* response() const {return m_response.get();}
		
		OAHttpDownloadOperation* operation() const {return m_operation;}
		void setOperation(OAHttpDownloadOperation* operation) {m_operation = operation;}
		
	protected:
		HttpDownload();
		
		XMLHttpRequest* m_request;
		RefPtr<HttpResponse> m_response;
		OAHttpDownloadOperation* m_operation;
	};
	
	class HttpDownloadManager : ATFNoncopyable::Noncopyable {
	public:
		virtual ~HttpDownloadManager();
		
		static HttpDownloadManager* sharedManager();
		
		void addDownload(PassRefPtr<HttpDownload> download);
		
	protected:
		static HttpDownloadManager* s_shared_manager;
		
		HttpDownloadManager();
		
		NSOperationQueue* m_queue;
	};
}

#endif
