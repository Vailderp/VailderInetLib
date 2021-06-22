#pragma once
#include "vildefs.h"

using packetc_t = const char*;
using packet_t = char*;

_VIL_BEGIN

template<typename _Value_Type>
constexpr packetc_t (enpackc) (_Value_Type type)
{
	return reinterpret_cast<packetc_t>(type);
}

template<typename _Value_Type>
constexpr packet_t (enpack) (_Value_Type type)
{
	return reinterpret_cast<packet_t>(type);
}

template<typename _Value_Type>
constexpr _Value_Type* (unpackc) (packetc_t type)
{
	return reinterpret_cast<_Value_Type*>(type);
}

template<typename _Value_Type>
constexpr _Value_Type* (unpack) (packet_t type)
{
	return reinterpret_cast<_Value_Type*>(type);
}

_VIL_END