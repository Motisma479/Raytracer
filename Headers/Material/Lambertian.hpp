#pragma once

#include "Core/Material.hpp"

class Lambertian : public Material
{
public:
	Lambertian(const Color& albedo_);

	bool Scatter(const Ray& in_, const HitRecord& rec_, Color& attenuation_, Ray& scattered_) override;
private:
	Color _albedo;
};