#pragma once
#include <LibMath/Maths.hpp>
#include "Core/Type.hpp"

class Ray
{
public:
	Maths::Vec3 orig;
	Maths::Vec3 dir;

	Ray();
	Ray(const Maths::Vec3& orig_, const Maths::Vec3& dir_);

	Maths::Vec3 At(f32 t) const;
};