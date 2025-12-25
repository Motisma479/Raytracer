#include "Hittable/Sphere.hpp"

Sphere::Sphere(const Maths::Vec3& center_, f32 radius_) : _center(center_), _radius(radius_) {}

bool Sphere::Hit(const Ray& ray_, f32 rayTMin_, f32 rayTMax_, HitRecord& record_) const
{
	Maths::Vec3 oc = _center - ray_.orig;
	f32 a = ray_.dir.GetMagnitudeSquared();
	f32 h = Maths::Vectors::DotProduct(ray_.dir, oc);
	f32 c = oc.GetMagnitudeSquared() - _radius * _radius;

	f32 discriminant = h * h - a * c;
	if (discriminant < 0) return false;
	f32 sqrtD = std::sqrt(discriminant);

	f32 root = (h - sqrtD) / a;
	if (root <= rayTMin_ || rayTMax_ <= root)
	{
		root = (h + sqrtD) / a;
		if (root <= rayTMin_ || rayTMax_ <= root) return false;
	}

	record_.t = root;
	record_.p = ray_.At(record_.t);
	
	Maths::Vec3 outwardNormal = (record_.p - _center) / _radius;//TODO if not used later maybe dont create a varible
	record_.SetFaceNormal(ray_, outwardNormal);
	
	return true;
}