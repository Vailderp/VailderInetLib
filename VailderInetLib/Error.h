#pragma once

#include <iostream>

template<typename _Type>
constexpr void error(_Type type)
{
	std::cout << (type);
}

template<typename... _Types>
constexpr void error(_Types... types)
{
	std::cout << error(types...);
}

