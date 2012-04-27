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
#include "SpriteFrame.h"
#include "AJSpriteFrame.h"
#include "AJTexture2D.h"
#include "G2DMacros.h"

namespace Aphid {
	SpriteFrame::SpriteFrame(ATF::PassRefPtr<Texture2D> texture, const Rect& rectInPixels, bool rotated, const Point& offset, const Size& originalSizeInPixels)
	: m_texture(texture)
	, m_rectInPixels(rectInPixels)
	, m_rotated(rotated)
	, m_offsetInPixels(offset)
	, m_originalSizeInPixels(originalSizeInPixels)
	{
		m_rect = toPoints(m_rectInPixels);
		
		LEAK_DETECT_INC("SpriteFrame");
	}
	
	SpriteFrame::~SpriteFrame()
	{
		LEAK_DETECT_DEC("SpriteFrame");
	}
	
	PassRefPtr<SpriteFrame> SpriteFrame::create(PassRefPtr<Texture2D> texture, const Rect& rectInPoints)
	{
		Rect rectInPixels = toPixels(rectInPoints);
		return create(texture, rectInPixels, false, PointZero, rectInPixels.size);
	}
	
	void SpriteFrame::setRect(const Rect& rect)
	{
		m_rect = rect;
		m_rectInPixels = toPixels(rect);
	}
	
	void SpriteFrame::setRectInPixels(const Rect& rect)
	{
		m_rectInPixels = rect;
		m_rect = toPoints(rect);
	}
	
	void SpriteFrame::markObjects(AJ::MarkStack &markStack, unsigned int markID)
	{
		if (isMarked(markID))
			return;
		DirectWrapper<AJSpriteFrame>::markObjects(markStack, markID);
		if (m_texture)
			m_texture->markObjects(markStack, markID);
	}
}
