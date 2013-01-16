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
#include "iOSUtils.h"

using namespace Aphid;

@interface OAHttpDownloadOperation : NSOperation {
	RefPtr<HttpDownload> _download;
	BOOL _executing;
	BOOL _finished;
	NSURLConnection *_connection;
}

- (void)finish;

- (void)updateExecuting:(BOOL)executing;

- (void)updateFinished:(BOOL)finished;
@end

@implementation OAHttpDownloadOperation

- (id)initWithDownload:(NonNullPassRefPtr<HttpDownload>)download
{
	self = [super init];
	if (self) {
		_download = download;
		_download->setOperation(self);
		_executing = NO;
		_finished = NO;
		_connection = nil;
	}
	return self;
}

- (void)dealloc
{
	oa_debug("OAHttpDownloadOperation dealloc");
	_download->setOperation(0);
	_download = 0;
	OA_OBJC_RELEASE(_connection);
	[super dealloc];
}

- (void)updateExecuting:(BOOL)executing
{
	[self willChangeValueForKey:@"isExecuting"];
	_executing = executing;
	[self didChangeValueForKey:@"isExecuting"];
}

- (void)updateFinished:(BOOL)finished
{
	[self willChangeValueForKey:@"isFinished"];
	_finished = finished;
	[self didChangeValueForKey:@"isFinished"];
}

- (BOOL)isConcurrent
{
	return YES;
}

- (BOOL)isFinished
{
	return _finished;
}

- (BOOL)isExecuting
{
	return _executing;
}

- (void)start
{
	if (![self isCancelled]) {
		RefPtr<XMLHttpRequest> r = _download->request();
		if (r) {
			NSURL *url = toNSURL(r->url());
			if (url) {
				NSTimeInterval timeout = r->timeout() == 0 ? NSIntegerMax : (NSTimeInterval) (r->timeout() / 1000.0);
				NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:timeout];
				if (request) {
					[request setHTTPMethod:toNSString(r->method())];
					const HttpHeaderMap& headers = r->headers();

					[request setValue:@"gzip" forHTTPHeaderField:@"Accept-Encoding"];
					[request addValue:@"Mozilla/5.0 (iPhone; U; CPU iPhone OS 4_3_3 like Mac OS X; en-us) AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8J2 Safari/6533.18.5" forHTTPHeaderField:@"User-Agent"];

					for (HttpHeaderMap::const_iterator it = headers.begin(); it != headers.end(); ++it)
						[request addValue:toNSString(it->second) forHTTPHeaderField:toNSString(it->first)];

					_connection = [[NSURLConnection alloc] initWithRequest:request delegate:self startImmediately:NO]; //TODO: check if delegate causes memory leak
					if (_connection) {
						[_connection scheduleInRunLoop:[NSRunLoop mainRunLoop]
						                       forMode:NSDefaultRunLoopMode];
						[_connection start];

						[self updateExecuting:YES];
						[self updateFinished:NO];

						return;
					} else
						oa_warn("connection is nil");
				} else
					oa_warn("request is nil");
			} else
				oa_warn("url is nil");
		} else
			oa_warn("XMLHttpRequest is null");
	}

	[self finish];
}

- (void)finish
{
	[_connection cancel];
	OA_OBJC_RELEASE(_connection);

	[self updateExecuting:NO];
	[self updateFinished:YES];
}

// delegate methods
- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
	UNUSED_PARAM(connection);

	if ([self isCancelled]) {
		[self finish];
		return;
	}

	HttpResponse *r = _download->response();
	r->setMimeType(toWebCoreString(response.MIMEType));
	r->setExpectedContentLength(response.expectedContentLength);
	r->setEncoding(toWebCoreString(response.textEncodingName));

	if ([response isKindOfClass:[NSHTTPURLResponse class]]) {
		NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *) response;
		r->setStatusCode(httpResponse.statusCode);
		HttpHeaderMap& mutableHeaders = r->responseHeaders();
		NSDictionary *headers = httpResponse.allHeaderFields;
		for (NSString *key in headers.keyEnumerator)
			mutableHeaders.add(toWebCoreString(key), toWebCoreString([headers valueForKey:key]));
		r->setStatusString(toWebCoreString([NSHTTPURLResponse localizedStringForStatusCode:httpResponse.statusCode]));
	}

	if (_download->request())
		_download->request()->didReceiveResponse();
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
	UNUSED_PARAM(connection);

	if ([self isCancelled]) {
		[self finish];
		return;
	}

	HttpResponse *r = _download->response();
	r->appendData(data);

	if (_download->request())
		_download->request()->didReceiveData();
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
	UNUSED_PARAM(connection);
	UNUSED_PARAM(error);

	///TODO P1: translate error to js error message
	if (_download->request())
		_download->request()->didFail();

	[self finish];
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
	UNUSED_PARAM(connection);

	if (_download->request())
		_download->request()->didFinishLoading();

	[self finish];
}

- (NSCachedURLResponse *)connection:(NSURLConnection *)connection willCacheResponse:(NSCachedURLResponse *)cachedResponse
{
	UNUSED_PARAM(connection);
	UNUSED_PARAM(cachedResponse);

	if ([self isCancelled]) {
		[self finish];
		return nil;
	}

	//TODO: fire event
	return nil;
}

- (NSURLRequest *)connection:(NSURLConnection *)connection willSendRequest:(NSURLRequest *)request redirectResponse:(NSURLResponse *)response
{
	UNUSED_PARAM(connection);
	UNUSED_PARAM(response);

	//TODO: save information of redirection?

	if ([self isCancelled]) {
		[self finish];
		return nil;
	}

	//TODO: fire event
	return request;
}
@end

namespace Aphid {
  using Aphid::String;
  ///-------------------------------------------------------------------------------------------------------------------
  /// HttpResponse
  HttpResponse::HttpResponse()
  : m_statusCode(0)
  , m_expectedContentLength(0)
  , m_data(0)
  , m_sealed(false)
  {
	  LEAK_DETECT_INC("HttpResponse");
  }

  HttpResponse::~HttpResponse()
  {
	  LEAK_DETECT_DEC("HttpResponse");
  }

  void HttpResponse::appendData(NSData *data)
  {
	  if (!m_data)
		  m_data = [[NSMutableData alloc] init];

	  [m_data appendData:data];
  }

  unsigned HttpResponse::dataLength() const
  {
	  return [m_data length];
  }

  String HttpResponse::responseText()
  {
	  if (m_sealed)
		  return m_text;

	  if (m_data) {
		  convertText();
		  return m_text;
	  }

	  return String();
  }

  void HttpResponse::convertText()
  {
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
  }

  void HttpResponse::seal()
  {
	  m_sealed = true;
	  convertText();
	  OA_OBJC_RELEASE(m_data);
  }

  ///-------------------------------------------------------------------------------------------------------------------
  /// HttpDownload
  HttpDownload::HttpDownload()
  : m_request(0)
  , m_operation(0)
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
	  if (m_operation) {
		  [m_operation cancel];
		  m_operation = 0;
	  }
  }

  ///-------------------------------------------------------------------------------------------------------------------
  /// HttpDownloadManager
  HttpDownloadManager::HttpDownloadManager()
  {
	  m_queue = [[NSOperationQueue alloc] init];
	  [m_queue setMaxConcurrentOperationCount:OA_MAX_CONCURRENT_DOWNLOAD];
  }

  HttpDownloadManager::~HttpDownloadManager()
  {
	  [m_queue cancelAllOperations];
	  OA_OBJC_RELEASE(m_queue);
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
	  OAHttpDownloadOperation *op = [[OAHttpDownloadOperation alloc] initWithDownload:download];
	  [m_queue addOperation:op];
	  [op release];
  }
}
