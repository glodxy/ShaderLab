cmake_minimum_required(VERSION 3.15)
project(RenderGraph)

add_library(RenderGraph SHARED RenderGraphHandle.h details/DependencyGraph.h DependencyGraph.cpp RenderGraphTexture.cpp RenderGraphTexture.h Allocator/ResourceAllocator.cpp Allocator/ResourceAllocator.h)
