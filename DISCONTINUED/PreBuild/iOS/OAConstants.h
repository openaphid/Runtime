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

#ifndef OpenAphid_OAConstants_h
#define OpenAphid_OAConstants_h

typedef enum {
	OAOrientationMaskPortrait = 1,
	OAOrientationMaskPortraitUpsideDown = 1 << 1,
	OAOrientationMaskLandscapeLeft = 1 << 2,
	OAOrientationMaskLandscapeRight = 1 << 3,
	OAOrientationPortrait = OAOrientationMaskPortrait | OAOrientationMaskPortraitUpsideDown,
	OAOrientationLandscape = OAOrientationMaskLandscapeLeft | OAOrientationMaskLandscapeRight,
	OAOrientationAll = OAOrientationPortrait | OAOrientationLandscape
} OAOrientationMask;

typedef enum {
	OAGLViewPixelFormatRGB565 = 0,
	OAGLViewPixelFormatRGBA8 = 1
} OAGLViewPixelFormat;

#endif
