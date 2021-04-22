//
// Created by glodxy on 2021/4/22.
//

#ifndef SHADERLAB_CALLABLE_H
#define SHADERLAB_CALLABLE_H

namespace GCore::RG::Backend {
    class  PresentCallable {
            public:

            using PresentFn = void(*)(bool presentFrame, void* user);

            PresentCallable(PresentFn fn, void* user) noexcept;
            ~PresentCallable() noexcept = default;
            PresentCallable(const PresentCallable& rhs) = default;
            PresentCallable& operator=(const PresentCallable& rhs) = default;

            /**
             * Call this PresentCallable, scheduling the associated frame for presentation. Pass false for
             * presentFrame to effectively "cancel" the presentation of the frame.
             *
             * @param presentFrame if false, will not present the frame but releases associated memory
             */
            void operator()(bool presentFrame = true) noexcept;

            private:

            PresentFn mPresentFn;
            void* mUser = nullptr;

    };

/**
 * @deprecated, FrameFinishedCallback has been renamed to SwapChain::FrameScheduledCallback.
 */
    using FrameFinishedCallback  = void(*)(PresentCallable callable, void* user);
    class Callable {

    };
}


#endif //SHADERLAB_CALLABLE_H
