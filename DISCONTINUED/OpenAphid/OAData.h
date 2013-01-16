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

#if PLATFORM(IPHONE)
#include <CoreGraphics/CGGeometry.h>
#include <CoreGraphics/CGAffineTransform.h>
#endif

#include "OAUtil.h"

namespace Aphid {
#if PLATFORM(IPHONE)
	typedef CGPoint Point;
	typedef CGSize Size;
	typedef CGRect Rect;
	typedef CGAffineTransform AffineTransform;
#endif

	typedef float TimeSec;
	
	struct Point {
		float x;
		float y;
	};

	struct Size {
		float width;
		float height;
	};

	struct Rect {
		Point origin;
		Size size;
	};

	struct AffineTransform {
		float a;
		float b;
		float c;
		float d;
		float tx;
		float ty;
	};
	
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
	
#if PLATFORM(IPHONE)
#define PointEqual(p1, p2) CGPointEqualToPoint((p1), (p2))
#define SizeEqual(s1, s2) CGSizeEqualToSize((s1), (s2))
#define RectEqual(r1, r2) CGRectEqualToRect((r1), (r2))
#define AffineTransformIsIdentity(f1) CGAffineTransformIsIdentity((f1))

#define RectMake(x, y, w, h) CGRectMake((x), (y), (w), (h))
#define PointMake(x, y) CGPointMake((x), (y))
#define SizeMake(w, h) CGSizeMake((w), (h))
#define AffineTransformMake(a, b, c, d, tx, ty)	CGAffineTransformMake((a), (b), (c), (d), (tx), (ty))
#define AffineTransformTranslate(t, tx, ty) CGAffineTransformTranslate((t), (tx), (ty))
#define AffineTransformRotate(t, angle) CGAffineTransformRotate((t), (angle))
#define AffineTransformConcat(t1, t2) CGAffineTransformConcat((t1), (t2))
#define AffineTransformScale(t, sx, sy)	CGAffineTransformScale((t), (sx), (sy))
#define AffineTransformInvert(t) CGAffineTransformInvert((t))
#define PointApplyAffineTransform(p, t) CGPointApplyAffineTransform((p), (t))
	
#define RectApplyAffineTransform(r, t) CGRectApplyAffineTransform((r), (t))
	
#define RectContainsPoint(r, p) CGRectContainsPoint((r), (p))
#else
	static inline int PointEqual(const Point& p1, const Point& p2)
	{
		return p1.x == p2.x && p1.y == p2.y;
	}

	static inline int SizeEqual(const Size& s1, const Size& s2)
	{
		return s1.width == s2.width && s1.height == s2.height;
	}

	static inline int RectEqual(const Rect& r1, const Rect& r2)
	{
		return PointEqual(r1.origin, r2.origin) && SizeEqual(r1.size, r2.size);
	}

	static inline int AffineTransformIsIdentity(const AffineTransform& t)
	{
		//TODO
		return 0;
	}

	static inline Rect RectMake(float x, float y, float w, float h)
	{
		Rect r;
		r.origin.x = x;
		r.origin.y = y;
		r.size.width = w;
		r.size.height = h;
		return r;
	}

	static inline Point PointMake(float x, float y)
	{
		return (Point){x, y};
	}

	static inline Size SizeMake(float w, float h)
	{
		return (Size){w, h};
	}

	static inline AffineTransform AffineTransformMake(float a, float b, float c, float d, float tx, float ty)
	{
		return (AffineTransform){a, b, c, d, tx, ty};
	}

	static inline AffineTransform AffineTransformTranslate(const AffineTransform& t, float tx, float ty)
	{
		return AffineTransformMake(t.a, t.b, t.c, t.d, t.tx + t.a * tx + t.c * ty, t.ty + t.b * tx + t.d * ty);
	}

	AffineTransform AffineTransformRotate(const AffineTransform& t, float angle);

	static AffineTransform AffineTransformConcat(const AffineTransform& t1, const AffineTransform& t2)
	{
		return AffineTransformMake(t1.a * t2.a + t1.b * t2.c, t1.a * t2.b + t1.b * t2.d,
									t1.c * t2.a + t1.d * t2.c, t1.c * t2.b + t1.d * t2.d,
									t1.tx * t2.a + t1.ty * t2.c + t2.tx,
									t1.tx * t2.b + t1.ty * t2.d + t2.ty);
	}

	static inline AffineTransform AffineTransformScale(const AffineTransform& t, float sx, float sy)
	{
		return AffineTransformMake(t.a * sx, t.b * sx, t.c * sy, t.d * sy, t.tx, t.ty);
	}

	static AffineTransform AffineTransformInvert(const AffineTransform& t)
	{
		float determinant = 1 / (t.a * t.d - t.b * t.c);

    	return AffineTransformMake(determinant * t.d, -determinant * t.b, -determinant * t.c, determinant * t.a,
							determinant * (t.c * t.ty - t.d * t.tx), determinant * (t.b * t.tx - t.a * t.ty) );
	}

	static Point PointApplyAffineTransform(const Point& p, const AffineTransform& t)
	{
		
  		return (Point){
  						(float)((double)t.a * p.x + (double)t.c * p.y + t.tx),
  					 	(float)((double)t.b * p.x + (double)t.d * p.y + t.ty)
  					 };
	}

	Rect RectApplyAffineTransform(const Rect& r, const AffineTransform& t);
	
	static int RectContainsPoint(const Rect& r, const Point& p)
	{
		return p.x >= r.origin.x && p.x <= (r.origin.x + r.size.width) && p.y >= r.origin.y && p.y <= (r.origin.y + r.size.height);
	}

#endif //PLATFORM(IPHONE)
	
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
