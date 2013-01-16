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
#include "HttpDownload.h"
#include "XMLHttpRequest.h"
#include "OAUtil.h"

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	/// HttpResponse
	HttpResponse::HttpResponse()
			: m_statusCode(0), m_expectedContentLength(0)
			//, m_data(0)
					, m_sealed(false)
	{
		LEAK_DETECT_INC("HttpResponse");
	}

	HttpResponse::~HttpResponse()
	{
		LEAK_DETECT_DEC("HttpResponse");
	}

	unsigned HttpResponse::dataLength() const
	{
		OA_TODO();
		return -1;
	}

	String HttpResponse::responseText()
	{
		if (m_sealed)
			return m_text;

		/*
		 if (m_data) {
		 convertText();
		 return m_text;
		 }
		 */OA_TODO();

		return String();
	}

	void HttpResponse::convertText()
	{
		/*
		 NSStringEncoding encoding = NSUTF8StringEncoding;
		 if (!m_encoding.isEmpty()) {
		 CFStringEncoding e = CFStringConvertIANACharSetNameToEncoding((CFStringRef) toNSString(m_encoding));
		 if (e == kCFStringEncodingInvalidId)
		 e = kCFStringEncodingUTF8;
		 encoding = CFStringConvertEncodingToNSStringEncoding(e);
		 }
		 if (encoding == kCFStringEncodingInvalidId)
		 encoding = NSUTF8StringEncoding;

		 NSString *string = [[NSString alloc] initWithData:m_data encoding:encoding];
		 m_text = toWebCoreString(string);
		 [string release];
		 */
		OA_TODO();
	}

	void HttpResponse::seal()
	{
		m_sealed = true;
		convertText();

		//OA_OBJC_RELEASE(m_data);
		OA_TODO();
	}

	///-------------------------------------------------------------------------------------------------------------------
	/// HttpDownload
	HttpDownload::HttpDownload()
			: m_request(0)
	//, m_operation(0)
	{
		m_response = HttpResponse::create(); //TODO: layzy creation?
		LEAK_DETECT_INC("HttpDownload");
	}

	HttpDownload::~HttpDownload()
	{
		LEAK_DETECT_DEC("HttpDownload");
	}

	void HttpDownload::cancel()
	{
		/*
		 if (m_operation) {
		 [m_operation cancel];
		 m_operation = 0;
		 }
		 */
		OA_TODO();
	}

	///-------------------------------------------------------------------------------------------------------------------
	/// HttpDownloadManager
	HttpDownloadManager::HttpDownloadManager()
	{
		/*
		 m_queue = [[NSOperationQueue alloc] init];
		 [m_queue setMaxConcurrentOperationCount:OA_MAX_CONCURRENT_DOWNLOAD];
		 */
		OA_TODO();
	}

	HttpDownloadManager::~HttpDownloadManager()
	{
		/*
		 [m_queue cancelAllOperations];
		 OA_OBJC_RELEASE(m_queue);
		 */
		OA_TODO();
	}

	HttpDownloadManager *HttpDownloadManager::s_shared_manager = 0;

	HttpDownloadManager *HttpDownloadManager::sharedManager()
	{
		if (!s_shared_manager)
			s_shared_manager = new HttpDownloadManager();

		return s_shared_manager;
	}

	void HttpDownloadManager::addDownload(PassRefPtr<HttpDownload> download)
	{
		/*
		 OAHttpDownloadOperation *op = [[OAHttpDownloadOperation alloc] initWithDownload:download];
		 [m_queue addOperation:op];
		 [op release];
		 */
		OA_TODO();
	}
}
