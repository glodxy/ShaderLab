//
// Created by glodxy on 2021/4/22.
//

#include "RenderGraphTexture.h"
#include <algorithm>

namespace GCore::RG{
    void RenderGraphTexture::create(ResourceAllocatorInterface& resourceAllocator, const char* name,
                                   RenderGraphTexture::Descriptor const& descriptor, RenderGraphTexture::Usage usage) noexcept {
        std::array<Backend::TextureSwizzle, 4> swizzle = {
                descriptor.swizzle.r,
                descriptor.swizzle.g,
                descriptor.swizzle.b,
                descriptor.swizzle.a };
        handle = resourceAllocator.createTexture(name,
                                                 descriptor.type, descriptor.levels, descriptor.format, descriptor.samples,
                                                 descriptor.width, descriptor.height, descriptor.depth,
                                                 swizzle, usage);
    }
}