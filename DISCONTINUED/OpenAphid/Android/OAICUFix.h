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

#ifndef OAICUFIX_H_
#define OAICUFIX_H_

#include <wtf/unicode/Unicode.h>
#include <icu/unicode/ucnv_err.h>
#include <icu/unicode/unorm.h>
#include <icu/unicode/ucnv.h>
#include <icu/unicode/parseerr.h>

U_STABLE UConverter* U_EXPORT2 
ucnv_open_44(const char *converterName, UErrorCode *err);

U_STABLE int32_t U_EXPORT2 
unorm_normalize_44(const UChar *source, int32_t sourceLength,
                UNormalizationMode mode, int32_t options,
                UChar *result, int32_t resultLength,
                UErrorCode *status);

U_STABLE UNormalizationCheckResult U_EXPORT2
unorm_quickCheck_44(const UChar *source, int32_t sourcelength,
                 UNormalizationMode mode,
                 UErrorCode *status);

U_STABLE void U_EXPORT2
UCNV_FROM_U_CALLBACK_ESCAPE_44(
                  const void *context,
                  UConverterFromUnicodeArgs *fromUArgs,
                  const UChar* codeUnits,
                  int32_t length,
                  UChar32 codePoint,
                  UConverterCallbackReason reason,
                  UErrorCode * err);

U_STABLE void U_EXPORT2 UCNV_FROM_U_CALLBACK_SUBSTITUTE_44 (
                  const void *context,
                  UConverterFromUnicodeArgs *fromUArgs,
                  const UChar* codeUnits,
                  int32_t length,
                  UChar32 codePoint,
                  UConverterCallbackReason reason,
                  UErrorCode * err);

U_STABLE void U_EXPORT2 
ucnv_fromUnicode_44 (UConverter * converter,
                  char **target,
                  const char *targetLimit,
                  const UChar ** source,
                  const UChar * sourceLimit,
                  int32_t* offsets,
                  UBool flush,
                  UErrorCode * err);

U_STABLE void U_EXPORT2
ucnv_setFromUCallBack_44 (UConverter * converter,
                       UConverterFromUCallback newAction,
                       const void *newContext,
                       UConverterFromUCallback *oldAction,
                       const void **oldContext,
                       UErrorCode * err);

U_STABLE void U_EXPORT2
ucnv_setSubstChars_44(UConverter *converter,
                   const char *subChars,
                   int8_t len,
                   UErrorCode *err);

U_STABLE void U_EXPORT2 ucnv_cbFromUWriteUChars_44(UConverterFromUnicodeArgs *args,
                             const UChar** source,
                             const UChar*  sourceLimit,
                             int32_t offsetIndex,
                             UErrorCode * err);

U_STABLE void U_EXPORT2
ucnv_cbFromUWriteBytes_44 (UConverterFromUnicodeArgs *args,
                        const char* source,
                        int32_t length,
                        int32_t offsetIndex,
                        UErrorCode * err);

U_STABLE void U_EXPORT2 UCNV_TO_U_CALLBACK_SUBSTITUTE_44 (
                  const void *context,
                  UConverterToUnicodeArgs *toUArgs,
                  const char* codeUnits,
                  int32_t length,
                  UConverterCallbackReason reason,
                  UErrorCode * err);

U_STABLE void U_EXPORT2
ucnv_setToUCallBack_44 (UConverter * converter,
                     UConverterToUCallback newAction,
                     const void* newContext,
                     UConverterToUCallback *oldAction,
                     const void** oldContext,
                     UErrorCode * err);

U_STABLE void U_EXPORT2 
ucnv_toUnicode_44(UConverter *converter,
               UChar **target,
               const UChar *targetLimit,
               const char **source,
               const char *sourceLimit,
               int32_t *offsets,
               UBool flush,
               UErrorCode *err);

U_STABLE void U_EXPORT2 
ucnv_setFallback_44(UConverter *cnv, UBool usesFallback);

U_STABLE void  U_EXPORT2
ucnv_close_44(UConverter * converter);

U_STABLE int32_t U_EXPORT2
uidna_IDNToASCII_44(  const UChar* src, int32_t srcLength,
                   UChar* dest, int32_t destCapacity,
                   int32_t options,
                   UParseError* parseError,
                   UErrorCode* status);


#endif /* OAICUFIX_H_ */
