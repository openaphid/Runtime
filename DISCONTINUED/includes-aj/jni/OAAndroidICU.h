#include <wtf/unicode/Unicode.h>

U_STABLE int8_t U_EXPORT2
u_charType_44(UChar32 c);


U_STABLE int32_t U_EXPORT2
u_strToLower_44(UChar *dest, int32_t destCapacity,
             const UChar *src, int32_t srcLength,
             const char *locale,
             UErrorCode *pErrorCode);

U_STABLE int32_t U_EXPORT2
u_strToUpper_44(UChar *dest, int32_t destCapacity,
             const UChar *src, int32_t srcLength,
             const char *locale,
             UErrorCode *pErrorCode);

U_STABLE UChar32 U_EXPORT2
u_foldCase_44(UChar32 c, uint32_t options);

U_STABLE int32_t U_EXPORT2
u_strFoldCase_44(UChar *dest, int32_t destCapacity,
              const UChar *src, int32_t srcLength,
              uint32_t options,
              UErrorCode *pErrorCode);

U_STABLE UCharDirection U_EXPORT2
u_charDirection_44(UChar32 c);

U_STABLE int32_t U_EXPORT2
u_memcasecmp_44(const UChar *s1, const UChar *s2, int32_t length, uint32_t options);

U_STABLE UBool U_EXPORT2
u_isspace_44(UChar32 c);

U_STABLE UChar32 U_EXPORT2
u_tolower_44(UChar32 c);

U_STABLE UChar32 U_EXPORT2
u_toupper_44(UChar32 c);

