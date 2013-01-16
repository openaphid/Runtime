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
#include "XMLHttpRequest.h"
#include "AJEventListener.h"
#include "AJXMLHttpRequest.h"
#include "OAGlobalObject.h"

namespace Aphid {
	using Aphid::String;
	using namespace AJ;
	
	static bool isValidToken(const String& name)
	{
    unsigned length = name.length();
    for (unsigned i = 0; i < length; i++) {
			UChar c = name[i];
			
			if (c >= 127 || c <= 32)
				return false;
			
			if (c == '(' || c == ')' || c == '<' || c == '>' || c == '@' ||
					c == ',' || c == ';' || c == ':' || c == '\\' || c == '\"' ||
					c == '/' || c == '[' || c == ']' || c == '?' || c == '=' ||
					c == '{' || c == '}')
				return false;
    }
		
    return true;
	}
	
	static bool isValidHeaderValue(const String& name)
	{
    // FIXME: This should really match name against
    // field-value in section 4.2 of RFC 2616.
		
    return !name.contains('\r') && !name.contains('\n');
	}
	
	static bool isSetCookieHeader(const String& name)
	{
    return equalIgnoringCase(name, "set-cookie") || equalIgnoringCase(name, "set-cookie2");
	}
	
	static void findCharsetInMediaType(const String& mediaType, unsigned int& charsetPos, unsigned int& charsetLen, unsigned int start=0)
	{
    charsetPos = start;
    charsetLen = 0;
		
    int pos = start;
    int length = (int)mediaType.length();
    
    while (pos < length) {
			pos = mediaType.find("charset", pos, false);
			if (pos <= 0) {
				charsetLen = 0;
				return;
			}
			
			// is what we found a beginning of a word?
			if (mediaType[pos-1] > ' ' && mediaType[pos-1] != ';') {
				pos += 7;
				continue;
			}
			
			pos += 7;
			
			// skip whitespace
			while (pos != length && mediaType[pos] <= ' ')
				++pos;
			
			if (mediaType[pos++] != '=') // this "charset" substring wasn't a parameter name, but there may be others
				continue;
			
			while (pos != length && (mediaType[pos] <= ' ' || mediaType[pos] == '"' || mediaType[pos] == '\''))
				++pos;
			
			// we don't handle spaces within quoted parameter values, because charset names cannot have any
			int endpos = pos;
			while (pos != length && mediaType[endpos] > ' ' && mediaType[endpos] != '"' && mediaType[endpos] != '\'' && mediaType[endpos] != ';')
				++endpos;
			
			charsetPos = pos;
			charsetLen = endpos - pos;
			return;
    }
	}
	
	static void setCharsetInMediaType(String& mediaType, const String& charsetValue)
	{
    unsigned int pos = 0, len = 0;
		
    findCharsetInMediaType(mediaType, pos, len);
		
    if (!len) {
			// When no charset found, append new charset.
			mediaType.stripWhiteSpace();
			if (mediaType[mediaType.length() - 1] != ';')
				mediaType.append(";");
			mediaType.append(" charset=");
			mediaType.append(charsetValue);
    } else {
			// Found at least one existing charset, replace all occurrences with new charset.
			while (len) {
				mediaType.replace(pos, len, charsetValue);
				unsigned int start = pos + charsetValue.length();
				findCharsetInMediaType(mediaType, pos, len, start);
			}
    }
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	// XMLHttpRequest
	
	ForbiddenHeaderSet* XMLHttpRequest::s_forbidden_headers = 0;
	bool XMLHttpRequest::isForbiddenHeader(const String &header)
	{
		if (!s_forbidden_headers) {
			s_forbidden_headers = new ForbiddenHeaderSet();
			s_forbidden_headers->add("accept-charset");
			s_forbidden_headers->add("accept-encoding");
			s_forbidden_headers->add("access-control-request-headers");
			s_forbidden_headers->add("access-control-request-method");
			s_forbidden_headers->add("connection");
			s_forbidden_headers->add("content-length");
			s_forbidden_headers->add("content-transfer-encoding");
			s_forbidden_headers->add("cookie");
			s_forbidden_headers->add("cookie2");
			s_forbidden_headers->add("date");
			s_forbidden_headers->add("expect");
			s_forbidden_headers->add("host");
			s_forbidden_headers->add("keep-alive");
			s_forbidden_headers->add("origin");
			s_forbidden_headers->add("referer");
			s_forbidden_headers->add("te");
			s_forbidden_headers->add("trailer");
			s_forbidden_headers->add("transfer-encoding");
			s_forbidden_headers->add("upgrade");
			s_forbidden_headers->add("user-agent");
			s_forbidden_headers->add("via");
		}
		
		return s_forbidden_headers->contains(header);
	}
	
	XMLHttpRequest::XMLHttpRequest()
	: m_async(true)
	, m_state(UNSENT)
	, m_timeout(0)
	, m_download(0)
	, m_error(false)
	, m_uploadComplete(false)
	{
		LEAK_DETECT_INC("XMLHttpRequest");
	}
	
	XMLHttpRequest::~XMLHttpRequest()
	{
		if (m_download)
			m_download->setRequest(0);
		LEAK_DETECT_DEC("XMLHttpRequest");
	}
	
	void XMLHttpRequest::open(const Aphid::String &method, const Aphid::KURL &url, bool async, JSExceptionCode& error)
	{
		doAbort();
		State oldState = m_state;
		m_state = UNSENT;
		m_error = false;
		
		m_uploadComplete = false;
		
		clearResponse();
		clearRequest();
		
		if (!isValidToken(method)) {
			error = JS_SYNTAX_ERROR;
			oa_error("invalid http method %s", method.utf8().data());
			return;
		}
		
		String upperMethod = method.upper();
		
		if (upperMethod == "CONNECT" || upperMethod == "TRACE" || upperMethod == "TRACK") {
			error = JS_SECURITY_ERROR;
			return;
		}
		
		m_url = url;
		
		if (upperMethod == "GET" || upperMethod == "POST" || upperMethod == "COPY" || upperMethod == "DELETE" || upperMethod == "HEAD"
        || upperMethod == "INDEX" || upperMethod == "LOCK" || upperMethod == "M-POST" || upperMethod == "MKCOL" || upperMethod == "MOVE"
        || upperMethod == "OPTIONS" || upperMethod == "PROPFIND" || upperMethod == "PROPPATCH" || upperMethod == "PUT"
        || upperMethod == "UNLOCK")
			m_method = upperMethod;
    else
			m_method = method;
		
		if (!async)
			oa_warn("sync mode is not supported in OpenAphid");
    m_async = async || true; //TODO: force setting to true here
		
		ASSERT(!m_download);
		
		if (oldState != OPENED)
			changeState(OPENED);
		else 
			m_state = OPENED;
	}
	
	void XMLHttpRequest::send(Aphid::JSExceptionCode &error)
	{
		send(String(), error);
	}
	
	void XMLHttpRequest::send(const Aphid::String &data, Aphid::JSExceptionCode &error)
	{
		if (!initSend(error))
			return;
		
		if (!data.isNull() &&
				m_method != "GET" &&
				m_method != "HEAD" &&
				m_url.protocolInHTTPFamily()
				) {
			String contentType = m_requestHeaders.get("Content-Type");
			if (contentType.isEmpty())
				appendRequestHeader("Content-Type", "application/xml");
			else {
				setCharsetInMediaType(contentType, "UTF-8");
				m_requestHeaders.set("Content-Type", contentType);
			}
		}
		
		doCreateRequest(error);
	}
	
	void XMLHttpRequest::abort()
	{
		RefPtr<XMLHttpRequest> lock(this);
		
		bool sent = m_download != 0;
		doAbort();
		
		m_requestHeaders.clear();
		
		if ((m_state <= OPENED && !sent) || m_state == DONE)
			m_state = UNSENT;
		else {
			changeState(DONE);
			m_state = UNSENT;
		}
		
		dispatchEvent(EventFlagRequestAbort, ProgressEvent::create(EventNames::abortEvent));
		//TODO handle upload abort
	}
	
	void XMLHttpRequest::doCreateRequest(Aphid::JSExceptionCode &error)
	{
		UNUSED_PARAM(error);
		//TODO: handle error?
		ASSERT(!m_download);
		
		m_download = HttpDownload::create();
		m_download->setRequest(this);
		HttpDownloadManager::sharedManager()->addDownload(m_download);
		enterPendingState(this);
	}
	
	bool XMLHttpRequest::initSend(Aphid::JSExceptionCode &error)
	{
		if (!OAGlobalObject::sharedInstance())
			return false;
		
		if (m_state != OPENED || m_download) {
			error = JS_INVALID_STATE_ERROR;
			return false;
		}
		
		m_error = false;
		return true;
	}
	
	void XMLHttpRequest::doAbort()
	{
		m_error = true;
		
		if (m_download) {
			m_download->cancel();
			m_download->setRequest(0);
			m_download = 0;
			exitPendingState(this);
		}
	}
	
	void XMLHttpRequest::clearRequest()
	{
		m_requestHeaders.clear();
		m_entityBody = 0;
	}
	
	void XMLHttpRequest::clearResponse()
	{
		//TODO:
	}
	
	void XMLHttpRequest::changeState(Aphid::XMLHttpRequest::State state)
	{
		if (state != m_state) {
			m_state = state;
			fireReadyStateChange();
		}
	}
	
	void XMLHttpRequest::fireReadyStateChange()
	{
		if (!OAGlobalObject::sharedInstance())
			return;
		dispatchEvent(EventFlagRequestReadyStateChange, ProgressEvent::create(EventNames::readystatechangeEvent), m_state == DONE ? true : false);
		
		if (m_state == DONE && !m_error) { //TODO: fires loadend event according to the W3C specs, but it's different from WebCore, why?
			dispatchEvent(EventFlagRequestLoad, ProgressEvent::create(EventNames::loadEvent));
			dispatchEvent(EventFlagRequestLoadEnd, ProgressEvent::create(EventNames::loadendEvent));
		}
	}
	
	void XMLHttpRequest::dispatchEvent(Aphid::EventFlag flag, PassRefPtr<Aphid::Event> event, bool flush)
	{
		UNUSED_PARAM(flush);
		
		//TODO: handle flush?
		unsigned flags = this->flags();
		if (flags & flag) {
			EventListener* listener = findEventListener(flag);
			ASSERT(listener);
			event->setTarget(this);
			event->setCurrentTarget(this);
			listener->handleEvent(event.get());
		}
		oa_debug("%d, %s", (int)flag, event->type().string().utf8().data());
	}
	
	void XMLHttpRequest::didReceiveResponse()
	{
		if (m_error)
			return;
		
		changeState(HEADERS_RECEIVED);
	}
	
	void XMLHttpRequest::didReceiveData()
	{
		if (m_error)
			return;
		
		if (m_state != LOADING)
			changeState(LOADING);
		else 
			fireReadyStateChange();
	}
	
	void XMLHttpRequest::didFinishLoading()
	{
		if (m_error)
			return;
		
		changeState(DONE);
		if (m_download) {
			exitPendingState(this);
			m_download = 0;
		}
	}
	
	void XMLHttpRequest::didFail()
	{
		if (m_error)
			return;
		
		dispatchEvent(EventFlagRequestError, ProgressEvent::create(EventNames::errorEvent));
		
		doAbort();
	}
	
	void XMLHttpRequest::appendRequestHeader(const Aphid::String &header, const Aphid::String &value)
	{
		pair<HttpHeaderMap::iterator, bool> result = m_requestHeaders.add(header, value);
    if (!result.second)
			result.first->second += ", " + value;
	}
	
	int XMLHttpRequest::status(JSExceptionCode &error) const
	{
		if (m_download && m_download->response())
			return m_download->response()->statusCode();
		
		if (m_state == OPENED) //TODO: no error should be raised according to the W3C specs, but WebKit does raise one when state is OPENED
			error = JS_INVALID_STATE_ERROR;
		return 0;
	}
	
	String XMLHttpRequest::statusText(JSExceptionCode &error) const
	{
		if (m_download && m_download->response())
			return m_download->response()->statusString();
		if (m_state == OPENED) //TODO: no error should be raised according to the W3C specs, but WebKit does raise one when state is OPENED
			error = JS_INVALID_STATE_ERROR;
		return String();
	}
	
	void XMLHttpRequest::setRequestHeader(const Aphid::String &header, const Aphid::String &value, JSExceptionCode &error)
	{
		if (m_state != OPENED || m_download) {
			error = JS_INVALID_STATE_ERROR;
			return;
		}
		
		if (!isValidToken(header) || !isValidHeaderValue(value)) {
			error = JS_SYNTAX_ERROR;
			return;
		}
		
		if (isForbiddenHeader(header)) {
			oa_error("forbidden header type: %s", header.utf8().data());
			error = JS_SYNTAX_ERROR;
			return;
		}
		
		appendRequestHeader(header, value);
	}
	
	String XMLHttpRequest::getResponseHeader(const Aphid::String &header, JSExceptionCode &error) const
	{
		if (m_state < HEADERS_RECEIVED) {
			error = JS_INVALID_STATE_ERROR;
			return String();
		}
		
		ASSERT(m_download);
		if (m_download->response())
			return m_download->response()->responseHeaders().get(header);
		
		return String();
	}
	
	String XMLHttpRequest::responseText()
	{
		if (m_download && m_download->response())
			return m_download->response()->responseText();
		
		return String();
	}
	
	void XMLHttpRequest::markObjects(AJ::MarkStack &markStack, unsigned int markID)
	{
		if (isMarked(markID))
			return;
		
		DirectWrapper<AJXMLHttpRequest>::markObjects(markStack, markID);
		EventTarget::markEventListeners(markStack, markID);
	}
	
	void XMLHttpRequest::markWhenPending(AJ::MarkStack &markStack, unsigned markID)
	{
		markObjects(markStack, markID);
	}
}