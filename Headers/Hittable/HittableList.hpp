#pragma once
#include "Core/Hittable.hpp"

#include <memory>
#include <vector>

class HittableList : public IHittable
{
public:
	std::vector<std::shared_ptr<IHittable>> objects;

	HittableList();

	void Add(std::shared_ptr<IHittable> object_);
	void Clear();

	bool Hit(const Ray& ray_, Interval rayT_, HitRecord& record_) const override;
};