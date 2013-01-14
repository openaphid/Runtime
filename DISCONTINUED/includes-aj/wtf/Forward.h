
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
/*
 *  Copyright (C) 2006, 2009 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef WTF_Forward_h
#define WTF_Forward_h

#include <stddef.h>

namespace ATF {
    template<typename T> class ListRefPtr;
    template<typename T> class OwnArrayPtr;
    template<typename T> class OwnPtr;
    template<typename T> class PassOwnPtr;
    template<typename T> class PassRefPtr;
    template<typename T> class RefPtr;
    template<typename T, size_t inlineCapacity> class Vector;
}

using ATF::ListRefPtr;
using ATF::OwnArrayPtr;
using ATF::OwnPtr;
using ATF::PassOwnPtr;
using ATF::PassRefPtr;
using ATF::RefPtr;
using ATF::Vector;

#endif // WTF_Forward_h
