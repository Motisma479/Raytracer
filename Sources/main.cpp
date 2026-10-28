#include "pch.hpp"
#include <iostream>
#include <functional>

#include "Core/Settings.hpp"
#include "LowRenderer/Camera.hpp"
#include "Core/Window.hpp"

//-- Hittable Objects -------------
#include "Hittable/HittableList.hpp"
#include "Hittable/Sphere.hpp"
//---------------------------------

//-- Materials --------------------
#include "Material/Lambertian.hpp"
#include "Material/Metal.hpp"
//---------------------------------

#define CL_HPP_ENABLE_EXCEPTIONS 
#include <CL/cl2.hpp>
#ifndef DEVICE
#define DEVICE CL_DEVICE_TYPE_DEFAULT
#endif
#define LENGTH 10

std::string LoadProgram(std::string _path)
{
    std::ifstream file(_path);
    if (!file.is_open())
    {
         ERROR_LOG_EX(std::string("cannot open: \"" + _path + "\""), "LoadProgram", DebugVerbosity::LITTLE_DEBUG);
        std::exit(1);
    }
    return std::string(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
}

int main(int argc, char* argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    /*
    cl_int CL_err = CL_SUCCESS;
    cl_uint numPlatforms = 0;
    
    CL_err = clGetPlatformIDs(0, NULL, &numPlatforms);

    if (CL_err == CL_SUCCESS)
        std::cout << numPlatforms << " platform(s) found\n";
    else
        std::cerr << "clGetPlatformIDs(" << CL_err << ")\n";

    std::vector<cl_platform_id> platforms(numPlatforms);
    clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);

    for (auto& platform : platforms) {
        size_t nameSize;
        clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, nullptr, &nameSize);

        std::string name(nameSize, '\0');
        clGetPlatformInfo(platform, CL_PLATFORM_NAME, nameSize, name.data(), nullptr);

        std::cout << name << "\n";
    }

    auto gen = []() {return rand() / (float)RAND_MAX; };

    std::vector<float> h_a(LENGTH);
    std::generate(h_a.begin(), h_a.end(), gen);
    cl::Buffer a;

    std::vector<float> h_b(LENGTH);
    std::generate(h_b.begin(), h_b.end(), gen);
    cl::Buffer b;

    std::vector<float> h_result(LENGTH);
    cl::Buffer result;

    cl::Context      context(CL_DEVICE_TYPE_DEFAULT);
    cl::CommandQueue queue(context);

    std::string sources = LoadProgram("Resources/Kernels/Type.cl") + LoadProgram("Resources/HelloWorld.cl");
    cl::Program      program(context, sources, true);
    //cl_device_id     device = 0;
    //cl_kernel        kernel = 0;
    //cl_mem           memObjects[3] = { 0, 0, 0 };
    //cl_int           errNum;

    
    
    auto kernel = cl::KernelFunctor<cl::Buffer, cl::Buffer, cl::Buffer>(program, "hello_kernell");
    a = cl::Buffer(context, std::begin(h_a), std::end(h_a), true);
    b = cl::Buffer(context, std::begin(h_b), std::end(h_b), true);
    result = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * LENGTH);
    
    try {
        kernel(cl::EnqueueArgs(queue, cl::NDRange(LENGTH)), a, b, result);
        queue.finish();
        cl::copy(queue, result, begin(h_result), end(h_result));
    }
    catch (cl::Error& e) {
        std::cerr << "OpenCL error: " << e.what() << " (" << e.err() << ")\n";
    }

    

    int correct = 0;
    float tmp;
    for (int i = 0; i < LENGTH; i++) {
        tmp = h_a[i] + h_b[i];       // expected value for d_c[i]
        tmp -= h_result[i];               // compute errors
        if (tmp * tmp < 0.001 * 0.001) { // correct if square deviation is less
            correct++;                 //  than tolerance squared
        }
        else {
            printf(" tmp %f h_a %f h_b %f  h_c %f \n", tmp, h_a[i], h_b[i], h_result[i]);
        }
    }

    printf("vector add to find C = A+B:  %d out of %d results were correct.\n",
        correct, LENGTH);



    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

    for (auto& device : devices) {
        std::cout << "Name:   " << device.getInfo<CL_DEVICE_NAME>() << "\n";
        std::cout << "Vendor: " << device.getInfo<CL_DEVICE_VENDOR>() << "\n";
        std::cout << "Type:   ";
        cl_device_type type = device.getInfo<CL_DEVICE_TYPE>();
        if (type & CL_DEVICE_TYPE_GPU)  std::cout << "GPU\n";
        if (type & CL_DEVICE_TYPE_CPU)  std::cout << "CPU\n";
    }

    //return 0; //end next is raytracer
    */
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
    //std::shared_ptr<Material> m1 = std::make_shared<Lambertian>(Lambertian({ 0.1f, 0.2f, 0.5f }));
    std::shared_ptr<Material> m1 = std::make_shared<Lambertian>(Lambertian({ 185.f/255.f, 0.1f, 0.1f }));              //red
    //std::shared_ptr<Material> m2 = std::make_shared<Lambertian>(Lambertian({ 0.8f, 0.8f, 0.0f }));
    std::shared_ptr<Material> m2 = std::make_shared<Lambertian>(Lambertian({ 185.f/255.f, 122.f/255.f, 87.f/255.f })); //ground color
    std::shared_ptr<Material> mr1 = std::make_shared<Metal>(Metal({ 0.8f, 0.8f, 0.8f }));                              //silver ball
    std::shared_ptr<Material> mr2 = std::make_shared<Metal>(Metal({ 0.8f, 0.6f, 0.2f }));                              //gold bass
    std::shared_ptr<Material> mr3 = std::make_shared<Metal>(Metal({ 0.9f, 0.6f, 0.1f }));                              //copper bass


    //-- Scene - 1 -----------------------------
    HittableList objects;
    //Sphere s({ 0.f,0.f,1.f }, 0.5f);
    objects.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, 0.f, -1.f), 0.5f, m1));
    objects.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, -100.5f, -1.f), 100.f, m2));
    
    //-- Scene - 2 -----------------------------
    HittableList objects2;
    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, -100.5f, -1.f), 100.f, m2));//ground
    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, 0.f, -2.2f), 0.5f, m1));    //red ball
    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(-1.0f, 0.f, -2.f), 0.5f, mr1));  //silver ball
    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(1.0f, 0.f, -2.f), 0.5f, mr2));   //gold ball
    for (u32 i = 0; i < 20; ++i)
    {
        f32 z = RNG::Get().NextFloat(-1, -0.5);
        //f32 y = -0.2 * z / -1;
        f32 y = std::lerp(-0.9f, -0.2f, z/-1);
        if(i%3 == 0)
            objects2.Add(std::make_shared<Sphere>(Maths::Vec3(RNG::Get().NextFloat(-1.5f, 1.5f), y, z), RNG::Get().NextFloat(0.1f, 0.2f), std::make_shared<Metal>(Metal({ RNG::Get().NextFloat(0.f, 0.8f), RNG::Get().NextFloat(0.f, 0.8f), RNG::Get().NextFloat(0.f, 0.8f) }))));    //red ball
        else
            objects2.Add(std::make_shared<Sphere>(Maths::Vec3(RNG::Get().NextFloat(-1.5f, 1.5f), y, z), RNG::Get().NextFloat(0.1f, 0.2f), std::make_shared<Lambertian>(Lambertian({ RNG::Get().NextFloat(0.f, 0.8f), RNG::Get().NextFloat(0.f, 0.8f), RNG::Get().NextFloat(0.f, 0.8f) }))));    //red ball

    }
    //for (u32 i = 0; i < 10; ++i)
    //{
    //    objects2.Add(std::make_shared<Sphere>(Maths::Vec3(RNG::Get().NextFloat(-2, 2), 0.f, RNG::Get().NextFloat(-2, 2)), RNG::Get().NextFloat(0.1, 2), mr2));   //gold ball
    //}
    //objects2.Add(std::make_shared<Sphere>(Maths::Vec3(2.0f, 0.f, -1.f), 0.5f, mr3));   //copper ball
    
    //-- Scene - 3 -----------------------------
    HittableList objects3;
    objects3.Add(std::make_shared<Sphere>(Maths::Vec3(-0.5f, 0.f, -1.f), 0.5f, mr1));
    objects3.Add(std::make_shared<Sphere>(Maths::Vec3(0.5f, 0.f, -1.f), 0.5f, mr2));
    objects3.Add(std::make_shared<Sphere>(Maths::Vec3(0.f, -100.5f, -1.f), 100.f, m2));

    //------------------------------------------
    HittableList* sceneToUse = &objects2;

    camera.Render(*sceneToUse,0,0,false);

    for(int i = 0; i < 100; ++i) //render i frame of the scene
    {
        camera.Render(*sceneToUse);
        MESSAGE_LOG_EX(std::format("frame {} rendered",i), "Render loop", DebugVerbosity::NO_DEBUG);
    }

    camera.ScreenShot();

    MESSAGE_LOG_EX("Scene2 rendered", "Final Render", DebugVerbosity::NO_DEBUG);

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
            break;
        default:
            sceneToUse = &objects;
            break;
        }

        camera.Render(*sceneToUse, 0, 0, false);
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