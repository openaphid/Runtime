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
 
 ===== IMPORTANT =====
 
 This is sample code demonstrating API, technology or techniques in development.
 Although this sample code has been reviewed for technical accuracy, it is not
 final. Apple is supplying this information to help you plan for the adoption of
 the technologies and programming interfaces described herein. This information
 is subject to change, and software implemented based on this sample code should
 be tested with final operating system software and final documentation. Newer
 versions of this sample code may be provided with future seeds of the API or
 technology. For information about updates to this and other developer
 documentation, view the New & Updated sidebars in subsequent documentation
 seeds.
 
 =====================
 
 File: EAGLView.m
 Abstract: Convenience class that wraps the CAEAGLLayer from CoreAnimation into a
 UIView subclass.
 
 Version: 1.3
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
 ("Apple") in consideration of your agreement to the following terms, and your
 use, installation, modification or redistribution of this Apple software
 constitutes acceptance of these terms.  If you do not agree with these terms,
 please do not use, install, modify or redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and subject
 to these terms, Apple grants you a personal, non-exclusive license, under
 Apple's copyrights in this original Apple software (the "Apple Software"), to
 use, reproduce, modify and redistribute the Apple Software, with or without
 modifications, in source and/or binary forms; provided that if you redistribute
 the Apple Software in its entirety and without modifications, you must retain
 this notice and the following text and disclaimers in all such redistributions
 of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may be used
 to endorse or promote products derived from the Apple Software without specific
 prior written permission from Apple.  Except as expressly stated in this notice,
 no other rights or licenses, express or implied, are granted by Apple herein,
 including but not limited to any patent rights that may be infringed by your
 derivative works or by other works in which the Apple Software may be
 incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
 WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
 WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
 COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
 DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
 CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
 APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2008 Apple Inc. All Rights Reserved.
 
 */

// Only compile this code on iOS. These files should NOT be included on your Mac project.
// But in case they are included, it won't be compiled.
#include <config.h>

#import <Availability.h>

#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED

#import <QuartzCore/QuartzCore.h>

#import "EAGLView.h"
#import "ES1Renderer.h"
//#import "../../CCDirector.h"
#import "G2DMacros.h"
#import "G2DConfiguration.h"
#import "OpenGL_Internal.h"

#import "Director.h"
#import "OAGlobalObject.h"
#import "Touch.h"

#define CCLOG NSLog
//CLASS IMPLEMENTATIONS:

using namespace Aphid;

@interface EAGLView (Private)
- (BOOL)setupSurfaceWithSharegroup:(EAGLSharegroup *)sharegroup;

- (unsigned int)convertPixelFormat:(OAGLViewPixelFormat)pixelFormat;
@end

@implementation EAGLView

@synthesize surfaceSize = size_;
@synthesize pixelFormat = pixelformat_, depthFormat = depthFormat_;
@synthesize touchDelegate = touchDelegate_;
@synthesize context = context_;
@synthesize multiSampling = multiSampling_;

+ (Class)layerClass
{
	return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame
{
	return [self initWithFrame:frame pixelFormat:OAGLViewPixelFormatRGB565 depthFormat:0 preserveBackbuffer:NO sharegroup:nil multiSampling:NO numberOfSamples:0];
}

- (id)initWithFrame:(CGRect)frame pixelFormat:(OAGLViewPixelFormat)format
{
	return [self initWithFrame:frame pixelFormat:format depthFormat:0 preserveBackbuffer:NO sharegroup:nil multiSampling:NO numberOfSamples:0];
}

- (id)initWithFrame:(CGRect)frame pixelFormat:(OAGLViewPixelFormat)format depthFormat:(GLuint)depth
{
	return [self initWithFrame:frame pixelFormat:format depthFormat:depth preserveBackbuffer:NO sharegroup:nil multiSampling:NO numberOfSamples:0];
}

- (id)initWithFrame:(CGRect)frame pixelFormat:(OAGLViewPixelFormat)format depthFormat:(GLuint)depth preserveBackbuffer:(BOOL)retained sharegroup:(EAGLSharegroup *)sharegroup multiSampling:(BOOL)sampling numberOfSamples:(unsigned int)nSamples
{
	if ((self = [super initWithFrame:frame])) {
		pixelformat_ = format;
		depthFormat_ = depth;
		multiSampling_ = sampling;
		requestedSamples_ = nSamples;
		preserveBackbuffer_ = retained;

		if (![self setupSurfaceWithSharegroup:sharegroup]) {
			[self release];
			return nil;
		}
	}

	return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder
{
	if ((self = [super initWithCoder:aDecoder])) {

		CAEAGLLayer *eaglLayer = (CAEAGLLayer *) [self layer];

		pixelformat_ = OAGLViewPixelFormatRGB565;
		depthFormat_ = 0; // GL_DEPTH_COMPONENT24_OES;
		multiSampling_ = NO;
		requestedSamples_ = 0;
		size_ = [eaglLayer bounds].size;

		if (![self setupSurfaceWithSharegroup:nil]) {
			[self release];
			return nil;
		}
	}

	return self;
}

- (BOOL)setupSurfaceWithSharegroup:(EAGLSharegroup *)sharegroup
{
	CAEAGLLayer *eaglLayer = (CAEAGLLayer *) self.layer;

	eaglLayer.opaque = YES;
	eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
		[NSNumber numberWithBool:preserveBackbuffer_], kEAGLDrawablePropertyRetainedBacking,
		[self convertPixelFormatToProperty:pixelformat_], kEAGLDrawablePropertyColorFormat, nil];


	renderer_ = [[ES1Renderer alloc] initWithDepthFormat:depthFormat_
	                                     withPixelFormat:[self convertPixelFormat:pixelformat_]
								                        withSharegroup:sharegroup
															       withMultiSampling:multiSampling_
																	 withNumberOfSamples:requestedSamples_];
	if (!renderer_)
		return NO;

	context_ = [renderer_ context];
	[context_ renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];

	discardFramebufferSupported_ = G2DConfiguration::supportsDiscardFramebuffer();

	return YES;
}

- (void)dealloc
{
	NSLog(@"deallocing %@", self);

	[renderer_ release];
	[super dealloc];
}

- (void)layoutSubviews
{
	if (!CGSizeEqualToSize(self.bounds.size * self.contentScaleFactor, size_)) { //Notes: different impl from CC
		[renderer_ resizeFromLayer:(CAEAGLLayer *) self.layer];
		size_ = [renderer_ backingSize];

		// Issue #914 #924
		Director *director = OAGlobalObject::sharedInstance()->namespaceG2D()->director();
		if (director) {
			director->reshapeProjection(size_);
			director->avoidLayoutFlicker();
		} else
			oa_debug("director is NULL");
	}
}

- (void)swapBuffers
{
	// IMPORTANT:
	// - preconditions
	//	-> context_ MUST be the OpenGL context
	//	-> renderbuffer_ must be the the RENDER BUFFER

#ifdef __IPHONE_4_0

	if (multiSampling_) {
		/* Resolve from msaaFramebuffer to resolveFramebuffer */
		//glDisable(GL_SCISSOR_TEST);     
		glBindFramebufferOES(GL_READ_FRAMEBUFFER_APPLE, [renderer_ msaaFrameBuffer]);
		glBindFramebufferOES(GL_DRAW_FRAMEBUFFER_APPLE, [renderer_ defaultFrameBuffer]);
		glResolveMultisampleFramebufferAPPLE();
	}

	if (discardFramebufferSupported_) {
		if (multiSampling_) {
			if (depthFormat_) {
				GLenum attachments[] = {GL_COLOR_ATTACHMENT0_OES, GL_DEPTH_ATTACHMENT_OES};
				glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, attachments);
			}
			else {
				GLenum attachments[] = {GL_COLOR_ATTACHMENT0_OES};
				glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, attachments);
			}

			glBindRenderbufferOES(GL_RENDERBUFFER_OES, [renderer_ colorRenderBuffer]);

		}

			// not MSAA
		else if (depthFormat_) {
			GLenum attachments[] = {GL_DEPTH_ATTACHMENT_OES};
			glDiscardFramebufferEXT(GL_FRAMEBUFFER_OES, 1, attachments);
		}
	}

#endif // __IPHONE_4_0

	if (![context_ presentRenderbuffer:GL_RENDERBUFFER_OES])
	oa_info("Failed to swap renderbuffer in %s\n", __FUNCTION__);

	if (OAGlobalObject::s_develop_mode)
	CHECK_GL_ERROR();

	// We can safely re-bind the framebuffer here, since this will be the
	// 1st instruction of the new main loop
	if (multiSampling_)
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, [renderer_ msaaFrameBuffer]);
}

- (unsigned int)convertPixelFormat:(OAGLViewPixelFormat)pixelFormat
{
	GLenum pFormat = GL_RGB565_OES;

	switch (pixelFormat) {
		case OAGLViewPixelFormatRGB565:
	    pFormat = GL_RGB565_OES;
	    break;
		case OAGLViewPixelFormatRGBA8:
	    pFormat = GL_RGBA8_OES;
	    break;
		default:
	    oa_error("Unknown pixel format: %d", pixelFormat);
	    break;
	}

	return pFormat;
}

- (NSString *)convertPixelFormatToProperty:(OAGLViewPixelFormat)pixelFormat
{
	switch (pixelFormat) {
		case OAGLViewPixelFormatRGB565:
	    return kEAGLColorFormatRGB565;
		case OAGLViewPixelFormatRGBA8:
	    return kEAGLColorFormatRGBA8;
		default:
	    oa_error("Unknown pixel format: %d", pixelFormat);
	    return kEAGLColorFormatRGB565;
	}
}

#pragma mark EAGLView - Point conversion

- (CGPoint)convertPointFromViewToSurface:(const CGPoint&)point
{
	CGRect bounds = [self bounds];

	return CGPointMake((point.x - bounds.origin.x) / bounds.size.width * size_.width, (point.y - bounds.origin.y) / bounds.size.height * size_.height);
}

- (CGRect)convertRectFromViewToSurface:(const CGRect&)rect
{
	CGRect bounds = [self bounds];

	return CGRectMake((rect.origin.x - bounds.origin.x) / bounds.size.width * size_.width, (rect.origin.y - bounds.origin.y) / bounds.size.height * size_.height, rect.size.width / bounds.size.width * size_.width, rect.size.height / bounds.size.height * size_.height);
}

// Pass the touches to the superview
#pragma mark EAGLView - Touch Delegate

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	TouchDispatcher::sharedDispatcher()->handleUITouches(touches, event, EventFlagTouchStart);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	TouchDispatcher::sharedDispatcher()->handleUITouches(touches, event, EventFlagTouchMove);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	TouchDispatcher::sharedDispatcher()->handleUITouches(touches, event, EventFlagTouchEnd);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	TouchDispatcher::sharedDispatcher()->handleUITouches(touches, event, EventFlagTouchCancel);
}

@end

#endif // __IPHONE_OS_VERSION_MAX_ALLOWED