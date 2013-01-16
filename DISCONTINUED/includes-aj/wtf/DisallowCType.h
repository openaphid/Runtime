
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
 * Copyright (C) 2007 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WTF_DisallowCType_h
#define WTF_DisallowCType_h

#include "Platform.h"

#define _DONT_USE_CTYPE_INLINE_

// The behavior of many of the functions in the <ctype.h> header is dependent
// on the current locale. But almost all uses of these functions are for
// locale-independent, ASCII-specific purposes. In WebKit code we use our own
// ASCII-specific functions instead. This header makes sure we get a compile-time
// error if we use one of the <ctype.h> functions by accident.

#include <ctype.h>

#undef isalnum
#undef isalpha
#undef isascii
#undef isblank
#undef iscntrl
#undef isdigit
#undef isgraph
#undef islower
#undef isprint
#undef ispunct
#undef isspace
#undef isupper
#undef isxdigit
#undef toascii
#undef tolower
#undef toupper

#define isalnum isalnum_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isalpha isalpha_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isascii isascii_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isblank isblank_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define iscntrl iscntrl_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isdigit isdigit_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isgraph isgraph_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define islower islower_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isprint isprint_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define ispunct ispunct_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isspace isspace_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isupper isupper_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define isxdigit isxdigit_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define toascii toascii_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define tolower tolower_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h
#define toupper toupper_WTF_Please_use_ASCIICType_instead_of_ctype_see_comment_in_ASCIICType_h

#endif
