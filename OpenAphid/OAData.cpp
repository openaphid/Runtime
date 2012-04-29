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

#include "OAData.h"

#include <math.h>

namespace Aphid {
	const Point PointZero = CGPointZero;
	const Size SizeZero = CGSizeZero;
	const Rect RectZero = CGRectZero;
	const AffineTransform AffineTransformIdentity = CGAffineTransformIdentity;
	const Vector2 Vector2Zero = {0, 0};
	const Vector2 Vector2Identity = {1, 1};
	const Vector3 Vector3Zero = {0, 0, 0};
	
	float Vector2::length() const 
	{
		 return sqrtf(x*x + y*y);
	}
};

