# cmake_minimum_required(VERSION <specify CMake version here>)
project(Ray_Tracer_in_C__)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Ray_Tracer_in_C__
        Camera.h
        Color.h
        Light.h
        Object.h
        Plane.h
        "Ray Tracer in C++.cpp"
        Ray.h
        Source.h
        Sphere.h
        Triangle.h
        Vect.h)
