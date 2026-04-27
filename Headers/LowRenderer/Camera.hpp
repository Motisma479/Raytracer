#pragma once

#include <LibMath/Maths.hpp>
#include "Core/Color.hpp"
#include "Core/Random.hpp"
#include <vector>

class IHittable;
class Ray;

class Camera
{
public:
	Camera();
	Camera(f32 aspectRatio_, s32 imageWidth_);
	
	void Render(const IHittable& object_, const f32 minJitering = -0.5f, const f32 maxJitering = 0.5f, bool useHistory = true);
	void ScreenShot();

	inline const std::vector<Color>& GetData() const { return image; }

	void Resize(f32 newWidth_, f32 newHeight_);

private:
	f32 _aspectRatio;

	s32 _imageWidth;
	s32 _imageHeight;
	s32 _maxDepth = 10;

	Maths::Vec3 _center;
	Maths::Vec3 _pixel00Loc;
	Maths::Vec3 _pixelDeltaU;
	Maths::Vec3 _pixelDeltaV;

	std::vector<Color> image;
	std::vector<Color> preImage;

	void Init();
	Color RayColor(const Ray& ray_, s32 depth_, const IHittable& object_) const;
};