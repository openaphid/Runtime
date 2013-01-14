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
#include "TextCodec.h"
#include <wtf/StringExtras.h>

namespace Aphid {
	using Aphid::String;
	
	TextCodec::~TextCodec()
	{
	}
	
	int TextCodec::getUnencodableReplacement(unsigned codePoint, UnencodableHandling handling, UnencodableReplacementArray replacement)
	{
    switch (handling) {
			case QuestionMarksForUnencodables:
				replacement[0] = '?';
				replacement[1] = 0;
				return 1;
			case EntitiesForUnencodables:
				snprintf(replacement, sizeof(UnencodableReplacementArray), "&#%u;", codePoint);
				return static_cast<int>(strlen(replacement));
			case URLEncodedEntitiesForUnencodables:
				snprintf(replacement, sizeof(UnencodableReplacementArray), "%%26%%23%u%%3B", codePoint);
				return static_cast<int>(strlen(replacement));
    }
    ASSERT_NOT_REACHED();
    replacement[0] = 0;
    return 0;
	}
}
