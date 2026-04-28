#pragma once

class Interval
{
public:
	f32 min, max;

	Interval();
	Interval(f32 min_, f32 max_);

	f32 Size() const;
	bool Contains(f32 value_) const;
	bool Surrounds(f32 value_) const;

	f32 Clamp(f32 value_) const;

	static const Interval empty, universe;
};