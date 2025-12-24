#pragma once
#include "Core/Type.hpp"
#include <string>

namespace Maths
{
	class Vec3;
}

class Color
{
public:
	f32 r, g, b;

	Color();
	Color(f32 r_, f32 g_, f32 b_);
	Color(const Maths::Vec3& vec_);

	std::string RGB255_str() const;
};