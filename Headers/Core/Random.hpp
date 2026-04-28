#pragma once

//Use a PCG generator
class RNG
{
public:
	RNG(u64 seed_);
	static RNG& Get();

	u32 NextUInt();

	f32 NextFloat(f32 min_ = 0.f, f32 max_ = 1.f);
	s32 NextInt(s32 min_ = 0, s32 max_ = 100);
	Maths::Vec3 NextVec3(f32 min_ = 0.f, f32 max_ = 1.f);
	Maths::Vec3 NextVec3Unit();
	
	Maths::Vec3 NextOnHemisphere(const Maths::Vec3 normal_);

private:
	u64 _state;
	u64 _inc;
};