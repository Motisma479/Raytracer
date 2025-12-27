#include <iostream>
#include <fstream>
#include <iomanip>

#include "Core/Settings.hpp"

#include "Core/Color.hpp"
#include "Hittable/HittableList.hpp"
#include "Hittable/Sphere.hpp"

Color RayColor(const Ray& ray_, const HittableList& lists_)
{
    HitRecord record;
    if (lists_.Hit(ray_, Interval(0, Maths::Constants::INF), record))
    {
        return (record.normal + 1) * 0.5;
    }

    Maths::Vec3 unitDirection = Maths::Vectors::Normalize(ray_.dir);
    f32 a = 0.5f * (unitDirection.y + 1.f);
    return Maths::Vec3(1.f) * (1.f - a) + Maths::Vec3(0.5f, 0.7f, 1.f) * a;
}

std::string ProgressBar(const char* full_, const char* empty_, int charLength_, int progress_)
{
    s32 fullLength = progress_ * charLength_ / 100;
    s32 emptyLength = charLength_ - fullLength;
    std::string result;
    for (int i = 0; i < fullLength; i++) result += full_;
    for (int i = 0; i < emptyLength; i++) result += empty_;
    return result;
}

//this could be better but since it should not be used to often it should be ok.
//it use a string to count the digit so allocations are made
s32 NumberOfDigit(s32 number_)
{
    return std::to_string(number_).length();
}

#ifdef _WIN32
#include <Windows.h>
#endif 
int main(int argc, char* argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    ParseArgs(argc, argv);
    const AppSettings& settings = Settings();
    if (settings.showHelp)
    {
        std::cout << "Raytracer [options] <value>\n\nOptions:\n-h, --help         Show help\n-d, --debug <int>  Set the debug Level (0 - 2)" << std::endl;
    }

    float aspectRatio = 16.f / 9.f;

    //s32 image_width = 400;
    s32 image_width = 1920;
    //s32 image_width = 3840;
    s32 image_height = static_cast<int>(image_width / aspectRatio);
    image_height = (image_height < 1) ? 1 : image_height; //ensure that the height is at least 1;

    //-- Scene -----------------------------

    HittableList objects;
    //Sphere s({ 0.f,0.f,1.f }, 0.5f);
    objects.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, 0.f, -1.f), 0.5f));
    objects.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, -100.5f, -1.f), 100.f));

    //-- Camera ----------------------------
    // This need to be it's own class in the futur

    f32 focalLength = 1.f;
    f32 viewportHeight = 2.f;
    f32 viewportWidth = viewportHeight * (static_cast<f32>(image_width) / image_height);
    Maths::Vec3 cameraCenter = { 0.f, 0.f, 0.f };

    Maths::Vec3 viewportU(viewportWidth, 0.f, 0.f);
    Maths::Vec3 viewportV(0.f, -viewportHeight, 0.f);

    Maths::Vec3 pixelDeltaU = viewportU / image_width;
    Maths::Vec3 pixelDeltaV = viewportV / image_height;

    Maths::Vec3 viewportUpperLeft = cameraCenter - Maths::Vec3(0.f, 0.f, focalLength) - viewportU * 0.5f - viewportV * 0.5f;
    Maths::Vec3 pixel00Loc = viewportUpperLeft + (pixelDeltaU + pixelDeltaV) * 0.5f;

    //-- Render ----------------------------
    //This will be used in a screen shot and will become a real time class
    std::cout << "Rendering..." << std::endl;
    s32 pixelsNumber = image_width * image_height;
    s32 imagePixelsMaxDigit = NumberOfDigit(pixelsNumber);
    
    std::string filename = "Test.PPM";
    std::ofstream ostrm(filename, std::ios::binary);
    ostrm << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    //std::cout << std::setfill('0') << "progress:\nline: 000; row: 000";
    if (settings.debugLevel == DebugLevel::LITTLE_DEBUG)
        std::clog << std::setfill('0') << "Progress: 000% [" << ProgressBar("\xe2\x96\x88","\xe2\x96\x91", 22, 0) << ']';
    if (settings.debugLevel == DebugLevel::FULL_DEBUG)
        std::clog << "Progress: 000% [" << ProgressBar("\xe2\x96\x88", "\xe2\x96\x91", 22, 0) << "] \xe2\x94\x83 Completed: " << std::setfill(' ') << std::setw(imagePixelsMaxDigit) << 0 << "px / " << pixelsNumber << "px";


    for (s32 j = 0; j < image_height; j++) {
        for (s32 i = 0; i < image_width; i++) {
            
            Maths::Vec3 pixelCenter = pixel00Loc + (pixelDeltaU * i) + (pixelDeltaV * j);
            Maths::Vec3 rayDirection = pixelCenter - cameraCenter;
            
            Ray ray(cameraCenter, rayDirection);

            Color col = RayColor(ray, objects);

            ostrm << col.RGB255_str() << '\n';
        }
        if (settings.debugLevel == DebugLevel::LITTLE_DEBUG)
        {
            s32 progress = static_cast<s32>((100 * j) / image_height);
            std::clog << "\rProgress: " << std::setw(3) << progress << "% [" << ProgressBar("\xe2\x96\x88", "\xe2\x96\x91", 22, progress) << ']'; //not the best but it is alright
        }
        if (settings.debugLevel == DebugLevel::FULL_DEBUG)
        {
            s32 progress = static_cast<s32>((100 * j) / image_height);
            std::clog << "\rProgress: " << std::setfill('0') << std::setw(3) << progress << "% [" << ProgressBar("\xe2\x96\x88", "\xe2\x96\x91", 22, progress) << "] \xe2\x94\x83 Completed: " << std::setfill(' ') << std::setw(imagePixelsMaxDigit) << image_width * j << "px / " << pixelsNumber << "px";
        }
    }
    if (settings.debugLevel == DebugLevel::LITTLE_DEBUG)
        std::clog << "\rProgress: 100% [" << ProgressBar("\xe2\x96\x88", "\xe2\x96\x91", 22, 100) << "]\n";
    
    if (settings.debugLevel == DebugLevel::FULL_DEBUG)
        std::clog << "\rProgress: 100% [" << ProgressBar("\xe2\x96\x88", "\xe2\x96\x91", 22, 100) << "] \xe2\x94\x83 Completed: " << pixelsNumber << "px / " << pixelsNumber << "px\n";

    std::cout << "Image Finished" << std::endl;

    return 0;
}