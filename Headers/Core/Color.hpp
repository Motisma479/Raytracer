#pragma once
#include "Core/Type.hpp"
#include <string>

struct Color
{
	f32 r, g, b;

	std::string RGB255_str();
};