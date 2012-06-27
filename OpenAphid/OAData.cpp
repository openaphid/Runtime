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
#include <math.h>

#include "OAData.h"

namespace Aphid {
#if PLATFORM(IPHONE)
	const Point PointZero = CGPointZero;
	const Size SizeZero = CGSizeZero;
	const Rect RectZero = CGRectZero;
	const AffineTransform AffineTransformIdentity = CGAffineTransformIdentity;
#elif PLATFORM(ANDROID)
	const Point PointZero = {0.0f, 0.0f};
	const Size SizeZero = {0.0f, 0.0f};
	const Rect RectZero = {PointZero, SizeZero};
	const AffineTransform AffineTransformIdentity = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
#else
#error unsupported platform
#endif
	const Vector2 Vector2Zero = {0, 0};
	const Vector2 Vector2Identity = {1, 1};
	const Vector3 Vector3Zero = {0, 0, 0};
	
	float Vector2::length() const 
	{
		 return sqrtf(x*x + y*y);
	}

#if PLATFORM(ANDROID)

	Rect RectApplyAffineTransform(const Rect& r, const AffineTransform& t)
	{
		float top = r.origin.y;
    	float left = r.origin.x;
		float right = r.origin.x + r.size.width;
		float bottom = r.origin.y + r.size.height;
	
		Point topLeft = PointApplyAffineTransform(PointMake(left, top), t);
    	Point topRight = PointApplyAffineTransform(PointMake(right, top), t);
    	Point bottomLeft = PointApplyAffineTransform(PointMake(left, bottom), t);
    	Point bottomRight = PointApplyAffineTransform(PointMake(right, bottom), t);

    	float minX = fminf(fminf(topLeft.x, topRight.x), fminf(bottomLeft.x, bottomRight.x));
    	float maxX = fmaxf(fmaxf(topLeft.x, topRight.x), fmaxf(bottomLeft.x, bottomRight.x));
    	float minY = fminf(fminf(topLeft.y, topRight.y), fminf(bottomLeft.y, bottomRight.y));
    	float maxY = fmaxf(fmaxf(topLeft.y, topRight.y), fmaxf(bottomLeft.y, bottomRight.y));
        
    	return RectMake(minX, minY, (maxX - minX), (maxY - minY));
	}

	AffineTransform AffineTransformRotate(const AffineTransform& t, float angle)
	{
		float fSin = sin(angle);
		float fCos = cos(angle);

		return AffineTransformMake(	t.a * fCos + t.c * fSin,
									t.b * fCos + t.d * fSin,
									t.c * fCos - t.a * fSin,
									t.d * fCos - t.b * fSin,
									t.tx,
									t.ty);
	}
#endif
};

