#ifndef OA_ANDROID_LOG_H
#define OA_ANDROID_LOG_H

#include "platform.h"

#if OS(ANDROID)

#include <stdio.h>
#include <stdarg.h>
#include <android/log.h>

#define AJ_ANDROID_TAG "OpenAphid"

#define printf(...) __android_log_print(ANDROID_LOG_DEBUG, AJ_ANDROID_TAG, __VA_ARGS__)

#define vfprintf(stream, format, args) aj_android_vfprintf(stream, format, args)

//static void aj_android_printf(const char* format,...);
void aj_android_vfprintf(FILE* stream, const char* format, va_list arg);

#endif //OS(ANDROID)

#endif