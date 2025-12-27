#include "Hittable/Sphere.hpp"

Sphere::Sphere(const Maths::Vec3& center_, f32 radius_) : _center(center_), _radius(std::fmax(0,radius_)) {}

float HitSphere(const Maths::Vec3& center_, f32 radius_, const Ray& ray_)
{
	Maths::Vec3 oc = center_ - ray_.orig;
	f32 a = Maths::Vectors::DotProduct(ray_.dir, ray_.dir);
	f32 b = -2.f * Maths::Vectors::DotProduct(ray_.dir, oc);
	f32 c = Maths::Vectors::DotProduct(oc, oc) - radius_ * radius_;
	f32 discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return -1.f;
	return (-b - std::sqrt(discriminant)) / (2.f * a);
}

bool Sphere::Hit(const Ray& ray_, Interval rayT_, HitRecord& record_) const
{
	Maths::Vec3 oc = _center - ray_.orig;
	f32 a = ray_.dir.GetMagnitudeSquared();
	f32 h = Maths::Vectors::DotProduct(ray_.dir, oc);
	f32 c = oc.GetMagnitudeSquared() - _radius * _radius;
	f32 discriminant = h * h - a * c; 
	
	if (discriminant < 0) return false;
	f32 sqrtD = std::sqrt(discriminant);

	f32 root = (h - sqrtD) / a;
	if (!rayT_.Surrounds(root))
	{
		root = (h + sqrtD) / a;
		if (!rayT_.Surrounds(root)) return false;
	}

	record_.t = root;
	record_.p = ray_.At(record_.t);
	
	Maths::Vec3 outwardNormal = (record_.p - _center) / _radius;//TODO if not used later maybe dont create a varible
	record_.SetFaceNormal(ray_, outwardNormal);
	
	return true;
}