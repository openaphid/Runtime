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
#include "Animation.h"
#include "AJAnimation.h"

namespace Aphid {
	Animation::Animation()
	: m_delay(0)
	{
		LEAK_DETECT_INC("Animation");
	}
	
	Animation::~Animation()
	{
		LEAK_DETECT_DEC("Animation");
	}
	
	PassRefPtr<Animation> Animation::create(const SpriteFrameVector& frames, TimeSec delay)
	{
		Animation* a = new Animation();
		a->m_frames.appendRange(frames.begin(), frames.end());
		a->setDelay(delay);
		return adoptRef(a);
	}
	
	void Animation::addFrame(PassRefPtr<SpriteFrame> frame)
	{
		m_frames.append(frame);
	}
	
	void Animation::markObjects(AJ::MarkStack &markStack, unsigned int markID)
	{
		if (isMarked(markID))
			return;
		DirectWrapper<AJAnimation>::markObjects(markStack, markID);
		for (SpriteFrameVector::iterator it = m_frames.begin(); it != m_frames.end(); ++it) {
			it->get()->markObjects(markStack, markID);
		}
	}
}