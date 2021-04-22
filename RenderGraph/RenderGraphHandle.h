//
// Created by glodxy on 2021/4/22.
//

#ifndef SHADERLAB_RENDERGRAPHHANDLE_H
#define SHADERLAB_RENDERGRAPHHANDLE_H

#include <cstdint>
#include <limits>
#include <utility>

namespace GCore::RG{
    template <class T>
    class RenderGraphId;

    //句柄类
    class RenderGraphHandle {
    public:
        using Index = uint16_t;
        using Version = uint16_t;

    private:
        template<typename T>
        friend class RenderGraphId;


        // 该类不由用户创建
        RenderGraphHandle() noexcept = default;
        explicit RenderGraphHandle(Index index) noexcept : index(index) {}

        // index to the resource handle
        //定义未初始化的初始值
        static constexpr uint16_t UNINITIALIZED = std::numeric_limits<Index>::max();
        uint16_t index = UNINITIALIZED;     // index to a ResourceSlot
        Version version = 0;

    public:
        RenderGraphHandle(RenderGraphHandle const& rhs) noexcept = default;
        RenderGraphHandle(RenderGraphHandle&& rhs) noexcept
                : index(rhs.index), version(rhs.version) {
            rhs.clear();
        }
        RenderGraphHandle& operator=(RenderGraphHandle const& rhs) noexcept = default;
        RenderGraphHandle& operator=(RenderGraphHandle&& rhs) noexcept  {
            std::swap(rhs.index, index);
            std::swap(rhs.version, version);
            return *this;
        }

        bool isInitialized() const noexcept { return index != UNINITIALIZED; }

        operator bool() const noexcept { return isInitialized(); }

        void clear() noexcept { index = UNINITIALIZED; version = 0; }

        bool operator < (const RenderGraphHandle& rhs) const noexcept {
            return index < rhs.index;
        }

        bool operator == (const RenderGraphHandle& rhs) const noexcept {
            return (index == rhs.index);
        }

        bool operator != (const RenderGraphHandle& rhs) const noexcept {
            return !operator==(rhs);
        }
    };


    template<typename ResourceType>
    class RenderGraphId : public RenderGraphHandle {
    public:
        RenderGraphId() noexcept = default;
        explicit RenderGraphId(RenderGraphHandle r) : RenderGraphHandle(r) { }
    };
}

#endif //SHADERLAB_RENDERGRAPHHANDLE_H
