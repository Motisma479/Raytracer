#pragma once

#include <LibMath/Maths.hpp>
#include "Core/Color.hpp"

class IHittable;
class Ray;

class Camera
{
public:
	Camera();
	Camera(f32 aspectRatio_, s32 imageWidth_);
	
	void Render(const IHittable& object_);

private:
	f32 _aspectRatio;

	s32 _imageWidth;
	s32 _imageHeight;

	Maths::Vec3 _center;
	Maths::Vec3 _pixel00Loc;
	Maths::Vec3 _pixelDeltaU;
	Maths::Vec3 _pixelDeltaV;

	void Init();
	Color RayColor(const Ray& ray_, const IHittable& object_) const;
};