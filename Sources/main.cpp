#include <iostream>
#include "Core/Settings.hpp"

#include "LowRenderer/Camera.hpp"

#include "Hittable/HittableList.hpp"
#include "Hittable/Sphere.hpp"


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

    //-- Camera ----------------------------
    Camera camera(settings.aspectRatio, settings.imageWidth);

    //-- Scene -----------------------------
    HittableList objects;
    //Sphere s({ 0.f,0.f,1.f }, 0.5f);
    objects.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, 0.f, -1.f), 0.5f));
    objects.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, -100.5f, -1.f), 100.f));


    camera.Render(objects);    

    return 0;
}