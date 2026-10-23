cmake_minimum_required(VERSION 3.16)
project(Lab4Graficas CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

add_executable(renderer
    src/main.cpp
    src/framebuffer.cpp
    src/line.cpp
    src/triangle.cpp
    src/objloader.cpp
)

if(TARGET SDL2::SDL2main)
    target_link_libraries(renderer PRIVATE SDL2::SDL2main)
endif()
target_link_libraries(renderer PRIVATE SDL2::SDL2 glm::glm)
