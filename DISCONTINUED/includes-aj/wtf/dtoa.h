
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
 *  Copyright (C) 2003, 2008 Apple Inc. All rights reserved.
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

#ifndef WTF_dtoa_h
#define WTF_dtoa_h

namespace ATF {
    class Mutex;
}

namespace ATF {

    extern ATF::Mutex* s_dtoaP5Mutex;

    double strtod(const char* s00, char** se);

    typedef char DtoaBuffer[80];
    void dtoa(DtoaBuffer result, double d, int ndigits, int* decpt, int* sign, char** rve);

    // dtoa() for ECMA-262 'ToString Applied to the Number Type.'
    // The *resultLength will have the length of the resultant string in bufer.
    // The resultant string isn't terminated by 0.
    void doubleToStringInAJFormat(double, DtoaBuffer, unsigned* resultLength);

} // namespace ATF

using ATF::DtoaBuffer;
using ATF::doubleToStringInAJFormat;

#endif // WTF_dtoa_h
