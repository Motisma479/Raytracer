#include "LowRenderer/Camera.hpp"
#include "Core/Logger.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

#include "Core/Ray.hpp"
#include "Core/Hittable.hpp"
#include "Core/Settings.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

//TODO move this function to a UtilsFunction.hpp/cpp file
std::string ProgressBar(const char* full_, const char* empty_, int charLength_, int progress_)
{
    s32 fullLength = progress_ * charLength_ / 100;
    s32 emptyLength = charLength_ - fullLength;
    std::string result;
    for (int i = 0; i < fullLength; i++) result += full_;
    for (int i = 0; i < emptyLength; i++) result += empty_;
    return result;
}

//TODO move this function to a UtilsFunction.hpp/cpp file
//this could be better but since it should not be used to often it should be ok.
//it use a string to count the digit so allocations are made
s32 NumberOfDigit(s32 number_)
{
    return std::to_string(number_).length();
}


Camera::Camera() : _aspectRatio(1.f), _imageWidth(100)
{
    Init();
}

Camera::Camera(f32 aspectRatio_, s32 imageWidth_) : _aspectRatio(aspectRatio_), _imageWidth(imageWidth_)
{
    Init();
}

void Camera::Render(const IHittable& object_, const f32 minJitering, const f32 maxJitering, bool useHistory)
{   
    for (s32 j = 0; j < _imageHeight; j++) {
        for (s32 i = 0; i < _imageWidth; i++) {
            const u32 pixelIndex = j * _imageWidth + i;

            f32 iJitering, jJitering;
            if (minJitering == maxJitering)
            {
                iJitering = jJitering = minJitering;
            }
            else
            {
                iJitering = rng->NextFloat(minJitering, maxJitering);
                jJitering = rng->NextFloat(minJitering, maxJitering);
            }

            Maths::Vec3 pixelCenter = _pixel00Loc + (_pixelDeltaU * (i + iJitering)) + (_pixelDeltaV * (j + jJitering));
            
            Maths::Vec3 rayDirection = pixelCenter - _center;

            Ray ray(_center, rayDirection);

            Color newColor = RayColor(ray, _maxDepth, object_);

            static auto linearToGamma = [](f32 color) {return color > 0 ? std::sqrt(color) : 0; };

            newColor.r = linearToGamma(newColor.r);
            newColor.g = linearToGamma(newColor.g);
            newColor.b = linearToGamma(newColor.b);
            if (useHistory)
            {
                Color& history = preImage[pixelIndex];
                Color outColor;
                outColor.r = std::lerp(newColor.r, history.r, 0.95f);
                outColor.g = std::lerp(newColor.g, history.g, 0.95f);
                outColor.b = std::lerp(newColor.b, history.b, 0.95f);

                image[pixelIndex] = outColor;
            }
            else
                image[pixelIndex] = newColor;

        }
    }
    preImage = image;
}

void Camera::ScreenShot()
{
    MESSAGE_LOG_EX("Exporting to png...", "ScreenShot", DebugVerbosity::LITTLE_DEBUG);
    std::vector<u8> correct;
    correct.reserve(image.size());
    static const Interval intensity(0.000, 0.999);
    for (const auto& c : image)
    {
        correct.push_back(256 * intensity.Clamp(c.r));
        correct.push_back(256 * intensity.Clamp(c.g));
        correct.push_back(256 * intensity.Clamp(c.b));
    }

    stbi_write_png("out.png", _imageWidth, _imageHeight, 3, correct.data(), _imageWidth * 3);
}

void Camera::Resize(f32 newWidth_, f32 newHeight_)
{
    newHeight_ = newHeight_ < 1 ? 1 : newHeight_;
    
    _imageWidth = newWidth_;
    _imageHeight = newHeight_;

    image.assign(_imageWidth * _imageHeight, { 0,0,0 });
    preImage = image;

    f32 focalLength = 1.f;
    f32 viewportHeight = 2.f;
    f32 viewportWidth = viewportHeight * (static_cast<f32>(_imageWidth) / _imageHeight);
    Maths::Vec3 cameraCenter = { 0.f, 0.f, 0.f };

    Maths::Vec3 viewportU(viewportWidth, 0.f, 0.f);
    Maths::Vec3 viewportV(0.f, -viewportHeight, 0.f);

    _pixelDeltaU = viewportU / _imageWidth;
    _pixelDeltaV = viewportV / _imageHeight;

    Maths::Vec3 viewportUpperLeft = cameraCenter - Maths::Vec3(0.f, 0.f, focalLength) - viewportU * 0.5f - viewportV * 0.5f;
    _pixel00Loc = viewportUpperLeft + (_pixelDeltaU + _pixelDeltaV) * 0.5f;
}

void Camera::Init()
{
    rng = new RNG(Settings().seed);//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

    _imageHeight = static_cast<int>(_imageWidth / _aspectRatio);
    _imageHeight = (_imageHeight < 1) ? 1 : _imageHeight; //ensure that the height is at least 1;

    image.assign(_imageWidth * _imageHeight, { 0,0,0 });
    preImage = image;

    //-- Camera ----------------------------
    f32 focalLength = 1.f;
    f32 viewportHeight = 2.f;
    f32 viewportWidth = viewportHeight * (static_cast<f32>(_imageWidth) / _imageHeight);
    Maths::Vec3 cameraCenter = { 0.f, 0.f, 0.f };

    Maths::Vec3 viewportU(viewportWidth, 0.f, 0.f);
    Maths::Vec3 viewportV(0.f, -viewportHeight, 0.f);

    _pixelDeltaU = viewportU / _imageWidth;
    _pixelDeltaV = viewportV / _imageHeight;

    Maths::Vec3 viewportUpperLeft = cameraCenter - Maths::Vec3(0.f, 0.f, focalLength) - viewportU * 0.5f - viewportV * 0.5f;
    _pixel00Loc = viewportUpperLeft + (_pixelDeltaU + _pixelDeltaV) * 0.5f;
}

Color Camera::RayColor(const Ray& ray_, s32 depth_, const IHittable& object_) const
{
    if (depth_ <= 0)
        return { 0,0,0 };

    HitRecord record;
    if (object_.Hit(ray_, Interval(0.001, Maths::Constants::INF), record))
    {
        //return (record.normal + 1) * 0.5;
        Maths::Vec3 direction = record.normal + rng->NextVec3Unit();//rng->NextOnHemisphere(record.normal);
        Color res = RayColor(Ray(record.p, direction), depth_ - 1, object_);
        return Color(res.r * 0.5, res.g * 0.5, res.b * 0.5);
    }

    Maths::Vec3 unitDirection = Maths::Vectors::Normalize(ray_.dir);
    f32 a = 0.5f * (unitDirection.y + 1.f);
    return Maths::Vec3(1.f) * (1.f - a) + Maths::Vec3(0.5f, 0.7f, 1.f) * a;
}
