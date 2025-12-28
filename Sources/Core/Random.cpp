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