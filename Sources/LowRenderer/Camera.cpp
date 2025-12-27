#include "LowRenderer/Camera.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

#include "Core/Ray.hpp"
#include "Core/Hittable.hpp"
#include "Core/Settings.hpp"

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

void Camera::Render(const IHittable& object_)
{
    const AppSettings& settings = Settings();

    //This will be used in a screen shot and will become a real time class
    std::cout << "Rendering..." << std::endl;
    s32 pixelsNumber = _imageWidth * _imageHeight;
    s32 imagePixelsMaxDigit = NumberOfDigit(pixelsNumber);

    if (settings.debugLevel == DebugLevel::FULL_DEBUG)
        std::clog << "Rendering Settings:\n   width:        " << _imageWidth << "\n   height:       " << _imageHeight << "\n   aspect ratio: " << _aspectRatio << std::endl;

    std::string filename = "Test.PPM";
    std::ofstream ostrm(filename, std::ios::binary);
    ostrm << "P3\n" << _imageWidth << ' ' << _imageHeight << "\n255\n";

    //std::cout << std::setfill('0') << "progress:\nline: 000; row: 000";
    if (settings.debugLevel == DebugLevel::LITTLE_DEBUG)
        std::clog << std::setfill('0') << "Progress: 000% [" << ProgressBar("\xe2\x96\x88", "\xe2\x96\x91", 22, 0) << ']';
    if (settings.debugLevel == DebugLevel::FULL_DEBUG)
        std::clog << "Progress: 000% [" << ProgressBar("\xe2\x96\x88", "\xe2\x96\x91", 22, 0) << "] \xe2\x94\x83 Completed: " << std::setfill(' ') << std::setw(imagePixelsMaxDigit) << 0 << "px / " << pixelsNumber << "px";


    for (s32 j = 0; j < _imageHeight; j++) {
        for (s32 i = 0; i < _imageWidth; i++) {

            Maths::Vec3 pixelCenter = _pixel00Loc + (_pixelDeltaU * i) + (_pixelDeltaV * j);
            Maths::Vec3 rayDirection = pixelCenter - _center;

            Ray ray(_center, rayDirection);

            Color col = RayColor(ray, object_);

            ostrm << col.RGB255_str() << '\n';
        }
        if (settings.debugLevel == DebugLevel::LITTLE_DEBUG)
        {
            s32 progress = static_cast<s32>((100 * j) / _imageHeight);
            std::clog << "\rProgress: " << std::setw(3) << progress << "% [" << ProgressBar("\xe2\x96\x88", "\xe2\x96\x91", 22, progress) << ']'; //not the best but it is alright
        }
        if (settings.debugLevel == DebugLevel::FULL_DEBUG)
        {
            s32 progress = static_cast<s32>((100 * j) / _imageHeight);
            std::clog << "\rProgress: " << std::setfill('0') << std::setw(3) << progress << "% [" << ProgressBar("\xe2\x96\x88", "\xe2\x96\x91", 22, progress) << "] \xe2\x94\x83 Completed: " << std::setfill(' ') << std::setw(imagePixelsMaxDigit) << _imageWidth * j << "px / " << pixelsNumber << "px";
        }
    }
    if (settings.debugLevel == DebugLevel::LITTLE_DEBUG)
        std::clog << "\rProgress: 100% [" << ProgressBar("\xe2\x96\x88", "\xe2\x96\x91", 22, 100) << "]\n";

    if (settings.debugLevel == DebugLevel::FULL_DEBUG)
        std::clog << "\rProgress: 100% [" << ProgressBar("\xe2\x96\x88", "\xe2\x96\x91", 22, 100) << "] \xe2\x94\x83 Completed: " << pixelsNumber << "px / " << pixelsNumber << "px\n";

    std::cout << "Image Finished" << std::endl;
}

void Camera::Init()
{
    _imageHeight = static_cast<int>(_imageWidth / _aspectRatio);
    _imageHeight = (_imageHeight < 1) ? 1 : _imageHeight; //ensure that the height is at least 1;

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

Color Camera::RayColor(const Ray& ray_, const IHittable& object_) const
{
    HitRecord record;
    if (object_.Hit(ray_, Interval(0, Maths::Constants::INF), record))
    {
        return (record.normal + 1) * 0.5;
    }

    Maths::Vec3 unitDirection = Maths::Vectors::Normalize(ray_.dir);
    f32 a = 0.5f * (unitDirection.y + 1.f);
    return Maths::Vec3(1.f) * (1.f - a) + Maths::Vec3(0.5f, 0.7f, 1.f) * a;
}
