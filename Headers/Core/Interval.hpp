#pragma once
#include "Core/Type.hpp"

class Interval
{
public:
	f32 min, max;

	Interval();
	Interval(f32 min_, f32 max_);

	f32 Size();
	bool Contains(f32 value_);
	bool Surrounds(f32 value_);

	static const Interval empty, universe;
};