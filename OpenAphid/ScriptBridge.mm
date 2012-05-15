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

#import "config.h"

#import <UIKit/UIKit.h>

#include <runtime/Completion.h>
#include <runtime/InitializeThreading.h>
#include <runtime/AJGlobalData.h>
#include <runtime/AJLock.h>
#include <API/APIShims.h>
#include <parser/SourceCode.h>

#import "AJOAGlobalObject.h"
#import "OAGlobalObject.h"

#import "ScriptBridge.h"
#include "Texture2D.h"
#include "SpriteBatchNode.h"

#import "EAGLView.h"
#include "ObjCDynamicBinding.h"
#include "AJDynamicBinding.h"
#include "AJNamespaceExt.h"

#include "Director.h"

#include "XMLHttpRequest.h"
#include "KURL.h"

#include "iOSUtils.h"
#include "ResourceManager.h"
#include "AJOABinding.h"
#include "NamespaceJS.h"
#include "Diagnostic.h"

#if OA_NATIVE_BENCHMARK
#include "Sprite.h"
#include "ActionEase.h"
#include "ActionGrid3D.h"
#include "ActionTiledGrid.h"
#include "NodeExt.h"
#include "ParticleSystem.h"
#endif

using namespace Aphid;
using namespace AJ;

@interface ScriptBridge() {
	Aphid::AJOAGlobalObject * _globalObject;
}
@end

@implementation ScriptBridge
@dynamic baseURL, developMode, bundleName;
@synthesize glViewDepthFormat, glViewPixelFormat;

- (void)setBaseURL:(NSURL *)baseURL 
{
	ResourceManager::sharedManager()->setBaseURL(baseURL);
	OAGlobalObject::sharedInstance()->namespaceCore()->setBaseURL(toKURL(baseURL));
}

- (NSURL *)baseURL 
{
	return ResourceManager::sharedManager()->baseURL();
}

- (void)setDevelopMode:(BOOL)developMode
{
	OAGlobalObject::s_develop_mode = developMode;
}

- (BOOL)developMode
{
	return OAGlobalObject::s_develop_mode;
}

- (NSString *)bundleName
{
	return ResourceManager::sharedManager()->bundleName();
}

- (void)setBundleName:(NSString *)bundleName
{
	ResourceManager::sharedManager()->setBundleName(bundleName);
}

- (void) initialize {
	AJ::initializeThreading();
	
	self.glViewPixelFormat = OAGLViewPixelFormatRGB565;
	self.glViewDepthFormat = 0;
	
	AJLock lock(LockForReal);
	
	RefPtr<AJGlobalData> globalData = AJGlobalData::create(AJ::ThreadStackTypeSmall);
	
	APIEntryShim shim(globalData.get(), false);
	
	RefPtr<Structure> structure = AJOAGlobalObject::createStructure(AJ::jsNull());
	_globalObject = new (globalData.get())AJOAGlobalObject(structure, OAGlobalObject::sharedInstance());
	gcProtect(_globalObject->globalExec()->dynamicGlobalObject());
	globalData->ref();

	
	OAGlobalObject::sharedInstance()->initialize();
}

- (void) destroy {
	//TODO:
	if (_globalObject) {
		ExecState* exec = _globalObject->globalExec();
		AJLock lock(exec);
		
		AJGlobalData& globalData = exec->globalData();
    AJGlobalObject* dgo = exec->dynamicGlobalObject();
    IdentifierTable* savedIdentifierTable = wtfThreadData().setCurrentIdentifierTable(globalData.identifierTable);
		
    // One reference is held by AJGlobalObject, another added by AJGlobalContextRetain().
    bool releasingContextGroup = globalData.refCount() == 2;
    bool releasingGlobalObject = Heap::heap(dgo)->unprotect(dgo);
    // If this is the last reference to a global data, it should also
    // be the only remaining reference to the global object too!
    ASSERT(!releasingContextGroup || releasingGlobalObject);
		
    // An API 'AJGlobalContextRef' retains two things - a global object and a
    // global data (or context group, in API terminology).
    // * If this is the last reference to any contexts in the given context group,
    //   call destroy on the heap (the global data is being  freed).
    // * If this was the last reference to the global object, then unprotecting
    //   it may  release a lot of GC memory - run the garbage collector now.
    // * If there are more references remaining the the global object, then do nothing
    //   (specifically that is more protects, which we assume come from other AJGlobalContextRefs).
    if (releasingContextGroup)
			globalData.heap.destroy();
    else if (releasingGlobalObject)
			globalData.heap.collectAllGarbage();
		
    globalData.deref();
		
    wtfThreadData().setCurrentIdentifierTable(savedIdentifierTable);
		
		_globalObject = NULL;
		
		OAGlobalObject::destroyInstance();
		
		ALIVE_REPORT();
	}
}

- (void)dealloc {
	[self destroy];
	[super dealloc];
}

- (BOOL)evaluateScriptFromFile:(NSString *)filename
{
	if (OAGlobalObject::s_develop_mode)
		Diagnostic::displayNotification([UIColor blackColor], [NSString stringWithFormat:@"Loading '%@' in develop mode", filename]);
	NSString* content = ResourceManager::sharedManager()->loadNSStringInBundle(filename);
	return [self evaluateScript:content URLString:filename repeat:1];
}

- (BOOL) evaluateScript:(NSString *)script URLString:(NSString *)urlString repeat:(NSUInteger)repeat {
	if (script.length) {
		if (OAGlobalObject::s_develop_mode)
			Diagnostic::displayNotification([UIColor blackColor], [NSString stringWithFormat:@"Loads '%@' in develop mode", urlString]);
		ExecState* exec = _globalObject->globalExec();		
		for (int i = 0; i < repeat; i++) {			
			Completion completion = NamespaceJS::includeScript(exec, exec->dynamicGlobalObject(), toUString(urlString), toUString(script));
			
			if (completion.complType() == Throw) {
				reportAJException(exec, completion.value());
				return NO;
			}
		}
		
		OAGlobalObject::sharedInstance()->namespaceJS()->fireOnload();	
		return YES;
	} else {
		Diagnostic::error([NSString stringWithFormat:@"Script [%@] is empty?!", urlString]);
		return YES;
	}	
}

- (void)setScriptBinding:(id<OABindingProtocol>)receiver name:(NSString *)name iOSOnly:(BOOL)iosOnly
{
	RefPtr<ObjCDynamicBinding> binding = ObjCDynamicBinding::create([name cStringUsingEncoding:NSUTF8StringEncoding], receiver);
	ExecState* exec = _globalObject->globalExec();
	AJLock lock(exec);
	AJNamespaceExt* ajNamespaceExt = ajoa_cast<AJNamespaceExt*>(asObject(toAJ(exec, _globalObject, iosOnly ? _globalObject->impl()->namespaceExtIOS() : _globalObject->impl()->namespaceExt())));
	AJDynamicBinding* ajBinding = ajoa_cast<AJDynamicBinding*>(asObject(toAJ(exec, _globalObject, binding.get())));
	ajNamespaceExt->putDirect(Identifier(exec, toUString(name)), ajBinding, ReadOnly | DontDelete);
	ASSERT(!exec->hadException());
}

- (BOOL) garbageCollection {
	return _globalObject->impl()->garbageCollection();
}

#if OA_NATIVE_BENCHMARK

#define USE_BATCH_NODE 0

+ (void) loadNativeTest {
	RefPtr<Scene> scene = Scene::create();
	
	UIImage* image = [UIImage imageNamed:@"tank.png"];
	UIImage* bgImage = [UIImage imageNamed:@"background.png"];
	
	RefPtr<Texture2D> texture = Texture2D::create(image.CGImage);
	
	SpriteFrameVector frames;
	for (int i = 0; i < 5; i++) {
		CGSize size = image.size;
		RefPtr<SpriteFrame> frame = SpriteFrame::create(texture, CGRectMake(size.width * i / 5, 0, size.width / 5, size.height));
		frames.append(frame);
	}
	
	scene->addChild(Sprite::create(Texture2D::create(bgImage.CGImage)));
	
#if USE_BATCH_NODE
	RefPtr<SpriteBatchNode> batchNode = SpriteBatchNode::create(texture);
	scene->addChild(batchNode);
#endif
	
	for (int i = 0; i < 1000; i++) {		
		//RefPtr<Sprite> sprite = Sprite::create(texture);		
		RefPtr<Sprite> sprite = Sprite::createWithFrame(frames.at(0));
		
		sprite->setPosition(PointMake(arc4random() % 320, arc4random() % 480));
		
		sprite->setTouchEnabled(true);
		
		sprite->scheduleUpdate();
		
		//Scheduler::sharedScheduler()->unscheduleUpdate(sprite.get());
#if USE_BATCH_NODE
		batchNode->addChild(sprite);
#else
		scene->addChild(sprite);
#endif
		
		//sprite->runAction(FadeOutAction::create(15));
		//sprite->runAction(MoveByAction::create(15, PointMake(50, -50)));
		
		RefPtr<Animation> animation = Animation::create(frames, 0.05);
		RefPtr<AnimateAction> animate = AnimateAction::create(animation, false);
		sprite->runAction(RepeatForeverAction::create(animate));
		
		
		/*Vector<RefPtr<FiniteTimeAction> > actions;
		 actions.append(MoveToAction::create(1, PointMake(320, 480)));
		 actions.append(MoveToAction::create(2, PointMake(0, 0)));
		 
		 RefPtr<IntervalAction> a = SequenceAction::create(actions);
		 
		 RefPtr<ScaleByAction> s = ScaleByAction::create(2, Vector2Make(2, 2));
		 RefPtr<ScaleByAction> rs = ATF::adoptRef(s->newReverse());
		 
		 RefPtr<SequenceAction> ss = SequenceAction::create(s, rs);
		 RefPtr<RepeatForeverAction> rss = RepeatForeverAction::create(EaseInOutAction::create(ss, 2));
		 
		 sprite->runAction(a);
		 sprite->runAction(rss);
		 
		 RefPtr<Waves3DAction> w3d = Waves3DAction::create(5, 40, ccg(15, 10), 10);
		 sprite->runAction(w3d);*/
	}
	
	/*
	RefPtr<ColorNode> colorNode = ColorNode::create(WebColors::cyan, SizeMake(320, 100));
	scene->addChild(colorNode);
	
	RefPtr<GradientNode> gradientNode = GradientNode::create(WebColors::black, WebColors::cyan);
	gradientNode->setContentSize(SizeMake(200, 50));
	scene->addChild(gradientNode);
	
	ParticleConfig config;
	config.setNumber(ParticleConfig::maxParticles, 250);
	config.setNumber(ParticleConfig::duration, kCCParticleDurationInfinity);
	config.setNumber(ParticleConfig::emitterType, kCCParticleModeGravity);
	config.setNumber(ParticleConfig::speed, 60);
	config.setNumber(ParticleConfig::speedVariance, 20);
	config.setNumber(ParticleConfig::angle, 90);
	config.setNumber(ParticleConfig::angleVariance, 10);
	config.setNumber(ParticleConfig::startParticleSize, 54);
	config.setNumber(ParticleConfig::startParticleSizeVariance, 10);
	config.setNumber(ParticleConfig::finishParticleSize, kCCParticleStartSizeEqualToEndSize);
	config.setNumber(ParticleConfig::particleLifespan, 3);
	config.setNumber(ParticleConfig::particleLifespanVariance, 0.25f);
	config.setNumber(ParticleConfig::startColorRed, 0.76f);
	config.setNumber(ParticleConfig::startColorGreen, 0.25f);
	config.setNumber(ParticleConfig::startColorBlue, 0.12f);
	config.setNumber(ParticleConfig::startColorAlpha, 1.0f);
	config.setNumber(ParticleConfig::finishColorAlpha, 1.0f);
	config.setNumber(ParticleConfig::startColorVarianceAlpha, 0.0f);
	config.setNumber(ParticleConfig::finishColorVarianceAlpha, 0.0f);
	config.setString(ParticleConfig::textureFileName, "fire.png");
	
	RefPtr<ParticleSystemPoint> fire1 = ParticleSystemPoint::create(config);
	RefPtr<ParticleSystemQuad> fire = ParticleSystemQuad::create(config);
	fire->setGravity(PointZero);
	fire->setRadialAccel(0);
	fire->setRadialAccelVar(0);
	fire->setPosition(PointMake(160, 240));
	fire->setPositionVariance(PointMake(40, 20));
	fire->setBlendAdditive(true);
	
	scene->addChild(fire);
	 */
	
	
	//RefPtr<SplitColumnsAction> w3d = SplitColumnsAction::create(10, 10);
	//scene->runAction(SequenceAction::create(w3d, StopGridAction::create()));
	
	Director::sharedDirector()->runWithScene(scene);
	
	
	//RefPtr<Test> test = Test::create(0);
	
	//Scheduler::sharedScheduler()->scheduleCallback(test, callback, 1, false, 1, 10);
}
#endif

- (UIView *)createGLView:(UIWindow*)window {
	EAGLView *glView = [[EAGLView alloc] initWithFrame:[window bounds]
																				 pixelFormat:self.glViewPixelFormat
																				 depthFormat:self.glViewDepthFormat
											];
	NSLog(@"glView.frame: %@", NSStringFromCGRect(glView.frame));
	
	
	//TODO: effect_adv_test requires pixelFormat be RGBA8
	/*
	 EAGLView *glView = [[EAGLView alloc] initWithFrame:[window bounds]
	 pixelFormat:OAGLViewPixelFormatRGBA8
	 depthFormat:GL_DEPTH_COMPONENT24_OES
	 ];*/
	
	//glView.multipleTouchEnabled = YES;
	
	Director::sharedDirector()->setGLView(glView);
	Director::sharedDirector()->setAnimationInterval(1.0/60.0);
	
	//Director::sharedDirector()->enableRetinaDisplay(true);
	
	//Director::sharedDirector()->setDisplayFPS(true);
	
#if OA_NATIVE_BENCHMARK
	Director::sharedDirector()->setDisplayFPS(true);
	[self loadNativeTest];
#endif
	
	return glView;
}

@end
