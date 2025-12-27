#pragma once
#include "Core/Hittable.hpp"

class Sphere : public IHittable
{
public:
	Sphere(const Maths::Vec3& center_, f32 radius_);
	bool Hit(const Ray& ray_, Interval rayT_, HitRecord& record_) const override;
private:
	Maths::Vec3 _center;
	f32 _radius;
};