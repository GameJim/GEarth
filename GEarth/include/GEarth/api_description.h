#pragma once

#ifdef GEarth_EXPORTS
#ifndef EARTH_API
#define EARTH_API __declspec(dllexport)
#endif
#else
#ifndef EARTH_API
#define EARTH_API __declspec(dllimport)
#endif
#endif



//宏定义，可能取消的函数结构
#ifdef __GNUC__
#    define GCC_VERSION_AT_LEAST(x,y) (__GNUC__ > (x) || __GNUC__ == (x) && __GNUC_MINOR__ >= (y))
#else
#    define GCC_VERSION_AT_LEAST(x,y) 0
#endif

#if GCC_VERSION_AT_LEAST(3,1)
#    define ATTRIBUTE_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#    define ATTRIBUTE_DEPRECATED __declspec(deprecated)
#else
#    define ATTRIBUTE_DEPRECATED
#endif

