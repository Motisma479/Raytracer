#include "Hittable/HittableList.hpp"

HittableList::HittableList() {}

void HittableList::Add(std::shared_ptr<IHittable> object_)
{
	objects.push_back(object_);
}

void HittableList::Clear()
{
	objects.clear();
}

bool HittableList::Hit(const Ray& ray_, Interval rayT_, HitRecord& record_) const
{
	bool asHitAnything = false;
	f32 closestSoFar = rayT_.max;
	HitRecord tempRecord;

	for (const std::shared_ptr<IHittable>& object : objects)
	{
		if (object->Hit(ray_, Interval(rayT_.min, closestSoFar), tempRecord))
		{
			asHitAnything = true;
			closestSoFar = tempRecord.t;
			record_ = tempRecord;
		}
	}

	return asHitAnything;
}