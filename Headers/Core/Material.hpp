#pragma once

#include "Core/Hittable.hpp"
#include "Core/Color.hpp"

class Material
{
public:
	virtual bool Scatter(const Ray& in_, const HitRecord& rec_, Color& attenuation_, Ray& scattered_) { return false; }
};