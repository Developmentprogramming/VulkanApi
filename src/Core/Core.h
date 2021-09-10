//
// Created by aeternum on 9/9/21.
//

#ifndef VULKANAPI_CORE_H
#define VULKANAPI_CORE_H

#include <memory>

namespace VulkanApi
{
    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Scope = std::unique_ptr<T>;

    template <typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(args...);
    }
}

#endif //VULKANAPI_CORE_H
