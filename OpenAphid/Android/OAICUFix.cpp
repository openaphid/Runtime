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

#include <wtf/unicode/icu/UnicodeIcu.h>
#include "OAICUFix.h"

U_STABLE UConverter* U_EXPORT2 
ucnv_open(const char *converterName, UErrorCode *err)
{
	return ucnv_open_44(converterName, err);
}

U_STABLE int32_t U_EXPORT2 
unorm_normalize(const UChar *source, int32_t sourceLength,
                UNormalizationMode mode, int32_t options,
                UChar *result, int32_t resultLength,
                UErrorCode *status)
{
	return unorm_normalize_44(source, sourceLength, mode, options, result, resultLength, status);
}

U_STABLE UNormalizationCheckResult U_EXPORT2
unorm_quickCheck(const UChar *source, int32_t sourcelength,
                 UNormalizationMode mode,
                 UErrorCode *status)
{
	return unorm_quickCheck_44(source, sourcelength, mode, status);
}

U_STABLE void U_EXPORT2 UCNV_FROM_U_CALLBACK_ESCAPE (
                  const void *context,
                  UConverterFromUnicodeArgs *fromUArgs,
                  const UChar* codeUnits,
                  int32_t length,
                  UChar32 codePoint,
                  UConverterCallbackReason reason,
                  UErrorCode * err)
{
	return UCNV_FROM_U_CALLBACK_ESCAPE_44(context, fromUArgs, codeUnits, length, codePoint, reason, err);
}

U_STABLE void U_EXPORT2 UCNV_FROM_U_CALLBACK_SUBSTITUTE (
                  const void *context,
                  UConverterFromUnicodeArgs *fromUArgs,
                  const UChar* codeUnits,
                  int32_t length,
                  UChar32 codePoint,
                  UConverterCallbackReason reason,
                  UErrorCode * err)
{
	UCNV_FROM_U_CALLBACK_SUBSTITUTE_44(context, fromUArgs, codeUnits, length, codePoint, reason, err);
}

U_STABLE void U_EXPORT2 
ucnv_fromUnicode (UConverter * converter,
                  char **target,
                  const char *targetLimit,
                  const UChar ** source,
                  const UChar * sourceLimit,
                  int32_t* offsets,
                  UBool flush,
                  UErrorCode * err)
{
	ucnv_fromUnicode_44(converter, target, targetLimit, source, sourceLimit, offsets, flush, err);
}

U_STABLE void U_EXPORT2
ucnv_setFromUCallBack (UConverter * converter,
                       UConverterFromUCallback newAction,
                       const void *newContext,
                       UConverterFromUCallback *oldAction,
                       const void **oldContext,
                       UErrorCode * err)
{
	ucnv_setFromUCallBack_44(converter, newAction, newContext, oldAction, oldContext, err);
}

U_STABLE void U_EXPORT2
ucnv_setSubstChars(UConverter *converter,
                   const char *subChars,
                   int8_t len,
                   UErrorCode *err)
{
	ucnv_setSubstChars_44(converter, subChars, len, err);
}

U_STABLE void U_EXPORT2 ucnv_cbFromUWriteUChars(UConverterFromUnicodeArgs *args,
                             const UChar** source,
                             const UChar*  sourceLimit,
                             int32_t offsetIndex,
                             UErrorCode * err)
{
	ucnv_cbFromUWriteUChars_44(args, source, sourceLimit, offsetIndex, err);
}

U_STABLE void U_EXPORT2
ucnv_cbFromUWriteBytes (UConverterFromUnicodeArgs *args,
                        const char* source,
                        int32_t length,
                        int32_t offsetIndex,
                        UErrorCode * err)
{
	ucnv_cbFromUWriteBytes_44(args, source, length, offsetIndex, err);
}

U_STABLE void U_EXPORT2 UCNV_TO_U_CALLBACK_SUBSTITUTE (
                  const void *context,
                  UConverterToUnicodeArgs *toUArgs,
                  const char* codeUnits,
                  int32_t length,
                  UConverterCallbackReason reason,
                  UErrorCode * err)
{
	UCNV_TO_U_CALLBACK_SUBSTITUTE_44(context, toUArgs, codeUnits, length, reason, err);
}

U_STABLE void U_EXPORT2
ucnv_setToUCallBack (UConverter * converter,
                     UConverterToUCallback newAction,
                     const void* newContext,
                     UConverterToUCallback *oldAction,
                     const void** oldContext,
                     UErrorCode * err)
{
	ucnv_setToUCallBack_44(converter, newAction, newContext, oldAction, oldContext, err);
}


U_STABLE void U_EXPORT2 
ucnv_toUnicode(UConverter *converter,
               UChar **target,
               const UChar *targetLimit,
               const char **source,
               const char *sourceLimit,
               int32_t *offsets,
               UBool flush,
               UErrorCode *err)
{
	ucnv_toUnicode_44(converter, target, targetLimit, source, sourceLimit, offsets, flush, err);
}

U_STABLE void U_EXPORT2 
ucnv_setFallback(UConverter *cnv, UBool usesFallback)
{
	ucnv_setFallback_44(cnv, usesFallback);
}

U_STABLE void  U_EXPORT2
ucnv_close(UConverter * converter)
{
	ucnv_close_44(converter);
}

U_STABLE int32_t U_EXPORT2
uidna_IDNToASCII(  const UChar* src, int32_t srcLength,
                   UChar* dest, int32_t destCapacity,
                   int32_t options,
                   UParseError* parseError,
                   UErrorCode* status)
{
	return uidna_IDNToASCII_44(src, srcLength, dest, destCapacity, options, parseError, status);
}
