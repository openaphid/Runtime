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

#ifndef OpenAphid_Animation_h
#define OpenAphid_Animation_h

#include "SpriteFrame.h"
#include "Texture2D.h"
#include "wtf/text/WTFString.h"

namespace Aphid {
	class AJAnimation;
	
	typedef Vector<RefPtr<SpriteFrame> > SpriteFrameVector;
	
	class Animation : public ATF::RefCounted<Animation> , public DirectWrapper<AJAnimation>{
		typedef Animation Self;
	public:
		virtual ~Animation();
		
		static PassRefPtr<Self> create()
		{
			return adoptRef(new Self());
		}
		
		static PassRefPtr<Self> create(const SpriteFrameVector& frames, TimeSec delay=1.0f/60.0f);
		
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
		Aphid::String name() const {return m_name;}
		void setName(const char* name) {m_name = name;}
		
		float delay() const {return m_delay;}
		void setDelay(float d) {m_delay = d;}
		
		const SpriteFrameVector& frames() const {return m_frames;}
		
		void addFrame(PassRefPtr<SpriteFrame> frame);
		
	protected:
		Animation();
		
		Aphid::String m_name;
		TimeSec m_delay;
		SpriteFrameVector m_frames;
	};
}

#endif
