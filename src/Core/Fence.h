//
// Created by aeternum on 9/23/21.
//

#ifndef VULKANAPI_FENCE_H
#define VULKANAPI_FENCE_H

#include "Device.h"

namespace VulkanApi
{
    class Fence
    {
    public:
        Fence(const Ref<Device>& device, bool initialize = true);
        Fence& operator=(const Fence& fence);
        virtual ~Fence();

        inline void WaitForFence(bool waitAllTime = true, uint64_t maxTimeout = UINT64_MAX) { vkWaitForFences(m_Device->GetVkDevice(), 1, &m_Fence, (waitAllTime ? VK_TRUE : VK_FALSE), maxTimeout); }
        inline void Reset() { vkResetFences(m_Device->GetVkDevice(), 1, &m_Fence); }

        inline VkFence& GetVkFence() { return m_Fence; }

    private:
        Ref<Device> m_Device;
        bool m_Initialized;

        VkFence m_Fence;
    };
}

#endif //VULKANAPI_FENCE_H
