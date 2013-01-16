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
#include <wtf/RefPtr.h>

#include "ResourceManager.h"
#include "Diagnostic.h"
#include "OAGlobalObject.h"
#include "OAJNIUtil.h"

using ATF::ByteArray;

namespace Aphid {
	using JNI::LocalObject;
	using JNI::LocalClass;
	using JNI::LocalString;
	using JNI::Cache;
	
	ResourceManager::ResourceManager()
	{
		OA_TODO();
	}
	
	ResourceManager::~ResourceManager()
	{
		OA_TODO();
	}
	
	ResourceManager* ResourceManager::s_instance = 0;
	ResourceManager* ResourceManager::sharedManager()
	{
		if (!s_instance)
			s_instance = new ResourceManager();
		return s_instance;
	}
	
	String ResourceManager::loadStringInBundle(const char *name)
	{
		//TODO: better error checking here
		
		JNIEnv* env = JNI::getJNIEnv();
		
		OwnPtr<LocalString> jname = JNI::toJNIString(env, name);
		OwnPtr<LocalObject> jret = JNI::callObjectMethod(env, Cache::s_resourcemanager_jobject_assetsbundle->global(), Cache::s_resourcemanager_jmethod_loadString, jname->local());
		String ret = JNI::toString(env, jret.get());
		
		return ret;
	}
		
	PassRefPtr<ATF::ByteArray> ResourceManager::loadDataInBundle(const char* name, bool supressLogging)
	{
		//TODO: better error checking here
		
		JNIEnv* env = JNI::getJNIEnv();
		
		OwnPtr<LocalString> jname = JNI::toJNIString(env, name);
		
		OwnPtr<LocalObject> jret = JNI::callObjectMethod(env, Cache::s_resourcemanager_jobject_assetsbundle->global(), Cache::s_resourcemanager_jmethod_loadData, jname->local(), supressLogging ? JNI_TRUE : JNI_FALSE);
		
		RefPtr<ByteArray> ret = JNI::toByteArray(env, jret.get());
		
		return ret.release();
	}
	
	PassOwnPtr<JNI::LocalObject> ResourceManager::loadBitmapInBundle(const char* name)
	{
		JNIEnv* env = JNI::getJNIEnv();
		OwnPtr<LocalString> jname = JNI::toJNIString(env, name);
		
		OwnPtr<LocalObject> jret = JNI::callObjectMethod(env, Cache::s_resourcemanager_jobject_assetsbundle->global(), Cache::s_resourcemanager_jmethod_loadBitmap, jname->local());
		return jret.release();
	}
}
