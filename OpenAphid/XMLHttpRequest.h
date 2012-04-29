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

#ifndef OpenAphid_XMLHttpRequest_h
#define OpenAphid_XMLHttpRequest_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>
#include <wtf/RefCounted.h>
#include <wtf/HashMap.h>
#include <runtime/UString.h>
#include <StringHash.h>
#include "KURL.h"
#include "Event.h"
#include "ExceptionCode.h"
#include "DirectWrapper.h"
#include "OAConfig.h"
#include "HttpDownload.h"
#include "PendingObject.h"

namespace Aphid {
	class FormData;
	class XMLHttpRequestUpload;
	class AJXMLHttpRequest;
	
	typedef HashSet<Aphid::String, Aphid::CaseFoldingHash > ForbiddenHeaderSet;
	
	///-------------------------------------------------------------------------------------------------------------------
	class XMLHttpRequest : public ATF::RefCounted<XMLHttpRequest>, public EventTarget, public DirectWrapper<AJXMLHttpRequest>, public PendingObject {
	public:
		enum State {
			UNSENT = 0,
			OPENED = 1,
			HEADERS_RECEIVED = 2,
			LOADING = 3,
			DONE = 4
		};
		
	public:
		virtual ~XMLHttpRequest();
		
		virtual XMLHttpRequest* toXHR() {return this;}
		
		static bool isForbiddenHeader(const Aphid::String& header);
		static ATF::PassRefPtr<XMLHttpRequest> create()
		{
			return ATF::adoptRef(new XMLHttpRequest());
		}
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		virtual void markWhenPending(AJ::MarkStack& markStack, unsigned markID);
		
		void open(const Aphid::String& method, const KURL& url, bool async, JSExceptionCode& error); //TODO: support user/password credential in a future release
		void setRequestHeader(const Aphid::String& header, const Aphid::String& value, JSExceptionCode& error);
		void send(JSExceptionCode& error);
		//TODO:
		/*
		void send(ArrayBuffer data);
		void send(Blob data);
		 */
		void send(const Aphid::String& data, JSExceptionCode& error);
		void send(FormData* data, JSExceptionCode& error);
		
		void abort();
		
		KURL url() const {return m_url;}
		const HttpHeaderMap& headers() const {return m_requestHeaders;}
		Aphid::String method() const {return m_method;}
		unsigned timeout() const {return m_timeout;}
		void setTimeout(unsigned t) {m_timeout = t;}
		State readyState() const {return m_state;}
		int status(JSExceptionCode& error) const;
		Aphid::String statusText(JSExceptionCode& error) const;
		Aphid::String getResponseHeader(const Aphid::String& header, JSExceptionCode& error) const;
		Aphid::String responseText();
		
		//callbacks
		void fireReadyStateChange();
		void dispatchEvent(EventFlag flag, PassRefPtr<Event> event, bool flush=false);
		void didReceiveResponse();
		void didReceiveData();
		void didFinishLoading();
		void didFail(); ///TODO: add an error domain object, ontimeout is currently not implemented yet
		
		using RefCounted<XMLHttpRequest>::ref;
		using RefCounted<XMLHttpRequest>::deref;
	protected:
		static ForbiddenHeaderSet* s_forbidden_headers;
		
		XMLHttpRequest();

		virtual void refEventTarget() {ref();}
		virtual void derefEventTarget() {deref();}
		
		void doAbort();
		void clearRequest();
		void clearResponse();
		void changeState(State state);
		bool initSend(JSExceptionCode& error);
		void appendRequestHeader(const Aphid::String& header, const Aphid::String& value); //TODO: use atomic string for header?
		void doCreateRequest(JSExceptionCode& error);
		
		KURL m_url;
		Aphid::String m_method;
		HttpHeaderMap m_requestHeaders;
		RefPtr<FormData> m_entityBody;
		Aphid::String m_overrideMimeType;
		bool m_async;
		unsigned m_timeout;
		
		State m_state;
		
		RefPtr<HttpDownload> m_download;
		bool m_error;
		
		bool m_uploadComplete;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class XMLHttpRequestUpload : ATF::RefCounted<XMLHttpRequestUpload> {
	public:
		virtual ~XMLHttpRequestUpload();
		
	private:
		XMLHttpRequestUpload();
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class FormData : public ATF::RefCounted<FormData> {
	public:
		virtual ~FormData();
		
		void append(const Aphid::String& name, const Aphid::String& value);
		
	private:
		FormData();
	};
}


#endif
