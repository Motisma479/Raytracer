cmake_minimum_required(VERSION 3.10)
project(Raytracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Add LibMath submodule to the include directories
include_directories( "Externals/LibMath" )
#--GLFW-RELATED-------------------------------------------------------
set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
add_subdirectory( "Externals/GLFW" )
#--GLAD---------------------------------------------------------------
add_subdirectory( "Externals/GLAD/cmake" )
glad_add_library(glad_gl STATIC
    REPRODUCIBLE
    API gl:core=4.6   # change version if needed
)
#--IMGUI--------------------------------------------------------------
include_directories( "Externals/IMGUI" )
set(EXTERNALS
    "Externals/IMGUI/imgui.cpp"
    "Externals/IMGUI/imgui_demo.cpp"
    "Externals/IMGUI/imgui_draw.cpp"
    "Externals/IMGUI/imgui_tables.cpp"
    "Externals/IMGUI/imgui_widgets.cpp"
    "Externals/IMGUI/backends/imgui_impl_opengl3.cpp"
    "Externals/IMGUI/backends/imgui_impl_glfw.cpp"
)
#--STB----------------------------------------------------------------
include_directories( "Externals/STB" )
#---------------------------------------------------------------------

include_directories( "Headers" )
set(PRECOMPILED_HEADER "Headers/pch.hpp")
set(SOURCES
    "Sources/main.cpp"

    "Sources/Core/Color.cpp"
    "Sources/Core/Hittable.cpp"
    "Sources/Core/Interval.cpp"
    "Sources/Core/Random.cpp"
    "Sources/Core/Ray.cpp"
    "Sources/Core/Settings.cpp"
    "Sources/Core/Window.cpp"

    "Sources/Hittable/HittableList.cpp"
    "Sources/Hittable/Sphere.cpp"

    "Sources/Material/Lambertian.cpp"
    "Sources/Material/Metal.cpp"

    "Sources/LowRenderer/Camera.cpp"
)

set(PLATFORM_SOURCES)

if(WIN32)
    set(PLATFORM_SOURCES
        "Sources/Platform/Windows/Clipboard.cpp"
    )
endif()

add_executable(${PROJECT_NAME} ${EXTERNALS} ${SOURCES} ${PLATFORM_SOURCES})
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PRECOMPILED_HEADER})
target_link_libraries( ${PROJECT_NAME} PRIVATE glfw glad_gl)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_LIST_DIR}/icon.png ${CMAKE_CURRENT_BINARY_DIR}/icon.png
    COMMENT "Copying icon.png to output directory"
)