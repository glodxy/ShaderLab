//
// Created by glodxy on 2021/4/22.
//

#ifndef SHADERLAB_TARGETBUFFERINFO_H
#define SHADERLAB_TARGETBUFFERINFO_H

#include "Handle.h"
#include "DriverEnums.h"

namespace GCore::RG::Backend{

    class TargetBufferInfo {
    public:
        // ctor for 2D textures
        TargetBufferInfo(Handle<HwTexture> h, uint8_t level = 0) noexcept // NOLINT(google-explicit-constructor)
                : handle(h), level(level) { }
        // ctor for cubemaps
        TargetBufferInfo(Handle<HwTexture> h, uint8_t level, TextureCubemapFace face) noexcept
                : handle(h), level(level), face(face) { }
        // ctor for 3D textures
        TargetBufferInfo(Handle<HwTexture> h, uint8_t level, uint16_t layer) noexcept
                : handle(h), level(level), layer(layer) { }

        // texture to be used as render target
        Handle<HwTexture> handle;
        // level to be used
        uint8_t level = 0;
        union {
            // face if texture is a cubemap
            TextureCubemapFace face;
            // for 3D textures
            uint16_t layer = 0;
        };
        TargetBufferInfo() noexcept { }
    };

    //多层buffer
    class MRT {
    public:
        static constexpr int TARGET_COUNT = 4;

    private:
        TargetBufferInfo mInfos[TARGET_COUNT];

    public:
        TargetBufferInfo const& operator[](size_t i) const noexcept {
            return mInfos[i];
        }

        TargetBufferInfo& operator[](size_t i) noexcept {
            return mInfos[i];
        }

        MRT() noexcept = default;

        MRT(TargetBufferInfo const& color) noexcept // NOLINT(hicpp-explicit-conversions)
                : mInfos{ color } {
        }

        MRT(TargetBufferInfo const& color0, TargetBufferInfo const& color1) noexcept
                : mInfos{ color0, color1 } {
        }

        MRT(TargetBufferInfo const& color0, TargetBufferInfo const& color1,
            TargetBufferInfo const& color2) noexcept
                : mInfos{ color0, color1, color2 } {
        }

        MRT(TargetBufferInfo const& color0, TargetBufferInfo const& color1,
            TargetBufferInfo const& color2, TargetBufferInfo const& color3) noexcept
                : mInfos{ color0, color1, color2, color3 } {
        }

        // this is here for backward compatibility
        MRT(Handle<HwTexture> h, uint8_t level, uint16_t layer) noexcept
                : mInfos{{ h, level, layer }} {
        }
    };
}
#endif //SHADERLAB_TARGETBUFFERINFO_H
