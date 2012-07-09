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

#include <string.h>
#include <runtime/InternalFunction.h>

#include "ObjCDynamicBinding.h"

#include "AJDynamicBinding.h"

#import "iOSUtils.h"
#include "Diagnostic.h"
#include "OAGlobalObject.h"

using namespace AJ;

@interface OABindingMap ()
{
	NSMutableDictionary* _bindings;
}
@property(nonatomic, assign, readonly) NSDictionary* bindings;
@end

@implementation OABindingMap
@synthesize bindings = _bindings;

- (id)init
{
	self = [super init];
	if (self) {
		_bindings = [[NSMutableDictionary alloc] init];
	}
	
	return self;
}

- (void)dealloc
{
	OA_OBJC_RELEASE(_bindings);
	[super dealloc];
}

- (void)bindSelector:(SEL)sel forName:(NSString *)name
{
	if (name && sel)
		[_bindings setValue:[NSValue valueWithPointer:sel] forKey:name];
}
@end

namespace Aphid {
	ObjCDynamicBinding::ObjCDynamicBinding(const char* name, id<OABindingProtocol> receiver)
	: Base(name)
	{
		m_receiver = [receiver retain];
		
		m_bindings = CFDictionaryCreateMutable(NULL, 0, &kCFCopyStringDictionaryKeyCallBacks, NULL);
		
		OABindingMap* bindingMap = [[[OABindingMap alloc] init] autorelease];
		[m_receiver bindSelectors:bindingMap];
		
		[bindingMap.bindings enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *) {
			BOOL correct = NO;
			
			if ([key isKindOfClass:[NSString class]] && [obj isKindOfClass:[NSValue class]]) { //check basic types
				NSString* name = key;
				NSValue* selValue = obj;
				
				SEL sel = (SEL)[selValue pointerValue];
				
				if (sel && [m_receiver respondsToSelector:sel]) { //check SEL
					bool invalid = false;
					
					if (OAGlobalObject::s_develop_mode) { //check signature
						NSMethodSignature* sig = [(id)m_receiver methodSignatureForSelector:sel];
						if (isValidReturnType([sig methodReturnType])) {
							size_t len = [sig numberOfArguments];
							for (size_t i = 2; i < len; i++) {
								if (!isValidParameterType([sig getArgumentTypeAtIndex:i])) {
									Diagnostic::error([NSString stringWithFormat:@"Invalid parameter type '%s' at %dth for binding: '%@', selector: '%@'", [sig getArgumentTypeAtIndex:i], i - 2, name, NSStringFromSelector(sel)]);
									invalid = true;
								}
							}
						} else {
							Diagnostic::error([NSString stringWithFormat:@"Invalid return type '%s' for binding: '%@', selector: %@", [sig methodReturnType], name, NSStringFromSelector(sel)]);
							invalid = true;
						}
					} 
					
					if (!invalid) {
						correct = YES;
						CFDictionarySetValue(m_bindings, name, sel);
					}
				} else
					Diagnostic::error([NSString stringWithFormat:@"Receiver %@ doesn't respond to %@", receiver, NSStringFromSelector(sel)]);
			}
			
			if (!correct) {
				Diagnostic::error([NSString stringWithFormat:@"Found invalid value in bindSelectors: key %@, value %@", key, obj]);
			}
		}];
	}
	
	ObjCDynamicBinding::~ObjCDynamicBinding()
	{
		OA_OBJC_RELEASE(m_receiver);
		CFRelease(m_bindings);
	}
	
	bool ObjCDynamicBinding::isSignedIntegerType(const char *type)
	{
		return 
		strcmp(type, @encode(char)) == 0 ||
		strcmp(type, @encode(int)) == 0 || 
		strcmp(type, @encode(short)) == 0 ||
		strcmp(type, @encode(long)) == 0 ||
		strcmp(type, @encode(long long)) == 0;
	}
	
	bool ObjCDynamicBinding::isUnsignedIntegerType(const char *type)
	{
		return 
		strcmp(type, @encode(unsigned char)) == 0 ||
		strcmp(type, @encode(unsigned int)) == 0 || 
		strcmp(type, @encode(unsigned short)) == 0 ||
		strcmp(type, @encode(unsigned long)) == 0 ||
		strcmp(type, @encode(unsigned long long)) == 0;
	}
	
	bool ObjCDynamicBinding::isFloatOrDoubleType(const char *type)
	{
		return
		strcmp(type, @encode(float)) == 0 ||
		strcmp(type, @encode(double)) == 0;
	}
	
	bool ObjCDynamicBinding::isNumberType(const char* type)
	{
		return
		isSignedIntegerType(type) ||
		isUnsignedIntegerType(type) ||
		isFloatOrDoubleType(type);
	}
	
	bool ObjCDynamicBinding::isValidReturnType(const char *type)
	{
		return strcmp(type, @encode(void)) == 0 || isValidParameterType(type);
	}
	
	bool ObjCDynamicBinding::isValidParameterType(const char *type)
	{
		return
		isNumberType(type) ||
		strcmp(type, @encode(id)) == 0 ;
	}
	
	void ObjCDynamicBinding::listFunctionNames(Vector<AJ::UString>& names)
	{
		for (NSString* key in [(NSDictionary*)m_bindings allKeys]) {
			names.append(toUString(key));
		}
	}
	
	bool ObjCDynamicBinding::respondsToFunction(const AJ::UString &functionName)
	{
		return CFDictionaryContainsKey(m_bindings, toNSString(functionName));
	}
	
	id ObjCDynamicBinding::convertToObjC(AJ::ExecState *exec, AJ::AJValue value)
	{
		if (!value) //empty value
			return nil;
		else if (value.isBoolean())
			return [NSNumber numberWithBool:(value.toBoolean(exec) ? YES : NO)];
		else if (value.isInt32())
			return [NSNumber numberWithInt:value.toInt32(exec)];
		else if (value.isNumber())
			return [NSNumber numberWithDouble:value.toNumber(exec)];
		else if (value.isUndefinedOrNull())
			return nil;
		else if (value.isString())
			return toNSString(value.toString(exec));
		else if (value.isObject()) {
			if (value.inherits(&AJArray::info)) {
				AJArray* ajArray = ajoa_cast<AJArray*>(asObject(value));
				NSMutableArray* array = [NSMutableArray arrayWithCapacity:ajArray->length()];
				for (int i = 0; i < ajArray->length(); i++) {
					id v = convertToObjC(exec, ajArray->getIndex(i));
					[array addObject:(v==nil ? [NSNull null] : v)];
				}
				return array;
			} else if (value.inherits(&InternalFunction::info)) {
				if (OAGlobalObject::s_develop_mode)
					oa_info("Convert function object to nil");
				return nil;
			}	else {
				AJObject* ajObject = asObject(value);
				PropertyNameArray properties(exec->lexicalGlobalObject()->globalData());
				ajObject->getPropertyNames(exec, properties);
				size_t size = properties.size();
				NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:size];
				for (size_t i = 0; i < size; i++) {
					const Identifier& identifier = properties[i];
					AJValue ajV = ajObject->get(exec, identifier);
					if (ajV) {
						if (!ajV.inherits(&InternalFunction::info)) {
							NSString* key = toNSString(identifier.ustring());
							id v = convertToObjC(exec, ajV);
							[dict setValue:(v == nil ? [NSNull null] : v) forKey:key];
						} else {
							oa_debug("function value is ignored in dictionary: %s", identifier.ascii());
						}
					} else {
						oa_debug("empty value is ignored in dictionary: %s", identifier.ascii());
					}
				}
				return dict;
			}
		} else {
			oa_error("Invalid value type for convertToObjC");
			ASSERT_NOT_REACHED();
			return nil;
		}
	}
	
	AJValue ObjCDynamicBinding::convertToAJ(AJ::ExecState *exec, AJOAGlobalObject* globalObject, id o)
	{
		if (o == nil || [o isKindOfClass:[NSNull class]])
			return jsNull();
		else if ([o isKindOfClass:[NSNumber class]]) {
			NSNumber* number = o;
			const char* type = [number objCType];
			if (isSignedIntegerType(type))
				return jsNumber(exec, [number intValue]);
			else if (isUnsignedIntegerType(type))
				return jsNumber(exec, [number unsignedIntValue]); //TODO: may lose precision here?
			else if (isFloatOrDoubleType(type))
				return jsNumber(exec, [number doubleValue]);
			else {
				oa_error([[NSString stringWithFormat:@"Invalid objc type of NSNumber: %@, %s", number, type] cStringUsingEncoding:NSUTF8StringEncoding]);
				ASSERT_NOT_REACHED();
				return jsNumber(exec, [number doubleValue]);
			}
		} else if ([o isKindOfClass:[NSString class]])
			return jsString(exec, toUString(o));
		else if ([o isKindOfClass:[NSArray class]]) {
			NSArray* array = o;
			MarkedArgumentBuffer args;
			for (id i in array) {
				args.append(convertToAJ(exec, globalObject, i));
			}
			return constructArray(exec, args);
		} else if ([o isKindOfClass:[NSDictionary class]]) {
			NSDictionary* dict = o;
			AJObject* aj = constructEmptyObject(exec, globalObject);
			[dict enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *) {
				if ([key isKindOfClass:[NSString class]]) {
					aj->putDirect(Identifier(exec, toUString(key)), convertToAJ(exec, globalObject, obj));
				} else
					oa_warn([[NSString stringWithFormat:@"Unsupported type of key in dict for convertToAJ: %@", key] cStringUsingEncoding:NSUTF8StringEncoding]);
			}];
			return aj;
		} else {
			oa_error([[NSString stringWithFormat:@"Invalid Objective-C object: %@", o] cStringUsingEncoding:NSUTF8StringEncoding]);
			return jsUndefined();
		}
	}
	
	AJ::AJValue ObjCDynamicBinding::invokeFunction(AJ::ExecState *exec, Aphid::AJOAGlobalObject *globalObject, const AJ::UString &functionName, const AJ::ArgList &args)
	{
#undef try
#undef catch
		@try {
			return doInvokeFunction(exec, globalObject, functionName, args);
		}
		@catch (NSException *exception) {
			exec->setException(Error::create(exec, GeneralError, 
																			 [[NSString stringWithFormat:@"Failed to invoke Objective-C function: '%s', NSException: %@", functionName.UTF8String().data(), exception] cStringUsingEncoding:NSUTF8StringEncoding]
																			 )
												 );
		}
	}
	
	AJ::AJValue ObjCDynamicBinding::doInvokeFunction(AJ::ExecState *exec, AJOAGlobalObject* globalObject, const AJ::UString &functionName, const AJ::ArgList &args)
	{
		SEL sel = (SEL)CFDictionaryGetValue(m_bindings, toNSString(functionName));
		if (sel) {
			NSMethodSignature* sig = [(id)m_receiver methodSignatureForSelector:sel];
			NSInvocation* invocation = [NSInvocation invocationWithMethodSignature:sig];
			[invocation setTarget:m_receiver];
			[invocation setSelector:sel];
			size_t count = [sig numberOfArguments] - 2;
			if (args.size() >= count) {
				for (int i = 0; i < count; i++) {
					const char* type = [sig getArgumentTypeAtIndex:i + 2];
					AJValue arg = args.at(i);
					if (isSignedIntegerType(type)) {
						if (arg.isNumber() || arg.isBoolean()) {
							int v = arg.toInt32(exec);
							[invocation setArgument:&v atIndex:i + 2];
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "integer");
							break;
						}							
					} else if (isUnsignedIntegerType(type)) {
						if (arg.isNumber() || arg.isBoolean()) {
							unsigned v = arg.toUInt32(exec);
							[invocation setArgument:&v atIndex:i + 2];
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "unsigned integer");
							break;
						}
					}
					else if (strcmp(type, @encode(float)) == 0) {
						if (arg.isNumber()) {
							float v = arg.toFloat(exec);
							[invocation setArgument:&v atIndex:i + 2];
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "float");
							break;
						}
					} else if (strcmp(type, @encode(double)) == 0) {
						if (arg.isNumber()) {
							double v = arg.toNumber(exec);
							[invocation setArgument:&v atIndex:i + 2];
						} else {
							OA_ERROR_INVALID_ARGUMENT(exec, formatUString("paramter_%d", i).UTF8String().data(), "double");
							break;
						}
					} else if (strcmp(type, @encode(id)) == 0) {
						id v = convertToObjC(exec, arg);
						[invocation setArgument:&v atIndex:i + 2];
					} else {
						oa_error("Invalid arg type in native function: %s", type);
						ASSERT_NOT_REACHED();
					}
				} //end of for loop
				
				if (!exec->hadException()) {
					AJValue retValue = jsUndefined();
					
					[invocation invoke];
					
					const char* type = [sig methodReturnType];
					
					if (strcmp(type, @encode(void)) == 0)
						retValue = jsUndefined();
					else {
						
#define GET_RETURN_NUMBER(TYPE) \
if (strcmp(type, @encode(TYPE)) == 0) { \
TYPE v = 0; \
[invocation getReturnValue:&v]; \
retValue = jsNumber(exec, v); \
}
						
						GET_RETURN_NUMBER(char)
						else GET_RETURN_NUMBER(int)
						else GET_RETURN_NUMBER(short)
						else GET_RETURN_NUMBER(long)
						else GET_RETURN_NUMBER(long long)
						else GET_RETURN_NUMBER(unsigned char)
						else GET_RETURN_NUMBER(unsigned int)
						else GET_RETURN_NUMBER(unsigned short)
						else GET_RETURN_NUMBER(unsigned long)
						else GET_RETURN_NUMBER(unsigned long long)
						else GET_RETURN_NUMBER(float)
						else GET_RETURN_NUMBER(double)
						else if (strcmp(type, @encode(id)) == 0) {
							id v = nil;
							[invocation getReturnValue:&v];
							retValue = convertToAJ(exec, globalObject, v);
						} else {
							oa_error("Invalid return type encoding: %s", type);
						}
						
						return retValue;
						
#undef GET_RETURN_NUMBER						
					}
					
				}
			} else
				OA_ERROR_NOT_ENOUGH_ARGUMENTS(exec, count, args.size());
		}
		return jsUndefined();
	}
}
