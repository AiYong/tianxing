/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: Platform
* Author:   aiyong
* Date:     2018/7/22 10:51
*/
#pragma once

#include <stdint.h>
#include "system_detection.hpp"

#if defined(OS_DARWIN) || defined(OS_LINUX)
    typedef int Handle;
    #define InvalidHandle -1
    typedef int Socket;
#elif defined(OS_WIN)
    extern "C" {
    #include <Windows.h>
    }
    typedef HANDLE Handle;
    #define InvalidHandle  INVALID_HANDLE
    typedef SOCKET Socket;
#endif

typedef uint8_t        u8;
typedef uint16_t       u16;
typedef uint32_t       u32;
typedef uint64_t       u64;
typedef int8_t          s8;
typedef int16_t         s16;
typedef int32_t         s32;
typedef int64_t         s64;


#define MAX_SHARD_COUNT 256
