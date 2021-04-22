//
// Created by glodxy on 2021/4/22.
//

#ifndef SHADERLAB_HANDLE_H
#define SHADERLAB_HANDLE_H

#include <cstdint>
#include <limits>
#include <utility>
#include <cassert>
#include <type_traits>
namespace GCore::RG::Backend{
    struct HwBufferObject;
    struct HwFence;
    struct HwIndexBuffer;
    struct HwProgram;
    struct HwRenderPrimitive;
    struct HwRenderTarget;
    struct HwSamplerGroup;
    struct HwStream;
    struct HwSwapChain;
    struct HwSync;
    struct HwTexture;
    struct HwTimerQuery;
    struct HwUniformBuffer;
    struct HwVertexBuffer;

/*
 * A type handle to a h/w resource
 */

//! \privatesection

    class HandleBase {
    public:
        using HandleId = uint32_t;
        static constexpr const HandleId nullid = HandleId{ std::numeric_limits<HandleId>::max() };

        constexpr HandleBase() noexcept: object(nullid) {}

        explicit HandleBase(HandleId id) noexcept : object(id) {
            assert(object != nullid); // usually means an uninitialized handle is used
        }

        HandleBase(HandleBase const& rhs) noexcept = default;
        HandleBase(HandleBase&& rhs) noexcept : object(rhs.object) {
            rhs.object = nullid;
        }

        HandleBase& operator=(HandleBase const& rhs) noexcept = default;
        HandleBase& operator=(HandleBase&& rhs) noexcept {
            std::swap(object, rhs.object);
            return *this;
        }

        explicit operator bool() const noexcept { return object != nullid; }

        void Clear() noexcept { object = nullid; }

        bool operator==(const HandleBase& rhs) const noexcept { return object == rhs.object; }
        bool operator!=(const HandleBase& rhs) const noexcept { return object != rhs.object; }

        // get this handle's handleId
        HandleId GetId() const noexcept { return object; }

    protected:
        HandleId object;
    };

    template <typename T>
    struct Handle : public HandleBase {
        using HandleBase::HandleBase;

        template<typename B, typename = std::enable_if_t<std::is_base_of<T, B>::value> >
        Handle(Handle<B> const& base) noexcept : HandleBase(base) { } // NOLINT(hicpp-explicit-conversions)

    private:
    };

// Types used by the command stream
// (we use this renaming because the macro-system doesn't deal well with "<" and ">")
    using BufferObjectHandle    = Handle<HwBufferObject>;
    using FenceHandle           = Handle<HwFence>;
    using IndexBufferHandle     = Handle<HwIndexBuffer>;
    using ProgramHandle         = Handle<HwProgram>;
    using RenderPrimitiveHandle = Handle<HwRenderPrimitive>;
    using RenderTargetHandle    = Handle<HwRenderTarget>;
    using SamplerGroupHandle    = Handle<HwSamplerGroup>;
    using StreamHandle          = Handle<HwStream>;
    using SwapChainHandle       = Handle<HwSwapChain>;
    using SyncHandle            = Handle<HwSync>;
    using TextureHandle         = Handle<HwTexture>;
    using TimerQueryHandle      = Handle<HwTimerQuery>;
    using UniformBufferHandle   = Handle<HwUniformBuffer>;
    using VertexBufferHandle    = Handle<HwVertexBuffer>;

}
#endif //SHADERLAB_HANDLE_H
