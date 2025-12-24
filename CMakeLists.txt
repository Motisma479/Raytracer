cmake_minimum_required(VERSION 3.10)
project(Raytracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Add LibMath submodule to the include directories
include_directories( "Externals/LibMath" )

include_directories( "Headers" )
set(SOURCES
    "Sources/main.cpp"

    "Sources/Core/Color.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})