cmake_minimum_required(VERSION 3.15)
project(ShaderLab)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(RenderGraph)
add_subdirectory(Backend)
add_subdirectory(libs)

add_executable(ShaderLab main.cpp Backend/Handle.h Backend/DriverEnums.h)
target_link_libraries(ShaderLab RenderGraph)