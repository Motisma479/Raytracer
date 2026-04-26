#include "Core/Random.hpp"

RNG::RNG(u64 seed_) : _state(seed_), _inc(1442695040888963407u) {}

u32 RNG::NextUInt()
{
	u64 oldState = _state;
	_state = oldState * 6364136223846793005u + _inc;
	u32 xorshifted = ((oldState >> 18u) ^ oldState) >> 27u;
	u32 rot = oldState >> 59u;
	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

f32 RNG::NextFloat(f32 min_, f32 max_)
{
	f32 r = static_cast<f32>(NextUInt()) / static_cast<f32>(0xFFFFFFFFu);
	return min_ + r * (max_ - min_);
}

s32 RNG::NextInt(s32 min_, s32 max_)
{
	return min_ + static_cast<s32>(NextUInt() % (max_ - min_ + 1));
}

Maths::Vec3 RNG::NextVec3(f32 min_, f32 max_)
{
	return { NextFloat(min_, max_), NextFloat(min_, max_), NextFloat(min_, max_) };
}

Maths::Vec3 RNG::NextVec3Unit()
{
	while (true)
	{
		Maths::Vec3 p = NextVec3(-1, 1);
		float lensq = p.GetMagnitudeSquared();
		if (1e-160 < lensq && lensq <= 1)
			return p / p.GetMagnitude();
	}
}

Maths::Vec3 RNG::NextOnHemisphere(const Maths::Vec3 normal_)
{
	Maths::Vec3 on_unit_sphere = NextVec3Unit();
	if (Maths::Vectors::DotProduct(on_unit_sphere, normal_) > 0.0) // In the same hemisphere as the normal
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}