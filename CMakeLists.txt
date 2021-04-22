cmake_minimum_required(VERSION 3.15)
project(ShaderLab)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(RenderGraph)

add_executable(ShaderLab main.cpp)
target_link_libraries(ShaderLab RenderGraph)