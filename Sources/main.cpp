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

    MESSAGE_LOG_EX(std::format("Initial seed: {:}", settings.seed), "App Init", DebugVerbosity::FULL_DEBUG);

    Window window(settings.imageWidth, settings.imageWidth / settings.aspectRatio,"Raytracer");

    //-- Camera ----------------------------
    Camera camera(settings.aspectRatio, settings.imageWidth);

    //-- Scene - 1 -----------------------------
    HittableList objects;
    //Sphere s({ 0.f,0.f,1.f }, 0.5f);
    objects.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, 0.f, -1.f), 0.5f));
    objects.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, -100.5f, -1.f), 100.f));
    
    //-- Scene - 2 -----------------------------
    HittableList objects2;
    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(-0.5f, 0.f, -1.f), 0.5f));
    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(0.5f, 0.f, -1.f), 0.5f));
    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, -100.5f, -1.f), 100.f));

    //------------------------------------------
    HittableList* sceneToUse = &objects;

    camera.Render(*sceneToUse);
    window.SendToScreen(camera.GetData());

    window.screenShot = [&]() {camera.ScreenShot(); };
    window.onSceneChange = [&](s32 sceneId) {
        switch (sceneId)
        {
        case 1:
            sceneToUse = &objects2;
            break;
        default:
            sceneToUse = &objects;
            break;
        }
    };

    while (!window.ShouldClose())
    {
        window.BeginUpdate();
        camera.Render(*sceneToUse);
        window.SendToScreen(camera.GetData());
        window.EndUpdate();
    }
    return 0;
}