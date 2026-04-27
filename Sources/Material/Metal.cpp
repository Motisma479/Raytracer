#include "Material/Metal.hpp"
#include "Core/Random.hpp"

Metal::Metal(const Color& albedo_) : _albedo(albedo_) {}

bool Metal::Scatter(const Ray& in_, const HitRecord& rec_, Color& attenuation_, Ray& scattered_)
{
    static auto reflect = [](const Maths::Vec3& v, const Maths::Vec3& n) {return v - (n * (2 * Maths::Vectors::DotProduct(v, n))); };
    Maths::Vec3 reflected = reflect(in_.dir, rec_.normal);

    scattered_ = Ray(rec_.p, reflected);
    attenuation_ = _albedo;
    return true;
}
