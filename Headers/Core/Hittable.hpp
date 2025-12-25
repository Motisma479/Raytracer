#pragma once
#include "Core/Ray.hpp"

class HitRecord
{
public:
	Maths::Vec3 p;
	Maths::Vec3 normal;
	f32 t;
	bool frontFace;

	void SetFaceNormal(const Ray& ray_, const Maths::Vec3& outwardNormal_);
};

class IHittable
{
public:
	virtual bool Hit(const Ray& ray_, f32 rayTMin_, f32 rayTMax_, HitRecord& record_) const = 0;
};