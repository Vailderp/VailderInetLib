#pragma once
#ifdef VIL_EXPORTS
#define VIL_API __declspec(dllexport)
#else
#define VIL_API __declspec(dllimport)
#endif

// NAMESPACE
#define _VIL_BEGIN namespace vil \
{
#define _VIL_END }
#define _VIL vil::