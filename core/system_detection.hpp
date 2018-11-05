/**
* Copyright (C), 2017-2018, 北京钜石科技有限公司
* FileName: SystemDetection
* Author:   aiyong
* Date:     2018/7/22 11:02
*/
#pragma once


#if defined(__APPLE__)
    #define OS_DARWIN
    #ifdef __LP64__
        #define OS_DARWIN64
    #else
        #define OS_DARWIN32
    #endif
#elif defined(_MSC_VER)
    #define OS_WIN
    #if define(_WIN64)
        #define OS_WIN64
    #else
        #define OS_WIN32
    #endif
#elif defined(__linux__)
    #define OS_LINUX
#else
    #  error "TianXing has not been ported to this OS - see http://www.justone.tech/"
#endif




