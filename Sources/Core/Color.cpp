#include "Core/Color.hpp"
#include <LibMath/Maths.hpp>

Color::Color() : r(0), g(0), b(0) {}
Color::Color(f32 r_, f32 g_, f32 b_) : r(r_), g(g_), b(b_) {}
Color::Color(const Maths::Vec3& vec_) : r(vec_.x), g(vec_.y), b(vec_.z) {}


std::string Color::RGB255_str() const
{
	return std::to_string(static_cast<s32>(255 * r)) + " " + std::to_string(static_cast<s32>(255 * g)) + " " + std::to_string(static_cast<s32>(255 * b));
}