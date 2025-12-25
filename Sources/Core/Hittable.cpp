#include "Core/Hittable.hpp"

void HitRecord::SetFaceNormal(const Ray& ray_, const Maths::Vec3& outwardNormal_)
{
	frontFace = Maths::Vectors::DotProduct(ray_.dir, outwardNormal_) < 0;
	normal = frontFace ? outwardNormal_ : -outwardNormal_;
}