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

#ifndef OpenAphid_OAData_h
#define OpenAphid_OAData_h

#include <CoreGraphics/CGGeometry.h>
#include <CoreGraphics/CGAffineTransform.h>

#include "OAUtil.h"

namespace Aphid {
	typedef CGPoint Point;
	typedef CGSize Size;
	typedef CGRect Rect;
	typedef CGAffineTransform AffineTransform;
	typedef float TimeSec;
	
	struct Vector2 {
		float x;
		float y;
		
		float length() const;
	};
	
	struct Vector3 {
		float x;
		float y;
		float z;
	};
	
	struct GridSize {
		int x;
		int y;
	};
	
	extern const Point PointZero;
	extern const Size SizeZero;
	extern const Rect RectZero;
	extern const AffineTransform AffineTransformIdentity;
	extern const Vector2 Vector2Zero;
	extern const Vector2 Vector2Identity;
	extern const Vector3 Vector3Zero;
	
#define PointEqual(p1, p2) CGPointEqualToPoint((p1), (p2))
#define SizeEqual(s1, s2) CGSizeEqualToSize((s1), (s2))
#define RectEqual(r1, r2) CGRectEqualToRect((r1), (r2))
#define AffineTransformIsIdentity(f1) CGAffineTransformIsIdentity((f1))

#define RectMake(x, y, w, h) CGRectMake((x), (y), (w), (h))
#define PointMake(x, y) CGPointMake((x), (y))
#define SizeMake(w, h) CGSizeMake((w), (h))
#define AffineTransformMake(a, b, c, d, tx, ty)	CGAffineTransformMake((a), (b), (c), (d), (tx), (ty))
#define AffineTransformTranslate(t, tx, ty) CGAffineTransformTranslate((t), (tx), (ty))
#define AffineTransformRotate(t, angel) CGAffineTransformRotate((t), (angel))
#define AffineTransformConcat(t1, t2) CGAffineTransformConcat((t1), (t2))
#define AffineTransformScale(t, sx, sy)	CGAffineTransformScale((t), (sx), (sy))
#define AffineTransformInvert(t) CGAffineTransformInvert((t))
#define PointApplyAffineTransform(p, t) CGPointApplyAffineTransform((p), (t))
	
#define RectApplyAffineTransform(r, t) CGRectApplyAffineTransform((r), (t))
	
#define RectContainsPoint(r, p) CGRectContainsPoint((r), (p))
	
	static inline Vector2 Vector2Make(float x, float y)
	{
		Vector2 v;
		v.x = x;
		v.y = y;
		
		return v;
	}
	
	static inline Vector3 Vector3Make(float x, float y, float z)
	{
		Vector3 v;
		v.x = x;
		v.y = y;
		v.z = z;
		
		return v;
	}
	
	static inline Rect RectMake2(const Point& origin, const Size& size)
	{
		return RectMake(origin.x, origin.y, size.width, size.height);
	}
	
	static bool Vector2Equal(const Vector2& v1, const Vector2& v2)
	{
		return v1.x == v2.x && v1.y == v2.y;
	}
	
	static bool Vector3Equal(const Vector3& v1, const Vector3& v2)
	{
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
	}
	
	static OA_RELEASE_INLINE Vector2 operator-(const Vector2 &v1, const Vector2 &v2)
	{
		Vector2 v = {v1.x - v2.x, v1.y - v2.y};
		return v;
	}
	
	static OA_RELEASE_INLINE Vector3 operator/(const Vector3 &v, float d)
	{
		Vector3 r = v;
		r.x /= d;
		r.y /= d;
		r.z /= d;
		return r;
	}
	
	static OA_RELEASE_INLINE Vector3 operator*(const Vector3 &v, float d)
	{
		Vector3 r = v;
		r.x *= d;
		r.y *= d;
		r.z *= d;
		return r;
	}
	
	static OA_RELEASE_INLINE Size operator*(const Size& s, float v) 
	{
		return (Size){s.width * v, s.height * v};
	}
	
	static Size toSize(const GridSize& gs)
	{
		return (Size){gs.x, gs.y};
	}
	
	static GridSize toGridSize(const Size& s)
	{
		return (GridSize){(int)s.width, (int)s.height};
	}	
}

#endif
