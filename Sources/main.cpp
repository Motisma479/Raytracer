#include <iostream>
#include "Core/Settings.hpp"

#include "LowRenderer/Camera.hpp"

#include "Hittable/HittableList.hpp"
#include "Hittable/Sphere.hpp"

#include <functional>

#include "Core/LogLevels.hpp"

#include "Core/Window.hpp"

#define LOG_USE_CLOG
#include "Core/Logger.hpp"

#include "Material/Lambertian.hpp"
#include "Material/Metal.hpp"

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
        std::cout << "\nRaytracer [options] <value>\n"
                  << "\nOptions:\n"
                  << "-h, --help                         Show help\n"
                  << "-d, --debug <int>                  Set the debug verbosity (0 - 2)\n"
                  << "-r, --ratio <float/literal>        Set the camera ratio (recommended: 16/9)\n"
                  << "-w, --width <int>                  Set the width of the output image\n"
                  << "-s, --seed  <unsigned long long>   Set the initial seed\n" << std::endl;
    
        //returning here is intended as the user would only use -h to find this list.
        return 0;
    }

    SET_SHOW_TO_LEVEL(settings.debugVerbosity);
    MESSAGE_LOG_EX(std::format("Initial seed= {:}", settings.seed), "App Init", DebugVerbosity::FULL_DEBUG);

    //-- RNG -------------------------------
    RNG::Get() = RNG(Settings().seed);

    //-- Camera ----------------------------
    Camera camera(settings.aspectRatio, settings.imageWidth);

    //-- Materials -----------------------------
    std::shared_ptr<Material> m1 = std::make_shared<Lambertian>(Lambertian({ 0.1f, 0.2f, 0.5f }));
    std::shared_ptr<Material> m2 = std::make_shared<Lambertian>(Lambertian({ 0.8f, 0.8f, 0.0f }));
    std::shared_ptr<Material> mr1 = std::make_shared<Metal>(Metal({ 0.8f, 0.8f, 0.8f }));
    std::shared_ptr<Material> mr2 = std::make_shared<Metal>(Metal({ 0.8f, 0.6f, 0.2f }));


    //-- Scene - 1 -----------------------------
    HittableList objects;
    //Sphere s({ 0.f,0.f,1.f }, 0.5f);
    objects.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, 0.f, -1.f), 0.5f, m1));
    objects.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, -100.5f, -1.f), 100.f, m2));
    
    //-- Scene - 2 -----------------------------
    HittableList objects2;
    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, 0.f, -1.2f), 0.5f, m1));
    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(-1.0f, 0.f, -1.f), 0.5f, mr1));
    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(1.0f, 0.f, -1.f), 0.5f, mr2));
    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, -100.5f, -1.f), 100.f, m2));
    
    //-- Scene - 3 -----------------------------
    HittableList objects3;
    objects3.Add(std::make_shared<Sphere>(Maths::Vec3(-0.5f, 0.f, -1.f), 0.5f, mr1));
    objects3.Add(std::make_shared<Sphere>(Maths::Vec3(0.5f, 0.f, -1.f), 0.5f, mr2));
    objects3.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, -100.5f, -1.f), 100.f, m2));

    //------------------------------------------
    HittableList* sceneToUse = &objects3;

    camera.Render(*sceneToUse,0,0,false);

    Window window(settings.imageWidth, settings.imageWidth / settings.aspectRatio, "Raytracer");
    window.SendToScreen(camera.GetData());

    window.screenShot = [&]() {camera.ScreenShot(); };
    window.onSceneChange = [&](s32 sceneId) {
        switch (sceneId)
        {
        case 1:
            sceneToUse = &objects2;
            break;
        case 2:
            sceneToUse = &objects3;
        default:
            sceneToUse = &objects;
            break;
        }
    };
    window.windowCallback.onSizeChange = [&](s32 width, s32 height) { camera.Resize(width, height); camera.Render(*sceneToUse, 0, 0, false); };
    while (!window.ShouldClose())
    {
        window.BeginUpdate();
        camera.Render(*sceneToUse);
        window.SendToScreen(camera.GetData());
        window.EndUpdate();
    }
    return 0;
}