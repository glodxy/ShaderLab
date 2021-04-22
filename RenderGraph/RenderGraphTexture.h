//
// Created by glodxy on 2021/4/22.
//

#ifndef SHADERLAB_RENDERGRAPHTEXTURE_H
#define SHADERLAB_RENDERGRAPHTEXTURE_H

#include <cstdint>
#include "../Backend/Handle.h"
#include "../Backend/DriverEnums.h"

namespace GCore::RG {
    //资源分配接口
    class ResourceAllocatorInterface;

    /*
     * 每个资源都包括描述符Descriptor，子资源描述符SubResourceDescriptor，掩码UsageMask
     * 都必定包含功能：Create，Destroy
     * */
    struct RenderGraphTexture {
        Backend::Handle<Backend::HwTexture> handle;

        /** describes a FrameGraphTexture resource */
        struct Descriptor {
            uint32_t width = 1;     // width of resource in pixel
            uint32_t height = 1;    // height of resource in pixel
            uint32_t depth = 1;     // # of images for 3D textures
            uint8_t levels = 1;     // # of levels for textures
            uint8_t samples = 0;    // 0=auto, 1=request not multisample, >1 only for NOT SAMPLEABLE
            Backend::SamplerType type = Backend::SamplerType::SAMPLER_2D;     // texture target type
            Backend::TextureFormat format = Backend::TextureFormat::RGBA8;    // resource internal format
            struct {
                using TS = Backend::TextureSwizzle;
                union {
                    Backend::TextureSwizzle channels[4] = {
                            TS::CHANNEL_0, TS::CHANNEL_1, TS::CHANNEL_2, TS::CHANNEL_3 };
                    struct {
                        Backend::TextureSwizzle r, g, b, a;
                    };
                };
            } swizzle;
        };

        /** Describes a FrameGraphTexture sub-resource */
        struct SubResourceDescriptor {
            uint8_t level = 0;      // resource's mip level
            uint8_t layer = 0;      // resource's layer or face
        };

        /** Usage for read and write */
        using Usage = Backend::TextureUsage;
        static constexpr Usage DEFAULT_R_USAGE = Usage::SAMPLEABLE;
        static constexpr Usage DEFAULT_W_USAGE = Usage::COLOR_ATTACHMENT;

        /**
         * Create the concrete resource
         * @param resourceAllocator resource allocator for textures and such
         * @param descriptor Descriptor to the resource
         */
        void create(ResourceAllocatorInterface& resourceAllocator, const char* name,
                    Descriptor const& descriptor, Usage usage) noexcept;

        /**
         * Destroy the concrete resource
         * @param resourceAllocator
         */
        void destroy(ResourceAllocatorInterface& resourceAllocator) noexcept;

        /**
         * Generates the Descriptor for a subresource from its parent Descriptor and its
         * SubResourceDescriptor
         * @param descriptor the parent's descriptor
         * @param srd        this subresource's  SubResourceDescriptor
         * @return           a new Descriptor suitable for this subresource
         */
        static Descriptor generateSubResourceDescriptor(Descriptor descriptor,
                                                        SubResourceDescriptor const& srd) noexcept;
    };
}


#endif //SHADERLAB_RENDERGRAPHTEXTURE_H
