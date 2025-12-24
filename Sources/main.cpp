#include <iostream>
#include <fstream>
#include <iomanip>

#include "Core/Color.hpp"
#include "Core/Ray.hpp"

bool HitSphere(const Maths::Vec3& center_, f32 radius_, const Ray& ray_)
{
    Maths::Vec3 oc = center_ - ray_.orig;
    f32 a = Maths::Vectors::DotProduct(ray_.dir, ray_.dir);
    f32 b = -2.f * Maths::Vectors::DotProduct(ray_.dir, oc);
    f32 c = Maths::Vectors::DotProduct(oc, oc) - radius_ * radius_;
    f32 discriminant = b * b - 4 * a * c;
    return discriminant >= 0;
}

Color RayColor(const Ray& ray_)
{
    if (HitSphere({ 0.f, 0.f, -1.f }, 0.5f, ray_))
        return { 1.f, 0.f, 0.f };

    Maths::Vec3 unitDirection = Maths::Vectors::Normalize(ray_.dir);
    f32 a = 0.5f * (unitDirection.y + 1.f);
    return Maths::Vec3(1.f) * (1.f - a) + Maths::Vec3(0.5f, 0.7f, 1.f) * a;
}

int main(int argc, char* argv[])
{
    //--------------------------------------
    //All argument are listed here, a settings struct would be nice to enable more informations etc..
    for(int i = 0; i<argc; ++i)
    {
        std::cout << argv[i] << std::endl;
    }
    //--------------------------------------


    float aspectRatio = 16.f / 9.f;

    s32 image_width = 400;
    s32 image_height = static_cast<int>(image_width / aspectRatio);
    image_height = (image_height < 1) ? 1 : image_height; //ensure that the height is at least 1;

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

    std::string filename = "Test.PPM";
    std::ofstream ostrm(filename, std::ios::binary);
    ostrm << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        
    //std::cout << std::setfill('0') << "progress:\nline: 000; row: 000";
    std::cout << std::setfill('0') << "progress: 000%";

    for (s32 j = 0; j < image_height; j++) {
        for (s32 i = 0; i < image_width; i++) {
            
            Maths::Vec3 pixelCenter = pixel00Loc + (pixelDeltaU * i) + (pixelDeltaV * j);
            Maths::Vec3 rayDirection = pixelCenter - cameraCenter;
            
            Ray ray(cameraCenter, rayDirection);

            Color col = RayColor(ray);

            ostrm << col.RGB255_str() << '\n';
            //std::cout << "\rline: " << std::setw(3) << j << "; row: " << std::setw(3) << i; //need a better log system because it cost too much
        }
        std::cout << "\rprogress: " << std::setw(3) << static_cast<s32>((100 * j) / image_height) << "%"; //not the best but it is alright
    }
    std::cout << "\rprogress: " << std::setw(3) << 100 << "%";

    std::cout << "\nImage Finished" << std::endl;

    return 0;
}