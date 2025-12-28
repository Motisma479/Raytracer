#pragma once
#include <Core/Type.hpp>

//Use a PCG generator
class RNG
{
public:
	RNG(u64 seed_);
	u32 NextUInt();
	f32 NextFloat(f32 min_ = 0.f, f32 max_ = 1.f);
	s32 NextInt(s32 min_ = 0, s32 max_ = 100);
private:
	u64 _state;
	u64 _inc;
};