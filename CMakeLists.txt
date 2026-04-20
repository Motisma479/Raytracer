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
#---------------------------------------------------------------------

include_directories( "Headers" )
set(SOURCES
    "Sources/main.cpp"

    "Sources/Core/Color.cpp"
    "Sources/Core/Hittable.cpp"
    "Sources/Core/Interval.cpp"
    "Sources/Core/Random.cpp"
    "Sources/Core/Ray.cpp"
    "Sources/Core/Settings.cpp"

    "Sources/Hittable/HittableList.cpp"
    "Sources/Hittable/Sphere.cpp"

    "Sources/LowRenderer/Camera.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries( ${PROJECT_NAME} PRIVATE glfw glad_gl)
