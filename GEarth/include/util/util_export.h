#pragma once



#ifdef UTIL_LIBRARY
#ifndef UTIL_API
#define UTIL_API __declspec(dllexport)
#endif
#else
#ifndef EARTH_API
#define EARTH_API __declspec(dllimport)
#endif
#endif
