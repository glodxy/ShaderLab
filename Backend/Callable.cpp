//
// Created by glodxy on 2021/4/22.
//

#include <cassert>
#include "Callable.h"

namespace GCore::RG::Backend{
    PresentCallable::PresentCallable(PresentFn fn, void* user) noexcept
            : mPresentFn(fn), mUser(user) {
        assert(fn != nullptr);
    }

    void PresentCallable::operator()(bool presentFrame) noexcept {

        mPresentFn(presentFrame, mUser);
        // Set mPresentFn to nullptr to denote that the callable has been called.
        mPresentFn = nullptr;
    }
}