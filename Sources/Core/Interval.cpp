#include "Core/Interval.hpp"
#include <LibMath/Maths.hpp>

const Interval Interval::empty = Interval(+Maths::Constants::INF, -Maths::Constants::INF);
const Interval Interval::universe = Interval(-Maths::Constants::INF, +Maths::Constants::INF);

Interval::Interval() : min(+Maths::Constants::INF), max(-Maths::Constants::INF) {}
Interval::Interval(f32 min_, f32 max_) : min(min_), max(max_) {}

f32 Interval::Size()
{
	return max - min;
}

bool Interval::Contains(f32 value_)
{
	return min <= value_ && value_ <= max;
}

bool Interval::Surrounds(f32 value_)
{
	return min < value_ && value_ < max;
}