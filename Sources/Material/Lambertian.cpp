#include "Material/Lambertian.hpp"
#include "Core/Random.hpp"

Lambertian::Lambertian(const Color& albedo_) : _albedo(albedo_) {}

bool Lambertian::Scatter(const Ray& in_, const HitRecord& rec_, Color& attenuation_, Ray& scattered_)
{
    Maths::Vec3 scatterDirection = rec_.normal + RNG::Get().NextVec3Unit();

    if (scatterDirection.IsNearZero())
        scatterDirection = rec_.normal;

    scattered_ = Ray(rec_.p, scatterDirection);
    attenuation_ = _albedo;
    return true;
}
