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

bool HittableList::Hit(const Ray& ray_, f32 rayTMin_, f32 rayTMax_, HitRecord& record_) const
{
	bool asHitAnything = false;
	f32 closestSoFar = rayTMax_;
	HitRecord tempRecord;

	for (const std::shared_ptr<IHittable>& object : objects)
	{
		if (object->Hit(ray_, rayTMin_, closestSoFar, tempRecord))
		{
			asHitAnything = true;
			closestSoFar = tempRecord.t;
			record_ = tempRecord;
		}
	}

	return asHitAnything;
}