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

	bool Hit(const Ray& ray_, f32 rayTMin_, f32 rayTMax_, HitRecord& record_) const override;
};