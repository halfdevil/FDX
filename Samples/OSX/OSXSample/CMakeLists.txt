cmake_minimum_required(VERSION 3.10)
project(OSXSample)

set(CMAKE_CXX_STANDARD 14)

include_directories(../ThirdParty/FreeImage/dist)
include_directories(../ThirdParty/freetype-2.8.1/include)
include_directories(../ThirdParty/gl3w-dist/include)
include_directories(../ThirdParty/glfw-3.2.1/include)
include_directories(../ThirdParty/tinyxml2)
include_directories(../ThirdParty/OpenAL\ 1.1\ SDK/include)

link_directories(../Engine/cmake-build-debug)
link_directories(../ThirdParty/FreeImage/Dist)
link_directories(../ThirdParty/freetype-2.8.1/bin)
link_directories(../ThirdParty/gl3w-dist/bin)
link_directories(../ThirdParty/glfw-3.2.1/bin)
link_directories(../ThirdParty/tinyxml2/bin/static)

include_directories(Include)
include_directories(../Engine/Include)

find_package(OpenAL)

add_executable(OSXSample
        Include/OSXSample.h
        Source/OSXSample.cpp)

target_link_libraries(OSXSample
        Engine
        freeimage
        freetype
        bz2
        iconv
        gl3w
        glfw
        tinyxml2
        ${OPENAL_LIBRARY})