#include "Core/Ray.hpp"

Ray::Ray() {}
Ray::Ray(const Maths::Vec3& orig_, const Maths::Vec3& dir_) : orig(orig_), dir(dir_) {}

Maths::Vec3 Ray::At(f32 t)
{
	return orig + dir * t;
}
